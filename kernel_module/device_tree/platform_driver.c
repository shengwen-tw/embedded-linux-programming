#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>

#define DRIVER_NAME "my_platform_driver"

MODULE_AUTHOR("Shengwen Cheng");
MODULE_DESCRIPTION("Device tree driver example code");
MODULE_LICENSE("GPL");

static int my_platform_driver_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	int32_t my_property_val = 0;

	dev_info(dev, "probe.\n");

	if(of_property_read_u32(np, "my_property", &my_property_val)) {
		dev_err(dev, "cannot find 'my_poperty'.\n");
		return -EINVAL;
	}

	dev_info(dev, "property=%d.\n", my_property_val);

	return 0;
}

static int my_plaform_driver_remove(struct platform_device *pdev)
{
	dev_info(&pdev->dev, "remove.\n");
	return 0;
}

static struct of_device_id my_match_table[] = {
     { .compatible = DRIVER_NAME, },
     {}
};

MODULE_DEVICE_TABLE(of, my_match_table);

static struct platform_driver my_platform_driver = {
        .probe = my_platform_driver_probe,
        .remove = my_plaform_driver_remove,
        .driver = {
                .name = DRIVER_NAME,
                .of_match_table = of_match_ptr(my_match_table),
                .owner = THIS_MODULE,
        },
};

static int __init my_platform_driver_init(void)
{
	pr_info("my_platform_driver: loaded.\n");
	platform_driver_register(&my_platform_driver);
	return 0;
}

static void __exit my_platform_driver_exit(void)
{
	pr_info("my_platform_driver: stop.\n");
	platform_driver_unregister(&my_platform_driver);
}

module_init(my_platform_driver_init);
module_exit(my_platform_driver_exit);
