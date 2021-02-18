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

static int task05_probe(struct usb_interface *interface,
			const struct usb_device_id *id)
{
	pr_debug("Task05 - USB driver probe function is called\n");
	return 0;
}

static void task05_disconnect(struct usb_interface *interface)
{
	pr_debug("Task05 - USB driver disconnect function is called\n");
}

static struct usb_driver task05_driver = { .name = "task05_driver",
					   .probe = task05_probe,
					   .disconnect = task05_disconnect,
					   .id_table = task05_id_table };

static int __init eudyptula_challenge_init(void)
{
	int retval = 0;

	pr_debug("Task05 - USB Driver Module loaded successfully!\n");
	retval = usb_register(&task05_driver);
	if (retval)
		pr_debug("Task05 -  USB Driver Module loading unsuccessful %d",
			 retval);

	return 0;
}

static void __exit eudyptula_challenge_exit(void)
{
	usb_deregister(&task05_driver);
	pr_debug("Task05 - USB Driver Module unloaded successfully\n");
}

module_init(eudyptula_challenge_init);
module_exit(eudyptula_challenge_exit);

