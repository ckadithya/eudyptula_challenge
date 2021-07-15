// SPDX-License-Identifier: GPL-2.0-only
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/errno.h>
#include <linux/slab.h>

MODULE_AUTHOR("Adithya Chandrakasan");
MODULE_DESCRIPTION("Eudyptula Challenge Linux Misc device module Task06");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.01");

char *id = "0c5078131802";

static ssize_t task06_misc_read(struct file *file, char __user *buf,
				size_t count, loff_t *off)
{
	return simple_read_from_buffer(buf, count, off, id, strlen(id));
}

static ssize_t task06_misc_write(struct file *file, const char __user *buf,
				 size_t count, loff_t *off)
{
	ssize_t ret, result = -EINVAL;
	char *destAddr;

	pr_debug("task06 misc device write\n");
	if (count != (strlen(id) + 1))
		return result;

	destAddr = kmalloc((strlen(id) + 1), GFP_KERNEL);
	if (!destAddr)
		return result;

	ret = simple_write_to_buffer(destAddr, (strlen(id) + 1), off, buf,
				     count);
	if (ret < 0) {
		result = ret;
		goto err;
	}

	if (ret != count)
		goto err;

	if (strncmp(id, destAddr, strlen(id)) != 0)
		goto err;
	result = count;
err:
	kfree(destAddr);
	return result;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.write = task06_misc_write,
	.read = task06_misc_read
};

static struct miscdevice task06_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "eudyptula",
	.fops = &fops
};

static int __init eudyptula_challenge_init(void)
{
	int retval = 0;

	retval = misc_register(&task06_misc_device);
	if (retval) {
		pr_debug("Task06 -  Misc Device register: unsuccess ! %d",
			 retval);
		return retval;
	}
	pr_debug("Task06 - Misc Device register: success!\n");
	return 0;
}

static void __exit eudyptula_challenge_exit(void)
{
	misc_deregister(&task06_misc_device);
	pr_debug("Task06 - Misc Device register Module unloaded\n");
}

module_init(eudyptula_challenge_init);
module_exit(eudyptula_challenge_exit);
