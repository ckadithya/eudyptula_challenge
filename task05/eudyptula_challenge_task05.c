// SPDX-License-Identifier: GPL-2.0-only
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/usb/input.h>
#include <linux/hid.h>

MODULE_AUTHOR("Adithya Chandrakasan");
MODULE_DESCRIPTION("Eudyptula Challenge Linux module Task05");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.01");

static struct usb_device_id task05_id_table[] = {
	{ USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			     USB_INTERFACE_SUBCLASS_BOOT,
			     USB_INTERFACE_PROTOCOL_KEYBOARD) },
	{}
};

MODULE_DEVICE_TABLE(usb, task05_id_table);

static int __init eudyptula_challenge_init(void)
{
	pr_debug("Task05 - USB Driver Module loaded successfully!\n");
	return 0;
}

static void __exit eudyptula_challenge_exit(void)
{
	pr_debug("Task05 - USB Driver Module unloaded successfully\n");
}

module_init(eudyptula_challenge_init);
module_exit(eudyptula_challenge_exit);
