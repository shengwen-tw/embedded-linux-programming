#include "linux/init.h"
#include "linux/kernel.h"
#include "linux/module.h"
#include "linux/fs.h"
#include "linux/device.h"
#include "linux/cdev.h"

#define SUCCESS 0

#define DEVICE_NAME "chardev"
#define BUF_LEN 80

enum {
	CDEV_NOT_USED = 0,
	CDEV_EXCLUSIVE_OPEN = 1
};

/* function prototypes */
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t, loff_t *);

/* prevent multiple access to the device */
static atomic_t already_open = ATOMIC_INIT(CDEV_NOT_USED);

static char msg[BUF_LEN];

static int major; /* major number assigned to our device driver */

static struct class *cls;

/* supported operations */
static struct file_operations chardev_fops = {
	.open = device_open,
	.release = device_release,
	.read = device_read,
	.write = device_write,
};

static int __init char_dev_init(void)
{
	major = register_chrdev(0, DEVICE_NAME, &chardev_fops);

	if(major < 0) {
		pr_alert("failed to register char device with %d\n\r", major);
		return major;
	}

	pr_alert("succeeded to register the char device with major number %d.", major);

	cls = class_create(THIS_MODULE, DEVICE_NAME);
	device_create(cls, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);

	pr_info("device created on /dev/%s\n", DEVICE_NAME);

	return SUCCESS;
}

static void __exit char_dev_exit(void)
{
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);

	unregister_chrdev(major, DEVICE_NAME);
}

static int device_open(struct inode *inode, struct file *file)
{
	static int counter = 0;

	/* atomic_cmpxchg(v, old, new)                                     *
	 * stands for atomic compare and exchange, which is equivalent to: *
	 * v == old ? v = new : v = old;                                   */
	if(atomic_cmpxchg(&already_open, CDEV_NOT_USED, CDEV_EXCLUSIVE_OPEN)) {
		return -EBUSY;
	}

	sprintf(msg, "I already told you %d times \"Hello world\"!\n", counter++);
	try_module_get(THIS_MODULE);

	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	atomic_set(&already_open, CDEV_NOT_USED);

	module_put(THIS_MODULE);

	return SUCCESS;
}

static ssize_t device_read(struct file *filep, char __user *buffer,
                           size_t len, loff_t *offset)
{
	int bytes_read = 0;
	const char *msg_ptr = msg;

	if(!*(msg_ptr + *offset)) { /* we are at the end of the message */
		*offset = 0; /* reset the offset */
		return 0;    /* EOL */
	}

	/* move to the user-specified offser */
	msg_ptr += *offset;

	/* return the data into the buffer */
	while(len && *msg_ptr) {
		put_user(*(msg_ptr++), buffer++);
		len--;
		bytes_read++;
	}

	/* update offset variable */
	*offset += bytes_read;

	return bytes_read;
}

static ssize_t device_write(struct file *filep, const char __user *buff,
                            size_t len, loff_t *off)
{
	pr_alert("sorry, write operation is not supported.\n");
	return -EINVAL;
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_LICENSE("GPL");
