#include <linux/init.h>
#include <linux/module.h>
#include <linux/workqueue.h>

static struct workqueue_struct *queue = NULL;
static struct work_struct work;

static void work_handler(struct work_struct *data)
{
	pr_info("Work handler function.\n");
}

static int __init work_queue_example_init(void)
{
	queue = alloc_workqueue("HELLOWORLD", WQ_UNBOUND, 1);
	INIT_WORK(&work, work_handler);
	schedule_work(&work);

	return 0;
}

static void __exit work_queue_example_exit(void)
{
	destroy_workqueue(queue);
}

module_init(work_queue_example_init);
module_exit(work_queue_example_exit);

MODULE_DESCRIPTION("Work queue example");
MODULE_LICENSE("GPL");
