#include <linux/kernel.h>

asmlinkage long my_syscall(void)
{
	printk("Hello World!\n");
	return 0;
}
