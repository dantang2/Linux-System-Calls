#include "array_stats.h"
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <asm/errno.h>

asmlinkage long sys_array_stats(struct array_stats* stats, long data[], long size)
{
  struct array_stats temp;
  long curr;
  long min;
  long max;
  long sum;
  long i;

  if(size <= 0)
  {
    printk("ERROR: size <= 0\n");
    return -EINVAL;
  }

  if(data == NULL)
  {
    printk("ERROR: data == NULL");
    return -EFAULT;
  }

  if(stats==NULL)
  {
    printk("ERROR: stats==NULL\n");
    return -EFAULT;
  }

  if(copy_from_user(&curr, &data[0], sizeof(long))!=0) // returns non-zero if error
  {
    printk("ERROR: Could not copy from user\n");
    printk("curr = %li, data[0]= %li\n", curr, data[0]);
    return -EFAULT;
  }

  min = curr;
  max = curr;
  sum = curr;

  for(i=1; i<size; i++)
  {
    if(copy_from_user(&curr, &data[i], sizeof(long))!=0)
    {
      printk("ERROR: Could not copy from user\n");
      printk("curr = %li, data[%li]= %li\n", curr, i, data[i]);
      return -EFAULT;
    }

    if(curr > max)
    {
      max = curr;
    }

    else if (curr < min)
    {
      min = curr;
    }

    sum += curr;
  }

  temp.min = min;
  temp.max = max;
  temp.sum = sum;

  //debugging purposes
  printk("Min: %li, Max: %li, Sum: %li\n", min, max, sum);

  if(copy_to_user(stats, &temp, sizeof(struct array_stats)) != 0)
  {
    printk("ERROR: Could not copy to user\n");
    printk("Stats array and/or this local array == NULL\n");
    return -EFAULT;
  }
  return 0;
}
