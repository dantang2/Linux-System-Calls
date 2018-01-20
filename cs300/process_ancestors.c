#include "process_ancestors.h"
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/uaccess.h>
#include <asm/errno.h>

asmlinkage long sys_process_ancestors(struct process_info info_array[], long size, long* num_filled)
{
  struct task_struct *currentProcess = current;
  struct process_info thisProcess;

  long i = 0;

  if(size<=0)
  {
    printk("ERROR: Size <= 0\n");
    return -EINVAL;
  }

  if(num_filled ==NULL)
  {
    printk("ERROR: num_filled == NULL\n");
    return -EFAULT;
  }

  if(info_array == NULL)
  {
    printk("ERROR: info_array == NULL\n");
    return -EFAULT;
  }

  while(i < size && (currentProcess->real_parent != currentProcess))
  {
    long numberOfSiblings = 0;
    long numberOfChildren = 0;
    struct list_head *travel_node = NULL;
    struct list_head *travel_node2 = NULL;

    if(i > 0)
    {
      currentProcess = currentProcess->real_parent;
    }


    thisProcess.pid = currentProcess->pid;

    strcpy(thisProcess.name, currentProcess->comm);

    thisProcess.state = currentProcess->state;

    thisProcess.uid = currentProcess->cred->uid.val;

    thisProcess.nvcsw = currentProcess->nvcsw;

    thisProcess.nivcsw = currentProcess -> nivcsw;

    if(list_empty(&(currentProcess->children))!=1)
    {
      list_for_each(travel_node, &(currentProcess->children))
      {
        numberOfChildren++;
      }
    }

    if(list_empty(&(currentProcess->sibling))!= 1)
    {
      list_for_each(travel_node2, &(currentProcess->sibling))
      {
        numberOfSiblings++;
      }
    }

    thisProcess.num_children = numberOfChildren;
    thisProcess.num_siblings = numberOfSiblings;

    //debugging purposes
    printk("pid: %li\n", (long)currentProcess->pid);
    printk("name: %li\n", (long)currentProcess->comm);
    printk("state: %li\n", (long)currentProcess->state);
    printk("uid: %li\n", (long)(currentProcess->cred)->uid.val);
    printk("nvcsw: %li\n", (long)currentProcess->nvcsw);
    printk("nivcsw: %li\n", (long)currentProcess->nivcsw);
    printk("num_children: %li\n", numberOfChildren);
    printk("num_siblings: %li\n", numberOfSiblings);
    printk("------------------------------\n");


    if(copy_to_user(&info_array[i], &thisProcess, sizeof(struct process_info)))
    {
      printk("ERROR: Could not copy to user\n");
      printk("info_array[%li] == NULL and/or thisProcess == NULL\n", i);
      return -EFAULT;
    }

    i++;

  }

  if(copy_to_user(num_filled, &i, sizeof(long)))
  {
    printk("ERROR: Could not copy to user\n");
    printk("num_filled = %li, i = %li\n", num_filled, i);
    return -EFAULT;
  }

  return 0;

}
