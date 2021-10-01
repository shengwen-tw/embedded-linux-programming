#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#ifndef DECLARE_TASKLET_OLD
#define DECLARE_TASKLET_OLD(arg1, arg2) DECLARE_TASKLET(arg1, arg2, 0L)
#endif 

static void tasklet_fn(unsigned long data)
{
	pr_info("Example tasklet starts\n");
	mdelay(5000);
	pr_info("Example tasklet ends\n");
}

static DECLARE_TASKLET_OLD(mytask, tasklet_fn);

static int __init example_tasklet_init(void)
{
	pr_info("Tasklet example init\n");
	tasklet_schedule(&mytask);
	mdelay(200);
	pr_info("Tasklet example init continues...\n");

	return 0;
}

static void __exit example_tasklet_exit(void)
{
	tasklet_kill(&mytask);
}

module_init(example_tasklet_init);
module_exit(example_tasklet_exit);

MODULE_DESCRIPTION("Tasklet example");
MODULE_LICENSE("GPL");
