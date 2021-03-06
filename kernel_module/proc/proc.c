#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

/* remain compatibility to old version */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS 
#endif

#define procfs_name "hello_world"
#define PROCFS_MAX_SIZE 1024

static struct proc_dir_entry *proc_file;

static char procfs_buffer[PROCFS_MAX_SIZE];
static unsigned long procfs_buffer_size = 0;

static ssize_t proc_file_read(struct file *file_ptr, char __user *buf,
                              size_t buf_len, loff_t *offset)
{
	char s[14] = "Hello World!\n";
	int len = sizeof(s);
	ssize_t ret = len;

	if(*offset == len) {
		return 0;
	}

	if(copy_to_user(buf, s, len)) {
		pr_info("copy to user failed.\n");
		ret = 0;
	} else {
		pr_info("procfile read: %s\n",
                        file_ptr->f_path.dentry->d_name.name);
		*offset += len;
	}

	return ret;
}

static ssize_t proc_file_write(struct file *file_ptr, const char __user *buf,
                               size_t buf_len, loff_t *offset)
{
	procfs_buffer_size = buf_len;
	if(procfs_buffer_size) {
		procfs_buffer_size = PROCFS_MAX_SIZE;
	}

	if(copy_from_user(procfs_buffer, buf, procfs_buffer_size)) {
		return -EFAULT;
	}

	procfs_buffer[procfs_buffer_size] = '\0';

	pr_info("%s", procfs_buffer);

	return procfs_buffer_size;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
	.proc_read = proc_file_read,
	.proc_write = proc_file_write,
};
#else
static const struct file_operations proc_file_fops = {
	.read = proc_file_read,
	.write = proc_file_write,
};
#endif

static int __init procfs_init(void)
{
	proc_file = proc_create(procfs_name, 0644, NULL, &proc_file_fops);
	if(NULL == proc_file) {
		proc_remove(proc_file);
		pr_alert("Error: could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}

	pr_info("/proc/%s created\n", procfs_name);
	return 0;
}

static void __exit procfs_exit(void)
{
	proc_remove(proc_file);
	pr_info("/proc/%s created\n", procfs_name);
}

module_init(procfs_init);
module_exit(procfs_exit);

MODULE_LICENSE("GPL");
