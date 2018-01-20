#include "process_ancestors.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>
#include <sys/syscall.h>

#define _CS300_TEST_ 340
#define _PROCESS_ANCESTORS_ 342
#define ANCESTOR_NAME_LEN 16

int main(int argc, char *argv[])
{
	printf("----- STARTING PROCESS_ANCESTORS SYSTEM CALL TESTS ----- \n\n");

	long num_filled;

	const long aValidSize = rand()%10 + 1;
	struct process_info listOfProcesses[aValidSize];

	printf("The valid size: %li\n\n", aValidSize);

	//test for valid answer
	printf("DIVING TO KERNEL LEVEL ... TESTING FOR VALID ANSWERS\n\n");
	int result = syscall(_PROCESS_ANCESTORS_, listOfProcesses, aValidSize, &num_filled);
	printf("\nRISING TO USER LEVEL ...\n\n");
	printf("num_filled = %li\n", num_filled);

	for(int i=0; i<num_filled; i++)
	{
		printf("listOfProcesses[%d] \n", i);
		printf("pid: %li\n", listOfProcesses[i].pid);
		printf("name: %s\n", listOfProcesses[i].name);
		printf("state: %li\n", listOfProcesses[i].state);
		printf("uid: %li\n", listOfProcesses[i].uid);
		printf("nvcsw: %li\n", listOfProcesses[i].nvcsw);
		printf("nivscw: %li\n", listOfProcesses[i].nivcsw);
		printf("num_children: %li\n", listOfProcesses[i].num_children);
		printf("num_siblings: %li\n", listOfProcesses[i].num_siblings);
		printf("---------------------------\n");
	}

	printf("\n");
	assert(result == 0);
	assert(num_filled <= aValidSize);

	//test for invalid array size
	printf("DIVING TO KERNEL LEVEL ... TESTING FOR ERRORS\n\n");
	result = syscall(_PROCESS_ANCESTORS_, listOfProcesses, (long)(-10), &num_filled);
	assert(result==-1);
	assert(errno==EINVAL);
	printf("\nRISING TO USER LEVEL ...\n\n");

	//test for no num_filled
	printf("DIVING TO KERNEL LEVEL ... TESTING FOR ERRORS\n\n");
	result = syscall(_PROCESS_ANCESTORS_, listOfProcesses, aValidSize, NULL);
	assert(result == -1);
	assert(errno == EFAULT);
	printf("\nRISING TO USER LEVEL ...\n\n");

	//test for bad pointers
	printf("DIVING TO KERNEL LEVEL ... TESTING FOR ERRORS\n\n");
	result = syscall(_PROCESS_ANCESTORS_, NULL, aValidSize, &num_filled);
	assert(result == -1);
	assert(errno == EFAULT);
	printf("\nRISING TO USER LEVEL ...\n\n");

	printf("----- TESTS COMPLETE -----\n");

	return 0;
}
