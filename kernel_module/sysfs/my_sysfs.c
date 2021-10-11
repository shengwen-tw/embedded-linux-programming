#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/sysfs.h>

static struct kobject *my_module;

static int my_variable = 0;

static ssize_t my_variable_show(struct kobject *kobj, struct kobj_attribute *attr,
                                char *buf)
{
	return sprintf(buf, "%d\n", my_variable);
}

static ssize_t my_variable_store(struct kobject *kobj, struct kobj_attribute *attr,
                                 char *buf, size_t count)
{
	sscanf(buf, "%du", &my_variable);
	return count;
}

static struct kobj_attribute my_variable_attribute =
	__ATTR(my_variable, 0660, my_variable_show, (void *)my_variable_store);

static int __init sysfs_module_init(void)
{
	int error = 0;

	pr_info("my_sysfs: Initialize successfully.\n");

	my_module = kobject_create_and_add("my_sysfs", kernel_kobj);
	if(!my_module) {
		return -ENOMEM;
	}

	error = sysfs_create_file(my_module, &my_variable_attribute.attr);
	if(error) {
		pr_info("Failed to create the 'my_variable' in  'sys/kernel/my_sysfs'.\n");
	}

	return error;
}

static void __exit sysfs_module_exit(void)
{
	pr_info("my_sysfs: Exit successfully.\n");
	kobject_put(my_module);
}

module_init(sysfs_module_init);
module_exit(sysfs_module_exit);

MODULE_LICENSE("GPL");
