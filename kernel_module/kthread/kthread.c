#include <linux/init.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct *my_task;

static int counter = 0;

static int my_kthread(void *arg)
{
	while(!kthread_should_stop()) {
		counter++;
		pr_info("kthread counter = %d.\n", counter);
		ssleep(1);
	}

	pr_info("kthread terminated.\n");
	return 0;
}

static int __init kthread_example_init(void)
{
	pr_info("Start kthread example.\n");

	my_task = kthread_create(my_kthread, NULL, "my_kthread");
	if(IS_ERR(my_kthread)) {
		pr_info("Failed to create kthread.\n");
		return PTR_ERR(my_task);
	} else {
		wake_up_process(my_task);
	}

	return 0;
}

static void __exit kthread_example_exit(void)
{
	kthread_stop(my_task);
	pr_info("Stop kthread example.\n");
}

module_init(kthread_example_init);
module_exit(kthread_example_exit);

MODULE_DESCRIPTION("kthread example");
MODULE_LICENSE("GPL");
