# Eudyptula Challenge - Task 05

## Getting started 

- I started with this task by reading the Linux Device Drivers, 3rd edition book which I downloaded online.

### Source code Explanation

- The following is the source code which I wrote after reading through the chapters 13 and 14 from the book. 
- I used vim editor to write the program.
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task05# cat eudyptula_challenge_task05.c 
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

root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task05# 
```
- The header files `<linux/module.h> <linux/kernel.h> <linux/init.h>` are used for including the essential modules, kernel info and macros respectively. We use the <linux/usb.h> <linux/usb/input.h> <linux/hid.h> headers as it has to be included by all USB device drivers where the related functions and structures are available.
- As a good coding practice we include the module related descriptions and same as from the task01. I used the pr_debug instead of printk from the task04 from the experience of using checkpatch.pl
- On refering https://elixir.bootlin.com/linux/latest/source/drivers/hid/usbhid/usbkbd.c#L385, https://elixir.bootlin.com/linux/latest/source/drivers/hid/usbhid/hid-core.c#L1650 and reading the usb_register(), usb_deregister() on the book "Linux Device Driver" - Chapter 13 USB Driver subsection - Registering a USB Driver, I got an understanding regarding all the usb_driver function callbacks (probe and disconnect). The probe function is called when a device is connected to the system, and the USB core probe function is registered. The disconnect function is called when device is disconnected to the system, and the USB core disconnect call back function is called to handle disconnection and the clean up. I require the probe and disconnect function to fulfill the requirement to call proble(), when usb keyboard is connected and disconnect() is when usb keyboard is removed from the laptop.
- I also checked the coding style with the checkpatch.pl

#### Makefile Explained
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task05# cat Makefile 
obj-m := eudyptula_challenge_task05.o
CFLAGS_eudyptula_challenge_task05.o := -DDEBUG

KDIR  :=  /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=$(PWD) modules
install:
	make -C $(KDIR) M=$(PWD) modules_install
clean:
	make -C $(KDIR) M=$(PWD) clean
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task05#
```
- Here Makefile is build against the current kernel in your system. The Makefile is having three options, all, install and clean. The `all` option is to build the eudyptula_challenge_task05.ko modules, the `install` option install eudyptula_challenge_task05.ko in the `extra` directory.
After `make install` and depmod -a. We can find the `eudyptula_challenge_task05.ko` in the extra directory.
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task05# ls -lt /lib/modules/5.11.0-eudyptula-00116-gfd373f81f19c/extra/eudyptula_challenge_task05.ko 
-rw-r--r-- 1 root root 365449 Feb 17 12:23 /lib/modules/5.11.0-eudyptula-00116-gfd373f81f19c/extra/eudyptula_challenge_task05.ko
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task05# modinfo eudyptula_challenge_task05
filename:       /lib/modules/5.11.0-eudyptula-00116-gfd373f81f19c/extra/eudyptula_challenge_task05.ko
version:        0.01
license:        GPL
description:    Eudyptula Challenge Linux module Task05
author:         Adithya Chandrakasan
srcversion:     FA943F2F7AD831EA600F668
alias:          usb:v*p*d*dc*dsc*dp*ic03isc01ip01in*
depends:        
retpoline:      Y
name:           eudyptula_challenge_task05
vermagic:       5.11.0-eudyptula-00116-gfd373f81f19c SMP mod_unload 
sig_id:         PKCS#7
signer:         Build time autogenerated kernel key
sig_key:        16:90:27:26:53:60:A2:D0:23:8A:CB:E1:1E:FD:F7:CF:0E:1E:40:C8
sig_hashalgo:   sha512
signature:      9F:45:47:73:CC:F0:3A:8E:3D:F0:34:0F:79:F4:01:96:5D:EE:31:5A:
		3F:9D:F4:E8:35:4B:ED:71:30:0A:A6:FB:7F:DF:2B:5C:D8:EE:76:0F:
		CB:7E:A5:3A:0F:FA:DB:CB:93:D5:2A:7E:74:E2:07:9E:76:DA:64:0A:
		DA:23:DB:97:97:82:C9:44:55:FE:C6:60:E2:CE:50:AF:75:F3:B5:70:
		B0:B9:1A:71:5C:E7:23:E9:F1:B9:4D:B8:AA:1B:18:E7:86:57:2E:45:
		2E:D5:7E:CD:FF:46:03:B0:74:04:35:09:86:BC:6E:14:FE:D9:1F:AD:
		12:62:D9:47:DF:97:BF:84:F7:69:39:0A:CA:73:1B:F8:A7:38:90:30:
		FC:27:A2:BF:1E:39:C4:FC:61:6C:82:BB:50:6F:70:05:B5:BD:F8:FA:
		C5:A1:74:D0:4F:7A:99:FC:3A:4E:C2:71:2A:C2:E8:5C:0D:29:99:9D:
		66:01:04:B8:23:24:1A:5E:64:03:0E:18:AC:D1:50:B4:F4:3F:DF:28:
		06:67:9E:F4:26:46:93:BC:EF:90:3C:F3:E9:88:04:D0:1C:A7:6E:08:
		36:95:F3:EF:A2:72:97:E6:07:A6:07:91:04:98:9C:9E:3A:5E:FB:AD:
		B9:49:EC:21:CD:94:4F:12:3A:69:91:09:74:00:56:D8:E4:6F:21:86:
		95:40:03:73:A2:81:40:B4:65:85:68:A6:30:FB:6A:14:A3:7D:27:07:
		45:E7:17:1D:4F:12:1F:CD:5B:97:09:C4:63:8C:47:31:0C:6E:E0:9B:
		70:BD:A9:1E:55:BA:A5:9B:09:39:18:53:7F:B7:A5:70:BB:B1:67:6A:
		05:CD:0E:3B:2A:81:E1:DD:11:9D:5B:59:0E:F7:5C:08:5E:EE:07:52:
		77:87:01:D8:6B:F3:09:FB:4F:14:28:3C:C9:A2:CB:F1:BD:5D:09:9F:
		FE:39:71:46:EE:52:8D:75:45:26:5D:71:CF:6E:B5:89:90:9E:43:88:
		59:6B:1D:FA:43:A3:C7:3F:BB:FF:10:93:3F:5C:24:E1:4D:AD:34:5C:
		F4:66:8D:22:CC:31:2A:F0:AE:F1:7A:38:F2:62:44:F1:87:CF:C5:6A:
		F2:B9:78:E0:A0:ED:0C:68:53:25:94:F5:77:24:19:56:2F:C0:5B:CF:
		72:94:28:79:13:FC:59:18:A7:F6:D0:5A:16:AF:04:37:65:65:5A:7C:
		F3:82:92:99:8F:A3:D3:CF:EE:46:94:31:87:EA:4F:AF:69:89:D3:E4:
		89:85:82:32:62:28:72:81:7E:65:9F:51:2A:99:B4:C3:B7:25:73:47:
		0F:3D:B9:95:9E:80:73:CF:85:83:C5:05
```

I have attached three logs
- eudyptula_challenge_task05_dmesg_unloaded - dmesg before the usb keyboard connected
- eudyptula_challenge_task05_dmesg_usb_connected - dmesg after the usb keyboard connected
```
[ 5103.514350] usb 1-3: new low-speed USB device number 4 using xhci_hcd
[ 5103.679749] usb 1-3: New USB device found, idVendor=03f0, idProduct=0024, bcdDevice= 1.30
[ 5103.679772] usb 1-3: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[ 5103.679782] usb 1-3: Product: HP Basic USB Keyboard
[ 5103.679789] usb 1-3: Manufacturer: CHICONY
[ 5103.812835] eudyptula_challenge_task05: loading out-of-tree module taints kernel.
[ 5103.813089] Task05 - USB Driver Module loaded successfully!
[ 5103.813118] Task05 - USB driver probe function is called
[ 5103.813888] usbcore: registered new interface driver task05_driver
```
- eudyptula_challenge_task05_dmesg_usb_disconnected - dmesg after the usb keyboard disconnected
```
[ 5103.514350] usb 1-3: new low-speed USB device number 4 using xhci_hcd
[ 5103.679749] usb 1-3: New USB device found, idVendor=03f0, idProduct=0024, bcdDevice= 1.30
[ 5103.679772] usb 1-3: New USB device strings: Mfr=1, Product=2, SerialNumber=0
[ 5103.679782] usb 1-3: Product: HP Basic USB Keyboard
[ 5103.679789] usb 1-3: Manufacturer: CHICONY
[ 5103.812835] eudyptula_challenge_task05: loading out-of-tree module taints kernel.
[ 5103.813089] Task05 - USB Driver Module loaded successfully!
[ 5103.813118] Task05 - USB driver probe function is called
[ 5103.813888] usbcore: registered new interface driver task05_driver
[ 5221.517294] usb 1-3: USB disconnect, device number 4
[ 5221.517446] Task05 - USB driver disconnect function is called
```

##### Conclusion
This exercise was quite challenging and required more reading to understand the points in the books as well as pay close attendation to the points in the links. I enjoyed working on this assignment with understanding on linux modules and real hardware drivers interactions.
