#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>

static DEFINE_MUTEX(my_mutex);

static int __init mutex_module_init(void)
{
	pr_info("Mutex example init.\n");

	if(mutex_trylock(&my_mutex) != 0) {
		pr_info("Mutex is lock.\n");

		if(mutex_is_locked(&my_mutex) == 0) {
			pr_info("The mutex failed to lock!\n");
		}

		mutex_unlock(&my_mutex);

		pr_info("Mutex is unlock.\n");
	} else {
		pr_info("Mutex failed to lock.\n");
	}

	return 0;
}

static void __exit mutex_module_exit(void)
{
	pr_info("Mutex example exit.\n");
}

module_init(mutex_module_init);
module_exit(mutex_module_exit);

MODULE_DESCRIPTION("Mutex example");
MODULE_LICENSE("GPL");


