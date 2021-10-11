#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>

static DEFINE_SPINLOCK(sl_static);
static spinlock_t sl_dynamic;

static void spinlock_example_static(void)
{
	unsigned long flags;

	spin_lock_irqsave(&sl_static, flags);
	pr_info("Lock static spinlock.\n");	

	/* Do something here safely. Because this uses 100% of CPU time,
         * this code should take no more than a few milliseconds to run
         */

	spin_unlock_irqrestore(&sl_static, flags);
	pr_info("Unlock static spinlock.\n");
}

static void spinlock_example_dynamic(void)
{
	unsigned long flags;

	spin_lock_init(&sl_dynamic);
	spin_lock_irqsave(&sl_dynamic, flags);
	pr_info("Lock dynamical spinlock.\n");	

	/* Do something here safely. Because this uses 100% of CPU time,
         * this code should take no more than a few milliseconds to run
         */

	spin_unlock_irqrestore(&sl_dynamic, flags);
	pr_info("Unlock dynamical spinlock.\n");
}

static int __init spinlock_example_init(void)
{
	pr_info("Spinlock example start.\n");

	spinlock_example_static();
	spinlock_example_dynamic();

	return 0;
}

static void __exit spinlock_example_exit(void)
{
	pr_info("Spinlock example exit.\n");
}

module_init(spinlock_example_init);
module_exit(spinlock_example_exit);

MODULE_DESCRIPTION("Spinlock example");
MODULE_LICENSE("GPL");
