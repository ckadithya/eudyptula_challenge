# Eudyptula challenge - task01
## Introduction
Writing a kernel module and a Makefile to print "Hello World!" to the kernel debug log level.
## Getting Started
- I am using Ubuntu to do this task. I refered [Writing a Simple Linux Kernel Module - blog](https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234), [Building External Modules](https://www.kernel.org/doc/html/latest/kbuild/modules.html) and [The Linux Kernel Module Programming Guide](https://tldp.org/LDP/lkmpg/2.6/html/c119.html)  where I learnt to use the following commands and write the module and Makefile.
- To get started with the developing environment, we have to install build-essential and linux-headers
```
apt-get install build-essential linux-headers-`uname -r`
```
- It installs the essential tools required to buid and run the module.

- We are creating a directory for eudyptula challenge - task01

```
mkdir -p eudyptula_challenge/task01
cd eudyptula_challenge/task01
```
### Source Code
- I used vim editor to write the following program.
```
/*
 *  eudyptula_challenge.c - Write a Linux kernel module, and stand-alone
 * Makefile, that when loaded prints to the kernel debug log level,
 * Be sure to make the module able to be unloaded as well.
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Adithya Chandrakasan");
MODULE_DESCRIPTION("Eudyptula Challenge Linux module.");
MODULE_VERSION("0.01");

static int __init eudyptula_challenge_init(void) {
  printk(KERN_DEBUG "Hello, World !\n");
  return 0;
}
static void __exit eudyptula_challenge_exit(void) {
  printk(KERN_DEBUG "Bye, World !\n");
}

module_init(eudyptula_challenge_init);
module_exit(eudyptula_challenge_exit);
```
#### Explanation of the code
- The include files are essential for including macros, kernel info and modules respectively in the code.
- If the license macro is not included the following warning will be seen when we run make all.
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make all
make -C /lib/modules/5.4.0-050400-generic/build M=/home/adithya/Workspace/eudyptula_challenge/task01 modules
make[1]: Entering directory '/usr/src/linux-headers-5.4.0-050400-generic'
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.o
  Building modules, stage 2.
  MODPOST 1 modules
WARNING: modpost: missing MODULE_LICENSE() in /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.o
see include/linux/module.h for more information
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.mod.o
  LD [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.4.0-050400-generic'

```
- So we adde macro license under GPL. The macro's author,description and version are added as a good coding practice and to set module information for `modinfo`
- The init function for loading and the exit fuction for unloading the module to the kernel are defined as static and returning an int.
- The printk() is used to communicate infomation with the user, a logging mechanism for the kernel. They come with 8 priority log levels for which the kernel has macros. As instructed I have used log level 7 KERN_DEBUG which is mainly used for debugging.
- The function call for `module_init` and `module_exit` functions are given at the end.

### Makefile
- We create following Makefile with 3 targets `all`, `clean`, and `test`.
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# cat Makefile
CONFIG_MODULE_SIG=n
CONFIG_MODULE_SIG_ALL=n
KDIR ?= "/lib/modules/$(shell uname -r)/build"
obj-m += eudyptula_challenge.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

test:
	sudo insmod eudyptula_challenge.ko
	sudo rmmod eudyptula_challenge.ko
	sudo dmesg -T
```
#### Explanation of the Makefile
- The first line tells the kbuild that there is one object in that directory called eudyptula_challenge.o. The `-m` tells that it is a module.
- The target all builds the module and creates the objects as seen below when we run `make all`. The -c flag tells to change to the directory first.
- Let us try to run the `make all`
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make all
make -C "/lib/modules/5.4.0-65-generic/build" M=/home/adithya/Workspace/eudyptula_challenge/task01 modules
make[1]: Entering directory '/usr/src/linux-headers-5.4.0-65-generic'
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.mod.o
  LD [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.4.0-65-generic'
```
- We can see that modules have .ko extentions distinguishing it from the regular object and it also has the .modinfo section with additional information about the module.
- The info can been seen as follows using the command `modinfo eudyptula_challenge.ko`
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# modinfo eudyptula_challenge.ko 
filename:       /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.ko
version:        0.01
description:    Eudyptula Challenge Linux module.
author:         Adithya Chandrakasan
license:        GPL
srcversion:     54BBF3D35381C8DD3D9AE08
depends:        
retpoline:      Y
name:           eudyptula_challenge
vermagic:       5.4.0-65-generic SMP mod_unload
```
- The target clean tells to get rid of all the objects before created.
- When we run `make clean` it removes all the files generated in the modules directory. `Module.symvers` contains all the exported symbols from the kernel and compiled modules which gets cleaned now.

```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make clean
make -C "/lib/modules/5.4.0-65-generic/build" M=/home/adithya/Workspace/eudyptula_challenge/task01 clean
make[1]: Entering directory '/usr/src/linux-headers-5.4.0-65-generic'
  CLEAN   /home/adithya/Workspace/eudyptula_challenge/task01/Module.symvers
make[1]: Leaving directory '/usr/src/linux-headers-5.4.0-65-generic'
```
- The target test is added to see the output of the kernel log without running separate commands.
- When we run the command `make test` we get the following.

```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make test
sudo insmod eudyptula_challenge.ko
sudo rmmod eudyptula_challenge.ko
sudo dmesg -T
[Tue Feb  9 11:33:26 2021] microcode: microcode updated early to revision 0xe2, date = 2020-07-14
[Tue Feb  9 11:33:26 2021] Linux version 5.4.0-65-generic (buildd@lcy01-amd64-018) (gcc version 9.3.0 (Ubuntu 9.3.0-17ubuntu1~20.04)) #73-Ubuntu SMP Mon Jan 18 17:25:17 UTC 2021 (Ubuntu 5.4.0-65.73-generic 5.4.78)
[Tue Feb  9 11:33:26 2021] Command line: BOOT_IMAGE=/boot/vmlinuz-5.4.0-65-generic root=UUID=db747fa5-b16c-4d3e-8911-a8778177263d ro earlyprink=vga quiet splash vt.handoff=7
[Tue Feb  9 11:33:26 2021] KERNEL supported cpus:
[Tue Feb  9 11:33:26 2021]   Intel GenuineIntel
[Tue Feb  9 11:33:26 2021]   AMD AuthenticAMD
[Tue Feb  9 11:33:26 2021]   Hygon HygonGenuine
[Tue Feb  9 11:33:26 2021]   Centaur CentaurHauls
[Tue Feb  9 11:33:26 2021]   zhaoxin   Shanghai  
[Tue Feb  9 11:33:26 2021] x86/fpu: Supporting XSAVE feature 0x001: 'x87 floating point registers'
[Tue Feb  9 11:33:26 2021] x86/fpu: Supporting XSAVE feature 0x002: 'SSE registers'
[Tue Feb  9 11:33:26 2021] x86/fpu: Supporting XSAVE feature 0x004: 'AVX registers'
[Tue Feb  9 11:33:26 2021] x86/fpu: Supporting XSAVE feature 0x008: 'MPX bounds registers'
[Tue Feb  9 11:33:26 2021] x86/fpu: Supporting XSAVE feature 0x010: 'MPX CSR'
[Tue Feb  9 11:33:26 2021] x86/fpu: xstate_offset[2]:  576, xstate_sizes[2]:  256
[Tue Feb  9 11:33:26 2021] x86/fpu: xstate_offset[3]:  832, xstate_sizes[3]:   64
[Tue Feb  9 11:33:26 2021] x86/fpu: xstate_offset[4]:  896, xstate_sizes[4]:   64
[Tue Feb  9 11:33:26 2021] x86/fpu: Enabled xstate features 0x1f, context size is 960 bytes, using 'compacted' format.
[Tue Feb  9 11:33:26 2021] BIOS-provided physical RAM map:
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000000000000-0x0000000000057fff] usable
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000000058000-0x0000000000058fff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000000059000-0x000000000009dfff] usable
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x000000000009e000-0x00000000000fffff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000000100000-0x0000000066957fff] usable
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000066958000-0x0000000066958fff] ACPI NVS
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000066959000-0x0000000066959fff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x000000006695a000-0x0000000068387fff] usable
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000068388000-0x0000000068c87fff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000068c88000-0x000000007ee9dfff] usable
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x000000007ee9e000-0x000000007f88dfff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x000000007f88e000-0x000000007ff7dfff] ACPI NVS
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x000000007ff7e000-0x000000007fffdfff] ACPI data
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x000000007fffe000-0x000000007fffefff] usable
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x000000007ffff000-0x000000008affffff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x00000000e0000000-0x00000000efffffff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x00000000fd000000-0x00000000fe7fffff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x00000000fec00000-0x00000000fec00fff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x00000000fed00000-0x00000000fed00fff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x00000000fed10000-0x00000000fed19fff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x00000000fed84000-0x00000000fed84fff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x00000000fee00000-0x00000000fee00fff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x00000000ffa10000-0x00000000ffffffff] reserved
[Tue Feb  9 11:33:26 2021] BIOS-e820: [mem 0x0000000100000000-0x0000000573ffffff] usable
[Tue Feb  9 11:33:26 2021] NX (Execute Disable) protection: active
[Tue Feb  9 11:33:26 2021] e820: update [mem 0x663d6018-0x663e6057] usable ==> usable
[Tue Feb  9 11:33:26 2021] e820: update [mem 0x663d6018-0x663e6057] usable ==> usable
[Tue Feb  9 11:33:26 2021] e820: update [mem 0x663b8018-0x663d5a57] usable ==> usable
[Tue Feb  9 11:33:26 2021] e820: update [mem 0x663b8018-0x663d5a57] usable ==> usable
[Tue Feb  9 11:33:26 2021] e820: update [mem 0x663a9018-0x663b7057] usable ==> usable
[Tue Feb  9 11:33:26 2021] e820: update [mem 0x663a9018-0x663b7057] usable ==> usable
[Tue Feb  9 11:33:26 2021] extended physical RAM map:
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000000000000-0x0000000000057fff] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000000058000-0x0000000000058fff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000000059000-0x000000000009dfff] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x000000000009e000-0x00000000000fffff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000000100000-0x00000000663a9017] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000663a9018-0x00000000663b7057] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000663b7058-0x00000000663b8017] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000663b8018-0x00000000663d5a57] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000663d5a58-0x00000000663d6017] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000663d6018-0x00000000663e6057] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000663e6058-0x0000000066957fff] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000066958000-0x0000000066958fff] ACPI NVS
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000066959000-0x0000000066959fff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x000000006695a000-0x0000000068387fff] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000068388000-0x0000000068c87fff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000068c88000-0x000000007ee9dfff] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x000000007ee9e000-0x000000007f88dfff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x000000007f88e000-0x000000007ff7dfff] ACPI NVS
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x000000007ff7e000-0x000000007fffdfff] ACPI data
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x000000007fffe000-0x000000007fffefff] usable
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x000000007ffff000-0x000000008affffff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000e0000000-0x00000000efffffff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000fd000000-0x00000000fe7fffff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000fec00000-0x00000000fec00fff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000fed00000-0x00000000fed00fff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000fed10000-0x00000000fed19fff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000fed84000-0x00000000fed84fff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000fee00000-0x00000000fee00fff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x00000000ffa10000-0x00000000ffffffff] reserved
[Tue Feb  9 11:33:26 2021] reserve setup_data: [mem 0x0000000100000000-0x0000000573ffffff] usable
[Tue Feb  9 11:33:26 2021] efi: EFI v2.50 by INSYDE Corp.
[Tue Feb  9 11:33:26 2021] efi:  ACPI 2.0=0x7fffd014  SMBIOS=0x7f102000  SMBIOS 3.0=0x7f100000  ESRT=0x7f103918  MEMATTR=0x6f73c018  TPMEventLog=0x68c94018 
[Tue Feb  9 11:33:26 2021] [Firmware Bug]: TPM Final Events table missing or invalid
[Tue Feb  9 11:33:26 2021] secureboot: Secure boot disabled
[Tue Feb  9 11:33:26 2021] SMBIOS 3.0.0 present.
[Tue Feb  9 11:33:26 2021] DMI: LENOVO 80XG/LNVNB161216, BIOS 4WCN47WW 06/30/2020
[Tue Feb  9 11:33:26 2021] tsc: Detected 2000.000 MHz processor
[Tue Feb  9 11:33:26 2021] tsc: Detected 1999.968 MHz TSC
[Tue Feb  9 11:33:26 2021] e820: update [mem 0x00000000-0x00000fff] usable ==> reserved
[Tue Feb  9 11:33:26 2021] e820: remove [mem 0x000a0000-0x000fffff] usable
[Tue Feb  9 11:33:26 2021] last_pfn = 0x574000 max_arch_pfn = 0x400000000
[Tue Feb  9 11:33:26 2021] MTRR default type: write-back
[Tue Feb  9 11:33:26 2021] MTRR fixed ranges enabled:
[Tue Feb  9 11:33:26 2021]   00000-9FFFF write-back
[Tue Feb  9 11:33:26 2021]   A0000-BFFFF uncachable
[Tue Feb  9 11:33:26 2021]   C0000-FFFFF write-protect
[Tue Feb  9 11:33:26 2021] MTRR variable ranges enabled:
[Tue Feb  9 11:33:26 2021]   0 base 00C0000000 mask 7FC0000000 uncachable
[Tue Feb  9 11:33:26 2021]   1 base 00A0000000 mask 7FE0000000 uncachable
[Tue Feb  9 11:33:26 2021]   2 base 0090000000 mask 7FF0000000 uncachable
[Tue Feb  9 11:33:26 2021]   3 base 008C000000 mask 7FFC000000 uncachable
[Tue Feb  9 11:33:26 2021]   4 base 008A000000 mask 7FFE000000 uncachable
[Tue Feb  9 11:33:26 2021]   5 base 0089000000 mask 7FFF000000 uncachable
[Tue Feb  9 11:33:26 2021]   6 base 0088800000 mask 7FFF800000 uncachable
[Tue Feb  9 11:33:26 2021]   7 disabled
[Tue Feb  9 11:33:26 2021]   8 disabled
[Tue Feb  9 11:33:26 2021]   9 disabled
[Tue Feb  9 11:33:26 2021] x86/PAT: Configuration [0-7]: WB  WC  UC- UC  WB  WP  UC- WT  
[Tue Feb  9 11:33:26 2021] last_pfn = 0x7ffff max_arch_pfn = 0x400000000
[Tue Feb  9 11:33:26 2021] esrt: Reserving ESRT space from 0x000000007f103918 to 0x000000007f1039c8.
[Tue Feb  9 11:33:26 2021] check: Scanning 1 areas for low memory corruption
[Tue Feb  9 11:33:26 2021] Using GB pages for direct mapping
[Tue Feb  9 11:33:26 2021] secureboot: Secure boot disabled
[Tue Feb  9 11:33:26 2021] RAMDISK: [mem 0x3d045000-0x3fffdfff]
[Tue Feb  9 11:33:26 2021] ACPI: Early table checksum verification disabled
[Tue Feb  9 11:33:26 2021] ACPI: RSDP 0x000000007FFFD014 000024 (v02 LENOVO)
[Tue Feb  9 11:33:26 2021] ACPI: XSDT 0x000000007FFC8188 000114 (v01 LENOVO CB-01    00000001      01000013)
[Tue Feb  9 11:33:26 2021] ACPI: FACP 0x000000007FFEE000 00010C (v05 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: DSDT 0x000000007FFCE000 01CD27 (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: FACS 0x000000007FF5B000 000040
[Tue Feb  9 11:33:26 2021] ACPI: UEFI 0x000000007FFFC000 000236 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: UEFI 0x000000007FFFB000 000042 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: MSDM 0x000000007FFFA000 000055 (v03 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFF9000 0004CE (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFF8000 000046 (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: TPM2 0x000000007FFF7000 000034 (v03 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFF3000 0030F8 (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: ASF! 0x000000007FFF2000 0000A5 (v32 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: ASPT 0x000000007FFF1000 000034 (v07 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: BOOT 0x000000007FFF0000 000028 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: DBGP 0x000000007FFEF000 000034 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: HPET 0x000000007FFED000 000038 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: APIC 0x000000007FFEC000 0000BC (v03 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: MCFG 0x000000007FFEB000 00003C (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: LPIT 0x000000007FFCD000 000094 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: WSMT 0x000000007FFCC000 000028 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFCB000 00029F (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFCA000 000346 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFC9000 000E41 (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFC7000 00051E (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: DBGP 0x000000007FFC6000 000034 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: DBG2 0x000000007FFC5000 000054 (v00 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFC3000 00106A (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0x000000007FFC2000 000EDE (v02 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: UEFI 0x000000007FFC1000 00063A (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: UEFI 0x000000007FFC0000 00005C (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: DMAR 0x000000007FFBF000 0000CC (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: FPDT 0x000000007FFBE000 000044 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: BGRT 0x000000007FFBD000 000038 (v01 LENOVO CB-01    00000001 ACPI 00040000)
[Tue Feb  9 11:33:26 2021] ACPI: Local APIC address 0xfee00000
[Tue Feb  9 11:33:26 2021] No NUMA configuration found
[Tue Feb  9 11:33:26 2021] Faking a node at [mem 0x0000000000000000-0x0000000573ffffff]
[Tue Feb  9 11:33:26 2021] NODE_DATA(0) allocated [mem 0x573fd5000-0x573ffffff]
[Tue Feb  9 11:33:26 2021] Zone ranges:
[Tue Feb  9 11:33:26 2021]   DMA      [mem 0x0000000000001000-0x0000000000ffffff]
[Tue Feb  9 11:33:26 2021]   DMA32    [mem 0x0000000001000000-0x00000000ffffffff]
[Tue Feb  9 11:33:26 2021]   Normal   [mem 0x0000000100000000-0x0000000573ffffff]
[Tue Feb  9 11:33:26 2021]   Device   empty
[Tue Feb  9 11:33:26 2021] Movable zone start for each node
[Tue Feb  9 11:33:26 2021] Early memory node ranges
[Tue Feb  9 11:33:26 2021]   node   0: [mem 0x0000000000001000-0x0000000000057fff]
[Tue Feb  9 11:33:26 2021]   node   0: [mem 0x0000000000059000-0x000000000009dfff]
[Tue Feb  9 11:33:26 2021]   node   0: [mem 0x0000000000100000-0x0000000066957fff]
[Tue Feb  9 11:33:26 2021]   node   0: [mem 0x000000006695a000-0x0000000068387fff]
[Tue Feb  9 11:33:26 2021]   node   0: [mem 0x0000000068c88000-0x000000007ee9dfff]
[Tue Feb  9 11:33:26 2021]   node   0: [mem 0x000000007fffe000-0x000000007fffefff]
[Tue Feb  9 11:33:26 2021]   node   0: [mem 0x0000000100000000-0x0000000573ffffff]
[Tue Feb  9 11:33:26 2021] Zeroed struct page in unavailable ranges: 23239 pages
[Tue Feb  9 11:33:26 2021] Initmem setup node 0 [mem 0x0000000000001000-0x0000000573ffffff]
[Tue Feb  9 11:33:26 2021] On node 0 totalpages: 5186873
[Tue Feb  9 11:33:26 2021]   DMA zone: 64 pages used for memmap
[Tue Feb  9 11:33:26 2021]   DMA zone: 21 pages reserved
[Tue Feb  9 11:33:26 2021]   DMA zone: 3996 pages, LIFO batch:0
[Tue Feb  9 11:33:26 2021]   DMA32 zone: 8023 pages used for memmap
[Tue Feb  9 11:33:26 2021]   DMA32 zone: 513437 pages, LIFO batch:63
[Tue Feb  9 11:33:26 2021]   Normal zone: 72960 pages used for memmap
[Tue Feb  9 11:33:26 2021]   Normal zone: 4669440 pages, LIFO batch:63
[Tue Feb  9 11:33:26 2021] Reserving Intel graphics memory at [mem 0x89000000-0x8affffff]
[Tue Feb  9 11:33:26 2021] ACPI: PM-Timer IO Port: 0x1808
[Tue Feb  9 11:33:26 2021] ACPI: Local APIC address 0xfee00000
[Tue Feb  9 11:33:26 2021] ACPI: LAPIC_NMI (acpi_id[0x01] high edge lint[0x1])
[Tue Feb  9 11:33:26 2021] ACPI: LAPIC_NMI (acpi_id[0x02] high edge lint[0x1])
[Tue Feb  9 11:33:26 2021] ACPI: LAPIC_NMI (acpi_id[0x03] high edge lint[0x1])
[Tue Feb  9 11:33:26 2021] ACPI: LAPIC_NMI (acpi_id[0x04] high edge lint[0x1])
[Tue Feb  9 11:33:26 2021] ACPI: LAPIC_NMI (acpi_id[0x05] high edge lint[0x1])
[Tue Feb  9 11:33:26 2021] ACPI: LAPIC_NMI (acpi_id[0x06] high edge lint[0x1])
[Tue Feb  9 11:33:26 2021] ACPI: LAPIC_NMI (acpi_id[0x07] high edge lint[0x1])
[Tue Feb  9 11:33:26 2021] ACPI: LAPIC_NMI (acpi_id[0x08] high edge lint[0x1])
[Tue Feb  9 11:33:26 2021] IOAPIC[0]: apic_id 2, version 32, address 0xfec00000, GSI 0-119
[Tue Feb  9 11:33:26 2021] ACPI: INT_SRC_OVR (bus 0 bus_irq 0 global_irq 2 dfl dfl)
[Tue Feb  9 11:33:26 2021] ACPI: INT_SRC_OVR (bus 0 bus_irq 9 global_irq 9 high level)
[Tue Feb  9 11:33:26 2021] ACPI: IRQ0 used by override.
[Tue Feb  9 11:33:26 2021] ACPI: IRQ9 used by override.
[Tue Feb  9 11:33:26 2021] Using ACPI (MADT) for SMP configuration information
[Tue Feb  9 11:33:26 2021] ACPI: HPET id: 0x8086a201 base: 0xfed00000
[Tue Feb  9 11:33:26 2021] e820: update [mem 0x6f6bb000-0x6f736fff] usable ==> reserved
[Tue Feb  9 11:33:26 2021] TSC deadline timer available
[Tue Feb  9 11:33:26 2021] smpboot: Allowing 4 CPUs, 0 hotplug CPUs
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x00000000-0x00000fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x00058000-0x00058fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x0009e000-0x000fffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x663a9000-0x663a9fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x663b7000-0x663b7fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x663b8000-0x663b8fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x663d5000-0x663d5fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x663d6000-0x663d6fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x663e6000-0x663e6fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x66958000-0x66958fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x66959000-0x66959fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x68388000-0x68c87fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x6f6bb000-0x6f736fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x7ee9e000-0x7f88dfff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x7f88e000-0x7ff7dfff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x7ff7e000-0x7fffdfff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x7ffff000-0x8affffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0x8b000000-0xdfffffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xe0000000-0xefffffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xf0000000-0xfcffffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfd000000-0xfe7fffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfe800000-0xfebfffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfec00000-0xfec00fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfec01000-0xfecfffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfed00000-0xfed00fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfed01000-0xfed0ffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfed10000-0xfed19fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfed1a000-0xfed83fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfed84000-0xfed84fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfed85000-0xfedfffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfee00000-0xfee00fff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xfee01000-0xffa0ffff]
[Tue Feb  9 11:33:26 2021] PM: Registered nosave memory: [mem 0xffa10000-0xffffffff]
[Tue Feb  9 11:33:26 2021] [mem 0x8b000000-0xdfffffff] available for PCI devices
[Tue Feb  9 11:33:26 2021] Booting paravirtualized kernel on bare hardware
[Tue Feb  9 11:33:26 2021] clocksource: refined-jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 7645519600211568 ns
[Tue Feb  9 11:33:26 2021] setup_percpu: NR_CPUS:8192 nr_cpumask_bits:4 nr_cpu_ids:4 nr_node_ids:1
[Tue Feb  9 11:33:26 2021] percpu: Embedded 55 pages/cpu s188416 r8192 d28672 u524288
[Tue Feb  9 11:33:26 2021] pcpu-alloc: s188416 r8192 d28672 u524288 alloc=1*2097152
[Tue Feb  9 11:33:26 2021] pcpu-alloc: [0] 0 1 2 3 
[Tue Feb  9 11:33:26 2021] Built 1 zonelists, mobility grouping on.  Total pages: 5105805
[Tue Feb  9 11:33:26 2021] Policy zone: Normal
[Tue Feb  9 11:33:26 2021] Kernel command line: BOOT_IMAGE=/boot/vmlinuz-5.4.0-65-generic root=UUID=db747fa5-b16c-4d3e-8911-a8778177263d ro earlyprink=vga quiet splash vt.handoff=7
[Tue Feb  9 11:33:26 2021] Dentry cache hash table entries: 4194304 (order: 13, 33554432 bytes, linear)
[Tue Feb  9 11:33:26 2021] Inode-cache hash table entries: 2097152 (order: 12, 16777216 bytes, linear)
[Tue Feb  9 11:33:26 2021] mem auto-init: stack:off, heap alloc:on, heap free:off
[Tue Feb  9 11:33:26 2021] Calgary: detecting Calgary via BIOS EBDA area
[Tue Feb  9 11:33:26 2021] Calgary: Unable to locate Rio Grande table in EBDA - bailing!
[Tue Feb  9 11:33:26 2021] Memory: 20187940K/20747492K available (14339K kernel code, 2399K rwdata, 4960K rodata, 2724K init, 4980K bss, 559552K reserved, 0K cma-reserved)
[Tue Feb  9 11:33:26 2021] random: get_random_u64 called from kmem_cache_open+0x2d/0x410 with crng_init=0
[Tue Feb  9 11:33:26 2021] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=4, Nodes=1
[Tue Feb  9 11:33:26 2021] Kernel/User page tables isolation: enabled
[Tue Feb  9 11:33:26 2021] ftrace: allocating 44555 entries in 175 pages
[Tue Feb  9 11:33:26 2021] rcu: Hierarchical RCU implementation.
[Tue Feb  9 11:33:26 2021] rcu: 	RCU restricting CPUs from NR_CPUS=8192 to nr_cpu_ids=4.
[Tue Feb  9 11:33:26 2021] 	Tasks RCU enabled.
[Tue Feb  9 11:33:26 2021] rcu: RCU calculated value of scheduler-enlistment delay is 25 jiffies.
[Tue Feb  9 11:33:26 2021] rcu: Adjusting geometry for rcu_fanout_leaf=16, nr_cpu_ids=4
[Tue Feb  9 11:33:26 2021] NR_IRQS: 524544, nr_irqs: 1024, preallocated irqs: 16
[Tue Feb  9 11:33:26 2021] random: crng done (trusting CPU's manufacturer)
[Tue Feb  9 11:33:26 2021] vt handoff: transparent VT on vt#7
[Tue Feb  9 11:33:26 2021] Console: colour dummy device 80x25
[Tue Feb  9 11:33:26 2021] printk: console [tty0] enabled
[Tue Feb  9 11:33:26 2021] ACPI: Core revision 20190816
[Tue Feb  9 11:33:26 2021] clocksource: hpet: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 79635855245 ns
[Tue Feb  9 11:33:26 2021] APIC: Switch to symmetric I/O mode setup
[Tue Feb  9 11:33:26 2021] DMAR: Host address width 39
[Tue Feb  9 11:33:26 2021] DMAR: DRHD base: 0x000000fed90000 flags: 0x0
[Tue Feb  9 11:33:26 2021] DMAR: dmar0: reg_base_addr fed90000 ver 1:0 cap 1c0000c40660462 ecap 7e3ff0505e
[Tue Feb  9 11:33:26 2021] DMAR: DRHD base: 0x000000fed91000 flags: 0x1
[Tue Feb  9 11:33:26 2021] DMAR: dmar1: reg_base_addr fed91000 ver 1:0 cap d2008c40660462 ecap f050da
[Tue Feb  9 11:33:26 2021] DMAR: RMRR base: 0x0000007f782000 end: 0x0000007f7a1fff
[Tue Feb  9 11:33:26 2021] DMAR: RMRR base: 0x00000088800000 end: 0x0000008affffff
[Tue Feb  9 11:33:26 2021] DMAR: ANDD device: 1 name: \_SB.PCI0.I2C0
[Tue Feb  9 11:33:26 2021] DMAR-IR: IOAPIC id 2 under DRHD base  0xfed91000 IOMMU 1
[Tue Feb  9 11:33:26 2021] DMAR-IR: HPET id 0 under DRHD base 0xfed91000
[Tue Feb  9 11:33:26 2021] DMAR-IR: Queued invalidation will be enabled to support x2apic and Intr-remapping.
[Tue Feb  9 11:33:26 2021] DMAR-IR: Enabled IRQ remapping in x2apic mode
[Tue Feb  9 11:33:26 2021] x2apic enabled
[Tue Feb  9 11:33:26 2021] Switched APIC routing to cluster x2apic.
[Tue Feb  9 11:33:26 2021] ..TIMER: vector=0x30 apic1=0 pin1=2 apic2=-1 pin2=-1
[Tue Feb  9 11:33:26 2021] clocksource: tsc-early: mask: 0xffffffffffffffff max_cycles: 0x39a8208cdd2, max_idle_ns: 881590748921 ns
[Tue Feb  9 11:33:26 2021] Calibrating delay loop (skipped), value calculated using timer frequency.. 3999.93 BogoMIPS (lpj=7999872)
[Tue Feb  9 11:33:26 2021] pid_max: default: 32768 minimum: 301
[Tue Feb  9 11:33:26 2021] LSM: Security Framework initializing
[Tue Feb  9 11:33:26 2021] Yama: becoming mindful.
[Tue Feb  9 11:33:26 2021] AppArmor: AppArmor initialized
[Tue Feb  9 11:33:26 2021] Mount-cache hash table entries: 65536 (order: 7, 524288 bytes, linear)
[Tue Feb  9 11:33:26 2021] Mountpoint-cache hash table entries: 65536 (order: 7, 524288 bytes, linear)
[Tue Feb  9 11:33:26 2021] *** VALIDATE tmpfs ***
[Tue Feb  9 11:33:26 2021] *** VALIDATE proc ***
[Tue Feb  9 11:33:26 2021] *** VALIDATE cgroup1 ***
[Tue Feb  9 11:33:26 2021] *** VALIDATE cgroup2 ***
[Tue Feb  9 11:33:26 2021] mce: CPU0: Thermal monitoring enabled (TM1)
[Tue Feb  9 11:33:26 2021] process: using mwait in idle threads
[Tue Feb  9 11:33:26 2021] Last level iTLB entries: 4KB 64, 2MB 8, 4MB 8
[Tue Feb  9 11:33:26 2021] Last level dTLB entries: 4KB 64, 2MB 0, 4MB 0, 1GB 4
[Tue Feb  9 11:33:26 2021] Spectre V1 : Mitigation: usercopy/swapgs barriers and __user pointer sanitization
[Tue Feb  9 11:33:26 2021] Spectre V2 : Mitigation: Full generic retpoline
[Tue Feb  9 11:33:26 2021] Spectre V2 : Spectre v2 / SpectreRSB mitigation: Filling RSB on context switch
[Tue Feb  9 11:33:26 2021] Spectre V2 : Enabling Restricted Speculation for firmware calls
[Tue Feb  9 11:33:26 2021] Spectre V2 : mitigation: Enabling conditional Indirect Branch Prediction Barrier
[Tue Feb  9 11:33:26 2021] Spectre V2 : User space: Mitigation: STIBP via seccomp and prctl
[Tue Feb  9 11:33:26 2021] Speculative Store Bypass: Mitigation: Speculative Store Bypass disabled via prctl and seccomp
[Tue Feb  9 11:33:26 2021] SRBDS: Mitigation: Microcode
[Tue Feb  9 11:33:26 2021] MDS: Mitigation: Clear CPU buffers
[Tue Feb  9 11:33:26 2021] Freeing SMP alternatives memory: 40K
[Tue Feb  9 11:33:26 2021] smpboot: CPU0: Intel(R) Core(TM) i3-6006U CPU @ 2.00GHz (family: 0x6, model: 0x4e, stepping: 0x3)
[Tue Feb  9 11:33:26 2021] Performance Events: PEBS fmt3+, Skylake events, 32-deep LBR, full-width counters, Intel PMU driver.
[Tue Feb  9 11:33:26 2021] ... version:                4
[Tue Feb  9 11:33:26 2021] ... bit width:              48
[Tue Feb  9 11:33:26 2021] ... generic registers:      4
[Tue Feb  9 11:33:26 2021] ... value mask:             0000ffffffffffff
[Tue Feb  9 11:33:26 2021] ... max period:             00007fffffffffff
[Tue Feb  9 11:33:26 2021] ... fixed-purpose events:   3
[Tue Feb  9 11:33:26 2021] ... event mask:             000000070000000f
[Tue Feb  9 11:33:26 2021] rcu: Hierarchical SRCU implementation.
[Tue Feb  9 11:33:26 2021] NMI watchdog: Enabled. Permanently consumes one hw-PMU counter.
[Tue Feb  9 11:33:26 2021] smp: Bringing up secondary CPUs ...
[Tue Feb  9 11:33:26 2021] x86: Booting SMP configuration:
[Tue Feb  9 11:33:26 2021] .... node  #0, CPUs:      #1 #2
[Tue Feb  9 11:33:26 2021] MDS CPU bug present and SMT on, data leak possible. See https://www.kernel.org/doc/html/latest/admin-guide/hw-vuln/mds.html for more details.
[Tue Feb  9 11:33:26 2021]  #3
[Tue Feb  9 11:33:26 2021] smp: Brought up 1 node, 4 CPUs
[Tue Feb  9 11:33:26 2021] smpboot: Max logical packages: 1
[Tue Feb  9 11:33:26 2021] smpboot: Total of 4 processors activated (15999.74 BogoMIPS)
[Tue Feb  9 11:33:26 2021] devtmpfs: initialized
[Tue Feb  9 11:33:26 2021] x86/mm: Memory block size: 128MB
[Tue Feb  9 11:33:26 2021] PM: Registering ACPI NVS region [mem 0x66958000-0x66958fff] (4096 bytes)
[Tue Feb  9 11:33:26 2021] PM: Registering ACPI NVS region [mem 0x7f88e000-0x7ff7dfff] (7274496 bytes)
[Tue Feb  9 11:33:26 2021] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 7645041785100000 ns
[Tue Feb  9 11:33:26 2021] futex hash table entries: 1024 (order: 4, 65536 bytes, linear)
[Tue Feb  9 11:33:26 2021] pinctrl core: initialized pinctrl subsystem
[Tue Feb  9 11:33:26 2021] PM: RTC time: 19:33:27, date: 2021-02-09
[Tue Feb  9 11:33:26 2021] NET: Registered protocol family 16
[Tue Feb  9 11:33:26 2021] audit: initializing netlink subsys (disabled)
[Tue Feb  9 11:33:26 2021] audit: type=2000 audit(1612899207.040:1): state=initialized audit_enabled=0 res=1
[Tue Feb  9 11:33:26 2021] EISA bus registered
[Tue Feb  9 11:33:26 2021] cpuidle: using governor ladder
[Tue Feb  9 11:33:26 2021] cpuidle: using governor menu
[Tue Feb  9 11:33:26 2021] Simple Boot Flag at 0x44 set to 0x1
[Tue Feb  9 11:33:26 2021] ACPI FADT declares the system doesn't support PCIe ASPM, so disable it
[Tue Feb  9 11:33:26 2021] ACPI: bus type PCI registered
[Tue Feb  9 11:33:26 2021] acpiphp: ACPI Hot Plug PCI Controller Driver version: 0.5
[Tue Feb  9 11:33:26 2021] PCI: MMCONFIG for domain 0000 [bus 00-ff] at [mem 0xe0000000-0xefffffff] (base 0xe0000000)
[Tue Feb  9 11:33:26 2021] PCI: MMCONFIG at [mem 0xe0000000-0xefffffff] reserved in E820
[Tue Feb  9 11:33:26 2021] PCI: Using configuration type 1 for base access
[Tue Feb  9 11:33:26 2021] HugeTLB registered 1.00 GiB page size, pre-allocated 0 pages
[Tue Feb  9 11:33:26 2021] HugeTLB registered 2.00 MiB page size, pre-allocated 0 pages
[Tue Feb  9 11:33:26 2021] ACPI: Added _OSI(Module Device)
[Tue Feb  9 11:33:26 2021] ACPI: Added _OSI(Processor Device)
[Tue Feb  9 11:33:26 2021] ACPI: Added _OSI(3.0 _SCP Extensions)
[Tue Feb  9 11:33:26 2021] ACPI: Added _OSI(Processor Aggregator Device)
[Tue Feb  9 11:33:26 2021] ACPI: Added _OSI(Linux-Dell-Video)
[Tue Feb  9 11:33:26 2021] ACPI: Added _OSI(Linux-Lenovo-NV-HDMI-Audio)
[Tue Feb  9 11:33:26 2021] ACPI: Added _OSI(Linux-HPI-Hybrid-Graphics)
[Tue Feb  9 11:33:26 2021] ACPI: 10 ACPI AML tables successfully acquired and loaded
[Tue Feb  9 11:33:26 2021] ACPI: [Firmware Bug]: BIOS _OSI(Linux) query ignored
[Tue Feb  9 11:33:26 2021] ACPI: Dynamic OEM Table Load:
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0xFFFF934B9C323000 0005AC (v02 PmRef  Cpu0Ist  00003000 INTL 20160527)
[Tue Feb  9 11:33:26 2021] ACPI: \_PR_.CPU0: _OSC native thermal LVT Acked
[Tue Feb  9 11:33:26 2021] ACPI: Dynamic OEM Table Load:
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0xFFFF934B9BDCF800 0003FF (v02 PmRef  Cpu0Cst  00003001 INTL 20160527)
[Tue Feb  9 11:33:26 2021] ACPI: Dynamic OEM Table Load:
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0xFFFF934B9BDC0400 000115 (v02 PmRef  Cpu0Hwp  00003000 INTL 20160527)
[Tue Feb  9 11:33:26 2021] ACPI: Dynamic OEM Table Load:
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0xFFFF934B9BDC1000 0001A4 (v02 PmRef  HwpLvt   00003000 INTL 20160527)
[Tue Feb  9 11:33:26 2021] ACPI: Dynamic OEM Table Load:
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0xFFFF934B9C321000 00065C (v02 PmRef  ApIst    00003000 INTL 20160527)
[Tue Feb  9 11:33:26 2021] ACPI: Dynamic OEM Table Load:
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0xFFFF934B9BDC0E00 000197 (v02 PmRef  ApHwp    00003000 INTL 20160527)
[Tue Feb  9 11:33:26 2021] ACPI: Dynamic OEM Table Load:
[Tue Feb  9 11:33:26 2021] ACPI: SSDT 0xFFFF934B9BDC0000 00018A (v02 PmRef  ApCst    00003000 INTL 20160527)
[Tue Feb  9 11:33:26 2021] ACPI: EC: EC started
[Tue Feb  9 11:33:26 2021] ACPI: EC: interrupt blocked
[Tue Feb  9 11:33:26 2021] ACPI: \_SB_.PCI0.LPCB.EC0_: Used as first EC
[Tue Feb  9 11:33:26 2021] ACPI: \_SB_.PCI0.LPCB.EC0_: GPE=0x4f, IRQ=-1, EC_CMD/EC_SC=0x66, EC_DATA=0x62
[Tue Feb  9 11:33:26 2021] ACPI: \_SB_.PCI0.LPCB.EC0_: Boot DSDT EC used to handle transactions
[Tue Feb  9 11:33:26 2021] ACPI: Interpreter enabled
[Tue Feb  9 11:33:26 2021] ACPI: (supports S0 S3 S4 S5)
[Tue Feb  9 11:33:26 2021] ACPI: Using IOAPIC for interrupt routing
[Tue Feb  9 11:33:26 2021] PCI: Using host bridge windows from ACPI; if necessary, use "pci=nocrs" and report a bug
[Tue Feb  9 11:33:26 2021] ACPI: Enabled 6 GPEs in block 00 to 7F
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: Power Resource [WRST] (on)
[Tue Feb  9 11:33:26 2021] ACPI: PCI Root Bridge [PCI0] (domain 0000 [bus 00-fe])
[Tue Feb  9 11:33:26 2021] acpi PNP0A08:00: _OSC: OS supports [ExtendedConfig ASPM ClockPM Segments MSI HPX-Type3]
[Tue Feb  9 11:33:26 2021] acpi PNP0A08:00: _OSC: OS now controls [PCIeHotplug SHPCHotplug PME AER PCIeCapability LTR]
[Tue Feb  9 11:33:26 2021] acpi PNP0A08:00: FADT indicates ASPM is unsupported, using BIOS configuration
[Tue Feb  9 11:33:26 2021] PCI host bridge to bus 0000:00
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [io  0x0000-0x0cf7 window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [io  0x0d00-0xffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000a0000-0x000bffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000c0000-0x000c3fff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000c4000-0x000c7fff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000c8000-0x000cbfff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000cc000-0x000cffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000d0000-0x000d3fff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000d4000-0x000d7fff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000d8000-0x000dbfff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x000dc000-0x000dffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0x8b000000-0xdfffffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [mem 0xfd000000-0xfe7fffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: root bus resource [bus 00-fe]
[Tue Feb  9 11:33:26 2021] pci 0000:00:00.0: [8086:1904] type 00 class 0x060000
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: [8086:1921] type 00 class 0x030000
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: reg 0x10: [mem 0xa0000000-0xa0ffffff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: reg 0x18: [mem 0x90000000-0x9fffffff 64bit pref]
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: reg 0x20: [io  0x4000-0x403f]
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: BAR 2: assigned to efifb
[Tue Feb  9 11:33:26 2021] pci 0000:00:14.0: [8086:9d2f] type 00 class 0x0c0330
[Tue Feb  9 11:33:26 2021] pci 0000:00:14.0: reg 0x10: [mem 0xa1380000-0xa138ffff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:00:14.0: PME# supported from D3hot D3cold
[Tue Feb  9 11:33:26 2021] pci 0000:00:14.2: [8086:9d31] type 00 class 0x118000
[Tue Feb  9 11:33:26 2021] pci 0000:00:14.2: reg 0x10: [mem 0xa13b0000-0xa13b0fff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:00:15.0: [8086:9d60] type 00 class 0x118000
[Tue Feb  9 11:33:26 2021] pci 0000:00:15.0: reg 0x10: [mem 0xa13b1000-0xa13b1fff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:00:16.0: [8086:9d3a] type 00 class 0x078000
[Tue Feb  9 11:33:26 2021] pci 0000:00:16.0: reg 0x10: [mem 0xa13b2000-0xa13b2fff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:00:16.0: PME# supported from D3hot
[Tue Feb  9 11:33:26 2021] pci 0000:00:17.0: [8086:282a] type 00 class 0x010400
[Tue Feb  9 11:33:26 2021] pci 0000:00:17.0: reg 0x10: [mem 0xa13a0000-0xa13a7fff]
[Tue Feb  9 11:33:26 2021] pci 0000:00:17.0: reg 0x14: [mem 0xa13b4000-0xa13b40ff]
[Tue Feb  9 11:33:26 2021] pci 0000:00:17.0: reg 0x18: [io  0x4080-0x4087]
[Tue Feb  9 11:33:26 2021] pci 0000:00:17.0: reg 0x1c: [io  0x4088-0x408b]
[Tue Feb  9 11:33:26 2021] pci 0000:00:17.0: reg 0x20: [io  0x4060-0x407f]
[Tue Feb  9 11:33:26 2021] pci 0000:00:17.0: reg 0x24: [mem 0xa1300000-0xa137ffff]
[Tue Feb  9 11:33:26 2021] pci 0000:00:17.0: PME# supported from D3hot
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.0: [8086:9d14] type 01 class 0x060400
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.0: PME# supported from D0 D3hot D3cold
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.5: [8086:9d15] type 01 class 0x060400
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.5: PME# supported from D0 D3hot D3cold
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.0: [8086:9d48] type 00 class 0x060100
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.2: [8086:9d21] type 00 class 0x058000
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.2: reg 0x10: [mem 0xa13ac000-0xa13affff]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.3: [8086:9d70] type 00 class 0x040300
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.3: reg 0x10: [mem 0xa13a8000-0xa13abfff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.3: reg 0x20: [mem 0xa1390000-0xa139ffff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.3: PME# supported from D3hot D3cold
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.4: [8086:9d23] type 00 class 0x0c0500
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.4: reg 0x10: [mem 0xa13b3000-0xa13b30ff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1f.4: reg 0x20: [io  0x4040-0x405f]
[Tue Feb  9 11:33:26 2021] pci 0000:01:00.0: [10ec:8168] type 00 class 0x020000
[Tue Feb  9 11:33:26 2021] pci 0000:01:00.0: reg 0x10: [io  0x3000-0x30ff]
[Tue Feb  9 11:33:26 2021] pci 0000:01:00.0: reg 0x18: [mem 0xa1204000-0xa1204fff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:01:00.0: reg 0x20: [mem 0xa1200000-0xa1203fff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:01:00.0: supports D1 D2
[Tue Feb  9 11:33:26 2021] pci 0000:01:00.0: PME# supported from D0 D1 D2 D3hot D3cold
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.0: PCI bridge to [bus 01]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.0:   bridge window [io  0x3000-0x3fff]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.0:   bridge window [mem 0xa1200000-0xa12fffff]
[Tue Feb  9 11:33:26 2021] pci 0000:02:00.0: [168c:0042] type 00 class 0x028000
[Tue Feb  9 11:33:26 2021] pci 0000:02:00.0: reg 0x10: [mem 0xa1000000-0xa11fffff 64bit]
[Tue Feb  9 11:33:26 2021] pci 0000:02:00.0: PME# supported from D0 D3hot D3cold
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.5: PCI bridge to [bus 02]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.5:   bridge window [mem 0xa1000000-0xa11fffff]
[Tue Feb  9 11:33:26 2021] ACPI: PCI Interrupt Link [LNKA] (IRQs 3 4 5 6 10 *11 12 14 15)
[Tue Feb  9 11:33:26 2021] ACPI: PCI Interrupt Link [LNKB] (IRQs 3 4 5 6 *10 11 12 14 15)
[Tue Feb  9 11:33:26 2021] ACPI: PCI Interrupt Link [LNKC] (IRQs 3 4 5 6 10 *11 12 14 15)
[Tue Feb  9 11:33:26 2021] ACPI: PCI Interrupt Link [LNKD] (IRQs 3 4 5 6 10 *11 12 14 15)
[Tue Feb  9 11:33:26 2021] ACPI: PCI Interrupt Link [LNKE] (IRQs 3 4 5 6 10 *11 12 14 15)
[Tue Feb  9 11:33:26 2021] ACPI: PCI Interrupt Link [LNKF] (IRQs 3 4 5 6 10 *11 12 14 15)
[Tue Feb  9 11:33:26 2021] ACPI: PCI Interrupt Link [LNKG] (IRQs 3 4 5 6 10 *11 12 14 15)
[Tue Feb  9 11:33:26 2021] ACPI: PCI Interrupt Link [LNKH] (IRQs 3 4 5 6 10 *11 12 14 15)
[Tue Feb  9 11:33:26 2021] platform MSFT0101:00: failed to claim resource 1: [mem 0xfed40000-0xfed40fff]
[Tue Feb  9 11:33:26 2021] acpi MSFT0101:00: platform device creation failed: -16
[Tue Feb  9 11:33:26 2021] ACPI: EC: interrupt unblocked
[Tue Feb  9 11:33:26 2021] ACPI: EC: event unblocked
[Tue Feb  9 11:33:26 2021] ACPI: \_SB_.PCI0.LPCB.EC0_: GPE=0x4f, IRQ=-1, EC_CMD/EC_SC=0x66, EC_DATA=0x62
[Tue Feb  9 11:33:26 2021] ACPI: \_SB_.PCI0.LPCB.EC0_: Boot DSDT EC used to handle transactions and events
[Tue Feb  9 11:33:26 2021] iommu: Default domain type: Translated 
[Tue Feb  9 11:33:26 2021] SCSI subsystem initialized
[Tue Feb  9 11:33:26 2021] libata version 3.00 loaded.
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: vgaarb: setting as boot VGA device
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: vgaarb: VGA device added: decodes=io+mem,owns=io+mem,locks=none
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: vgaarb: bridge control possible
[Tue Feb  9 11:33:26 2021] vgaarb: loaded
[Tue Feb  9 11:33:26 2021] ACPI: bus type USB registered
[Tue Feb  9 11:33:26 2021] usbcore: registered new interface driver usbfs
[Tue Feb  9 11:33:26 2021] usbcore: registered new interface driver hub
[Tue Feb  9 11:33:26 2021] usbcore: registered new device driver usb
[Tue Feb  9 11:33:26 2021] pps_core: LinuxPPS API ver. 1 registered
[Tue Feb  9 11:33:26 2021] pps_core: Software ver. 5.3.6 - Copyright 2005-2007 Rodolfo Giometti <giometti@linux.it>
[Tue Feb  9 11:33:26 2021] PTP clock support registered
[Tue Feb  9 11:33:26 2021] EDAC MC: Ver: 3.0.0
[Tue Feb  9 11:33:26 2021] Registered efivars operations
[Tue Feb  9 11:33:26 2021] PCI: Using ACPI for IRQ routing
[Tue Feb  9 11:33:26 2021] PCI: pci_cache_line_size set to 64 bytes
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x00058000-0x0005ffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x0009e000-0x0009ffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x663a9018-0x67ffffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x663b8018-0x67ffffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x663d6018-0x67ffffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x66958000-0x67ffffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x68388000-0x6bffffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x6f6bb000-0x6fffffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x7ee9e000-0x7fffffff]
[Tue Feb  9 11:33:26 2021] e820: reserve RAM buffer [mem 0x7ffff000-0x7fffffff]
[Tue Feb  9 11:33:26 2021] NetLabel: Initializing
[Tue Feb  9 11:33:26 2021] NetLabel:  domain hash size = 128
[Tue Feb  9 11:33:26 2021] NetLabel:  protocols = UNLABELED CIPSOv4 CALIPSO
[Tue Feb  9 11:33:26 2021] NetLabel:  unlabeled traffic allowed by default
[Tue Feb  9 11:33:26 2021] hpet0: at MMIO 0xfed00000, IRQs 2, 8, 0, 0, 0, 0, 0, 0
[Tue Feb  9 11:33:26 2021] hpet0: 8 comparators, 64-bit 24.000000 MHz counter
[Tue Feb  9 11:33:26 2021] clocksource: Switched to clocksource tsc-early
[Tue Feb  9 11:33:26 2021] *** VALIDATE bpf ***
[Tue Feb  9 11:33:26 2021] VFS: Disk quotas dquot_6.6.0
[Tue Feb  9 11:33:26 2021] VFS: Dquot-cache hash table entries: 512 (order 0, 4096 bytes)
[Tue Feb  9 11:33:26 2021] *** VALIDATE ramfs ***
[Tue Feb  9 11:33:26 2021] *** VALIDATE hugetlbfs ***
[Tue Feb  9 11:33:26 2021] AppArmor: AppArmor Filesystem Enabled
[Tue Feb  9 11:33:26 2021] pnp: PnP ACPI init
[Tue Feb  9 11:33:26 2021] system 00:00: [mem 0xfd000000-0xfdabffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:00: [mem 0xfdad0000-0xfdadffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:00: [mem 0xfdb00000-0xfdffffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:00: [mem 0xfe000000-0xfe01ffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:00: [mem 0xfe036000-0xfe03bfff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:00: [mem 0xfe03d000-0xfe3fffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:00: [mem 0xfe410000-0xfe7fffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:00: Plug and Play ACPI device, IDs PNP0c02 (active)
[Tue Feb  9 11:33:26 2021] system 00:01: [io  0x2000-0x20fe] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:01: Plug and Play ACPI device, IDs PNP0c02 (active)
[Tue Feb  9 11:33:26 2021] system 00:02: [io  0x0680-0x069f] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:02: [io  0xffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:02: [io  0xffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:02: [io  0xffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:02: [io  0x1800-0x18fe] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:02: [io  0x164e-0x164f] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:02: Plug and Play ACPI device, IDs PNP0c02 (active)
[Tue Feb  9 11:33:26 2021] pnp 00:03: Plug and Play ACPI device, IDs PNP0b00 (active)
[Tue Feb  9 11:33:26 2021] system 00:04: [io  0x1854-0x1857] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:04: Plug and Play ACPI device, IDs INT3f0d PNP0c02 (active)
[Tue Feb  9 11:33:26 2021] pnp 00:05: Plug and Play ACPI device, IDs PNP0303 (active)
[Tue Feb  9 11:33:26 2021] system 00:06: [mem 0xfe029000-0xfe029fff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:06: [mem 0xfe028000-0xfe028fff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:06: Plug and Play ACPI device, IDs PNP0c02 (active)
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xfed10000-0xfed17fff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xfed18000-0xfed18fff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xfed19000-0xfed19fff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xe0000000-0xefffffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xfed20000-0xfed3ffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xfed90000-0xfed93fff] could not be reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xfed45000-0xfed8ffff] could not be reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xff000000-0xffffffff] could not be reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0xfee00000-0xfeefffff] could not be reserved
[Tue Feb  9 11:33:26 2021] system 00:07: [mem 0x8b000000-0x8b01ffff] has been reserved
[Tue Feb  9 11:33:26 2021] system 00:07: Plug and Play ACPI device, IDs PNP0c02 (active)
[Tue Feb  9 11:33:26 2021] pnp: PnP ACPI: found 8 devices
[Tue Feb  9 11:33:26 2021] thermal_sys: Registered thermal governor 'fair_share'
[Tue Feb  9 11:33:26 2021] thermal_sys: Registered thermal governor 'bang_bang'
[Tue Feb  9 11:33:26 2021] thermal_sys: Registered thermal governor 'step_wise'
[Tue Feb  9 11:33:26 2021] thermal_sys: Registered thermal governor 'user_space'
[Tue Feb  9 11:33:26 2021] thermal_sys: Registered thermal governor 'power_allocator'
[Tue Feb  9 11:33:26 2021] clocksource: acpi_pm: mask: 0xffffff max_cycles: 0xffffff, max_idle_ns: 2085701024 ns
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.0: PCI bridge to [bus 01]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.0:   bridge window [io  0x3000-0x3fff]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.0:   bridge window [mem 0xa1200000-0xa12fffff]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.5: PCI bridge to [bus 02]
[Tue Feb  9 11:33:26 2021] pci 0000:00:1c.5:   bridge window [mem 0xa1000000-0xa11fffff]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 4 [io  0x0000-0x0cf7 window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 5 [io  0x0d00-0xffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 6 [mem 0x000a0000-0x000bffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 7 [mem 0x000c0000-0x000c3fff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 8 [mem 0x000c4000-0x000c7fff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 9 [mem 0x000c8000-0x000cbfff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 10 [mem 0x000cc000-0x000cffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 11 [mem 0x000d0000-0x000d3fff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 12 [mem 0x000d4000-0x000d7fff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 13 [mem 0x000d8000-0x000dbfff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 14 [mem 0x000dc000-0x000dffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 15 [mem 0x8b000000-0xdfffffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:00: resource 16 [mem 0xfd000000-0xfe7fffff window]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:01: resource 0 [io  0x3000-0x3fff]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:01: resource 1 [mem 0xa1200000-0xa12fffff]
[Tue Feb  9 11:33:26 2021] pci_bus 0000:02: resource 1 [mem 0xa1000000-0xa11fffff]
[Tue Feb  9 11:33:26 2021] NET: Registered protocol family 2
[Tue Feb  9 11:33:26 2021] tcp_listen_portaddr_hash hash table entries: 16384 (order: 6, 262144 bytes, linear)
[Tue Feb  9 11:33:26 2021] TCP established hash table entries: 262144 (order: 9, 2097152 bytes, linear)
[Tue Feb  9 11:33:26 2021] TCP bind hash table entries: 65536 (order: 8, 1048576 bytes, linear)
[Tue Feb  9 11:33:26 2021] TCP: Hash tables configured (established 262144 bind 65536)
[Tue Feb  9 11:33:26 2021] UDP hash table entries: 16384 (order: 7, 524288 bytes, linear)
[Tue Feb  9 11:33:26 2021] UDP-Lite hash table entries: 16384 (order: 7, 524288 bytes, linear)
[Tue Feb  9 11:33:26 2021] NET: Registered protocol family 1
[Tue Feb  9 11:33:26 2021] NET: Registered protocol family 44
[Tue Feb  9 11:33:26 2021] pci 0000:00:02.0: Video device with shadowed ROM at [mem 0x000c0000-0x000dffff]
[Tue Feb  9 11:33:26 2021] PCI: CLS 64 bytes, default 64
[Tue Feb  9 11:33:26 2021] Trying to unpack rootfs image as initramfs...
[Tue Feb  9 11:33:26 2021] Freeing initrd memory: 48868K
[Tue Feb  9 11:33:26 2021] DMAR: ACPI device "device:6f" under DMAR at fed91000 as 00:15.0
[Tue Feb  9 11:33:26 2021] PCI-DMA: Using software bounce buffering for IO (SWIOTLB)
[Tue Feb  9 11:33:26 2021] software IO TLB: mapped [mem 0x7a8f6000-0x7e8f6000] (64MB)
[Tue Feb  9 11:33:26 2021] check: Scanning for low memory corruption every 60 seconds
[Tue Feb  9 11:33:26 2021] Initialise system trusted keyrings
[Tue Feb  9 11:33:26 2021] Key type blacklist registered
[Tue Feb  9 11:33:26 2021] workingset: timestamp_bits=36 max_order=23 bucket_order=0
[Tue Feb  9 11:33:26 2021] zbud: loaded
[Tue Feb  9 11:33:26 2021] squashfs: version 4.0 (2009/01/31) Phillip Lougher
[Tue Feb  9 11:33:26 2021] fuse: init (API version 7.31)
[Tue Feb  9 11:33:26 2021] *** VALIDATE fuse ***
[Tue Feb  9 11:33:26 2021] *** VALIDATE fuse ***
[Tue Feb  9 11:33:26 2021] Platform Keyring initialized
[Tue Feb  9 11:33:26 2021] Key type asymmetric registered
[Tue Feb  9 11:33:26 2021] Asymmetric key parser 'x509' registered
[Tue Feb  9 11:33:26 2021] Block layer SCSI generic (bsg) driver version 0.4 loaded (major 244)
[Tue Feb  9 11:33:26 2021] io scheduler mq-deadline registered
[Tue Feb  9 11:33:26 2021] pcieport 0000:00:1c.0: PME: Signaling with IRQ 122
[Tue Feb  9 11:33:26 2021] pcieport 0000:00:1c.5: PME: Signaling with IRQ 123
[Tue Feb  9 11:33:26 2021] shpchp: Standard Hot Plug PCI Controller Driver version: 0.4
[Tue Feb  9 11:33:26 2021] efifb: probing for efifb
[Tue Feb  9 11:33:26 2021] efifb: showing boot graphics
[Tue Feb  9 11:33:26 2021] efifb: framebuffer at 0x90000000, using 4128k, total 4128k
[Tue Feb  9 11:33:26 2021] efifb: mode is 1366x768x32, linelength=5504, pages=1
[Tue Feb  9 11:33:26 2021] efifb: scrolling: redraw
[Tue Feb  9 11:33:26 2021] efifb: Truecolor: size=8:8:8:8, shift=24:16:8:0
[Tue Feb  9 11:33:26 2021] fbcon: Deferring console take-over
[Tue Feb  9 11:33:26 2021] fb0: EFI VGA frame buffer device
[Tue Feb  9 11:33:26 2021] intel_idle: MWAIT substates: 0x11142120
[Tue Feb  9 11:33:26 2021] intel_idle: v0.4.1 model 0x4E
[Tue Feb  9 11:33:26 2021] intel_idle: lapic_timer_reliable_states 0xffffffff
[Tue Feb  9 11:33:26 2021] ACPI: AC Adapter [ADP0] (off-line)
[Tue Feb  9 11:33:26 2021] input: Lid Switch as /devices/LNXSYSTM:00/LNXSYBUS:00/PNP0C0D:00/input/input0
[Tue Feb  9 11:33:26 2021] ACPI: Lid Switch [LID0]
[Tue Feb  9 11:33:26 2021] input: Power Button as /devices/LNXSYSTM:00/LNXSYBUS:00/PNP0C0C:00/input/input1
[Tue Feb  9 11:33:26 2021] ACPI: Power Button [PWRB]
[Tue Feb  9 11:33:26 2021] Serial: 8250/16550 driver, 32 ports, IRQ sharing enabled
[Tue Feb  9 11:33:26 2021] Linux agpgart interface v0.103
[Tue Feb  9 11:33:26 2021] tpm_crb MSFT0101:00: [Firmware Bug]: Bad ACPI memory layout
[Tue Feb  9 11:33:26 2021] loop: module loaded
[Tue Feb  9 11:33:26 2021] libphy: Fixed MDIO Bus: probed
[Tue Feb  9 11:33:26 2021] tun: Universal TUN/TAP device driver, 1.6
[Tue Feb  9 11:33:26 2021] PPP generic driver version 2.4.2
[Tue Feb  9 11:33:26 2021] VFIO - User Level meta-driver version: 0.3
[Tue Feb  9 11:33:26 2021] ehci_hcd: USB 2.0 'Enhanced' Host Controller (EHCI) Driver
[Tue Feb  9 11:33:26 2021] ehci-pci: EHCI PCI platform driver
[Tue Feb  9 11:33:26 2021] ehci-platform: EHCI generic platform driver
[Tue Feb  9 11:33:26 2021] ohci_hcd: USB 1.1 'Open' Host Controller (OHCI) Driver
[Tue Feb  9 11:33:26 2021] ohci-pci: OHCI PCI platform driver
[Tue Feb  9 11:33:26 2021] ohci-platform: OHCI generic platform driver
[Tue Feb  9 11:33:26 2021] uhci_hcd: USB Universal Host Controller Interface driver
[Tue Feb  9 11:33:26 2021] xhci_hcd 0000:00:14.0: xHCI Host Controller
[Tue Feb  9 11:33:26 2021] xhci_hcd 0000:00:14.0: new USB bus registered, assigned bus number 1
[Tue Feb  9 11:33:26 2021] xhci_hcd 0000:00:14.0: hcc params 0x200077c1 hci version 0x100 quirks 0x0000000081109810
[Tue Feb  9 11:33:26 2021] xhci_hcd 0000:00:14.0: cache line size of 64 is not supported
[Tue Feb  9 11:33:26 2021] usb usb1: New USB device found, idVendor=1d6b, idProduct=0002, bcdDevice= 5.04
[Tue Feb  9 11:33:26 2021] usb usb1: New USB device strings: Mfr=3, Product=2, SerialNumber=1
[Tue Feb  9 11:33:26 2021] usb usb1: Product: xHCI Host Controller
[Tue Feb  9 11:33:26 2021] usb usb1: Manufacturer: Linux 5.4.0-65-generic xhci-hcd
[Tue Feb  9 11:33:26 2021] usb usb1: SerialNumber: 0000:00:14.0
[Tue Feb  9 11:33:26 2021] hub 1-0:1.0: USB hub found
[Tue Feb  9 11:33:26 2021] hub 1-0:1.0: 12 ports detected
[Tue Feb  9 11:33:26 2021] xhci_hcd 0000:00:14.0: xHCI Host Controller
[Tue Feb  9 11:33:26 2021] xhci_hcd 0000:00:14.0: new USB bus registered, assigned bus number 2
[Tue Feb  9 11:33:26 2021] xhci_hcd 0000:00:14.0: Host supports USB 3.0 SuperSpeed
[Tue Feb  9 11:33:26 2021] usb usb2: New USB device found, idVendor=1d6b, idProduct=0003, bcdDevice= 5.04
[Tue Feb  9 11:33:26 2021] usb usb2: New USB device strings: Mfr=3, Product=2, SerialNumber=1
[Tue Feb  9 11:33:26 2021] usb usb2: Product: xHCI Host Controller
[Tue Feb  9 11:33:26 2021] usb usb2: Manufacturer: Linux 5.4.0-65-generic xhci-hcd
[Tue Feb  9 11:33:26 2021] usb usb2: SerialNumber: 0000:00:14.0
[Tue Feb  9 11:33:26 2021] hub 2-0:1.0: USB hub found
[Tue Feb  9 11:33:26 2021] hub 2-0:1.0: 6 ports detected
[Tue Feb  9 11:33:26 2021] usb: port power management may be unreliable
[Tue Feb  9 11:33:26 2021] i8042: PNP: PS/2 Controller [PNP0303:PS2K] at 0x60,0x64 irq 1
[Tue Feb  9 11:33:26 2021] i8042: PNP: PS/2 appears to have AUX port disabled, if this is incorrect please boot with i8042.nopnp
[Tue Feb  9 11:33:26 2021] serio: i8042 KBD port at 0x60,0x64 irq 1
[Tue Feb  9 11:33:26 2021] mousedev: PS/2 mouse device common for all mice
[Tue Feb  9 11:33:26 2021] rtc_cmos 00:03: RTC can wake from S4
[Tue Feb  9 11:33:26 2021] rtc_cmos 00:03: registered as rtc0
[Tue Feb  9 11:33:26 2021] rtc_cmos 00:03: alarms up to one month, y3k, 242 bytes nvram, hpet irqs
[Tue Feb  9 11:33:26 2021] i2c /dev entries driver
[Tue Feb  9 11:33:26 2021] device-mapper: uevent: version 1.0.3
[Tue Feb  9 11:33:26 2021] device-mapper: ioctl: 4.41.0-ioctl (2019-09-16) initialised: dm-devel@redhat.com
[Tue Feb  9 11:33:26 2021] platform eisa.0: Probing EISA bus 0
[Tue Feb  9 11:33:26 2021] platform eisa.0: EISA: Cannot allocate resource for mainboard
[Tue Feb  9 11:33:26 2021] platform eisa.0: Cannot allocate resource for EISA slot 1
[Tue Feb  9 11:33:26 2021] platform eisa.0: Cannot allocate resource for EISA slot 2
[Tue Feb  9 11:33:26 2021] platform eisa.0: Cannot allocate resource for EISA slot 3
[Tue Feb  9 11:33:26 2021] platform eisa.0: Cannot allocate resource for EISA slot 4
[Tue Feb  9 11:33:26 2021] platform eisa.0: Cannot allocate resource for EISA slot 5
[Tue Feb  9 11:33:26 2021] platform eisa.0: Cannot allocate resource for EISA slot 6
[Tue Feb  9 11:33:26 2021] platform eisa.0: Cannot allocate resource for EISA slot 7
[Tue Feb  9 11:33:26 2021] platform eisa.0: Cannot allocate resource for EISA slot 8
[Tue Feb  9 11:33:26 2021] platform eisa.0: EISA: Detected 0 cards
[Tue Feb  9 11:33:26 2021] intel_pstate: Intel P-state driver initializing
[Tue Feb  9 11:33:26 2021] intel_pstate: HWP enabled
[Tue Feb  9 11:33:26 2021] ledtrig-cpu: registered to indicate activity on CPUs
[Tue Feb  9 11:33:26 2021] EFI Variables Facility v0.08 2004-May-17
[Tue Feb  9 11:33:26 2021] input: AT Translated Set 2 keyboard as /devices/platform/i8042/serio0/input/input2
[Tue Feb  9 11:33:26 2021] battery: ACPI: Battery Slot [BAT0] (battery present)
[Tue Feb  9 11:33:26 2021] intel_pmc_core intel_pmc_core.0:  initialized
[Tue Feb  9 11:33:26 2021] drop_monitor: Initializing network drop monitor service
[Tue Feb  9 11:33:26 2021] NET: Registered protocol family 10
[Tue Feb  9 11:33:26 2021] Segment Routing with IPv6
[Tue Feb  9 11:33:26 2021] NET: Registered protocol family 17
[Tue Feb  9 11:33:26 2021] Key type dns_resolver registered
[Tue Feb  9 11:33:26 2021] RAS: Correctable Errors collector initialized.
[Tue Feb  9 11:33:26 2021] microcode: sig=0x406e3, pf=0x40, revision=0xe2
[Tue Feb  9 11:33:26 2021] microcode: Microcode Update Driver: v2.2.
[Tue Feb  9 11:33:26 2021] IPI shorthand broadcast: enabled
[Tue Feb  9 11:33:26 2021] sched_clock: Marking stable (683418324, 575971)->(690570187, -6575892)
[Tue Feb  9 11:33:26 2021] registered taskstats version 1
[Tue Feb  9 11:33:26 2021] Loading compiled-in X.509 certificates
[Tue Feb  9 11:33:26 2021] Loaded X.509 cert 'Build time autogenerated kernel key: 254611168742f91faa32f8ad6a92fc903145caeb'
[Tue Feb  9 11:33:26 2021] zswap: loaded using pool lzo/zbud
[Tue Feb  9 11:33:26 2021] Key type ._fscrypt registered
[Tue Feb  9 11:33:26 2021] Key type .fscrypt registered
[Tue Feb  9 11:33:26 2021] Key type big_key registered
[Tue Feb  9 11:33:26 2021] Key type trusted registered
[Tue Feb  9 11:33:26 2021] Key type encrypted registered
[Tue Feb  9 11:33:26 2021] AppArmor: AppArmor sha1 policy hashing enabled
[Tue Feb  9 11:33:26 2021] integrity: Loading X.509 certificate: UEFI:db
[Tue Feb  9 11:33:26 2021] integrity: Loaded X.509 cert 'Microsoft Windows Production PCA 2011: a92902398e16c49778cd90f99e4f9ae17c55af53'
[Tue Feb  9 11:33:26 2021] integrity: Loading X.509 certificate: UEFI:db
[Tue Feb  9 11:33:26 2021] integrity: Loaded X.509 cert 'Microsoft Corporation UEFI CA 2011: 13adbf4309bd82709c8cd54f316ed522988a1bd4'
[Tue Feb  9 11:33:26 2021] integrity: Loading X.509 certificate: UEFI:db
[Tue Feb  9 11:33:26 2021] integrity: Loaded X.509 cert '4W320-14IKB: 8dcee6331cb26082429338acdf6278f7'
[Tue Feb  9 11:33:26 2021] integrity: Loading X.509 certificate: UEFI:db
[Tue Feb  9 11:33:26 2021] integrity: Problem loading X.509 certificate -65
[Tue Feb  9 11:33:26 2021] Error adding keys to platform keyring UEFI:db
[Tue Feb  9 11:33:26 2021] ima: Allocated hash algorithm: sha1
[Tue Feb  9 11:33:26 2021] ima: No architecture policies found
[Tue Feb  9 11:33:26 2021] evm: Initialising EVM extended attributes:
[Tue Feb  9 11:33:26 2021] evm: security.selinux
[Tue Feb  9 11:33:26 2021] evm: security.SMACK64
[Tue Feb  9 11:33:26 2021] evm: security.SMACK64EXEC
[Tue Feb  9 11:33:26 2021] evm: security.SMACK64TRANSMUTE
[Tue Feb  9 11:33:26 2021] evm: security.SMACK64MMAP
[Tue Feb  9 11:33:26 2021] evm: security.apparmor
[Tue Feb  9 11:33:26 2021] evm: security.ima
[Tue Feb  9 11:33:26 2021] evm: security.capability
[Tue Feb  9 11:33:26 2021] evm: HMAC attrs: 0x1
[Tue Feb  9 11:33:26 2021] PM:   Magic number: 9:695:596
[Tue Feb  9 11:33:26 2021] rtc_cmos 00:03: setting system clock to 2021-02-09T19:33:27 UTC (1612899207)
[Tue Feb  9 11:33:26 2021] Freeing unused decrypted memory: 2040K
[Tue Feb  9 11:33:26 2021] Freeing unused kernel image memory: 2724K
[Tue Feb  9 11:33:26 2021] Write protecting the kernel read-only data: 22528k
[Tue Feb  9 11:33:26 2021] Freeing unused kernel image memory: 2008K
[Tue Feb  9 11:33:26 2021] Freeing unused kernel image memory: 1184K
[Tue Feb  9 11:33:26 2021] x86/mm: Checked W+X mappings: passed, no W+X pages found.
[Tue Feb  9 11:33:26 2021] x86/mm: Checking user space page tables
[Tue Feb  9 11:33:26 2021] x86/mm: Checked W+X mappings: passed, no W+X pages found.
[Tue Feb  9 11:33:26 2021] Run /init as init process
[Tue Feb  9 11:33:26 2021] usb 1-7: new full-speed USB device number 2 using xhci_hcd
[Tue Feb  9 11:33:27 2021] i801_smbus 0000:00:1f.4: SPD Write Disable is set
[Tue Feb  9 11:33:27 2021] i801_smbus 0000:00:1f.4: SMBus using PCI interrupt
[Tue Feb  9 11:33:27 2021] ahci 0000:00:17.0: version 3.0
[Tue Feb  9 11:33:27 2021] hidraw: raw HID events driver (C) Jiri Kosina
[Tue Feb  9 11:33:27 2021] ahci 0000:00:17.0: AHCI 0001.0301 32 slots 2 ports 6 Gbps 0x3 impl RAID mode
[Tue Feb  9 11:33:27 2021] ahci 0000:00:17.0: flags: 64bit ncq pm led clo only pio slum part deso sadm sds apst 
[Tue Feb  9 11:33:27 2021] r8169 0000:01:00.0: can't disable ASPM; OS doesn't have ASPM control
[Tue Feb  9 11:33:27 2021] scsi host0: ahci
[Tue Feb  9 11:33:27 2021] scsi host1: ahci
[Tue Feb  9 11:33:27 2021] ata1: SATA max UDMA/133 abar m524288@0xa1300000 port 0xa1300100 irq 125
[Tue Feb  9 11:33:27 2021] ata2: SATA max UDMA/133 abar m524288@0xa1300000 port 0xa1300180 irq 125
[Tue Feb  9 11:33:27 2021] intel-lpss 0000:00:15.0: enabling device (0000 -> 0002)
[Tue Feb  9 11:33:27 2021] idma64 idma64.0: Found Intel integrated DMA 64-bit
[Tue Feb  9 11:33:27 2021] libphy: r8169: probed
[Tue Feb  9 11:33:27 2021] r8169 0000:01:00.0 eth0: RTL8168gu/8111gu, 8c:16:45:58:a1:ed, XID 509, IRQ 126
[Tue Feb  9 11:33:27 2021] r8169 0000:01:00.0 eth0: jumbo features [frames: 9200 bytes, tx checksumming: ko]
[Tue Feb  9 11:33:27 2021] i2c_hid i2c-SYNA2B33:00: i2c-SYNA2B33:00 supply vdd not found, using dummy regulator
[Tue Feb  9 11:33:27 2021] i2c_hid i2c-SYNA2B33:00: i2c-SYNA2B33:00 supply vddl not found, using dummy regulator
[Tue Feb  9 11:33:27 2021] r8169 0000:01:00.0 enp1s0: renamed from eth0
[Tue Feb  9 11:33:27 2021] usb 1-7: New USB device found, idVendor=0cf3, idProduct=e500, bcdDevice= 0.01
[Tue Feb  9 11:33:27 2021] usb 1-7: New USB device strings: Mfr=0, Product=0, SerialNumber=0
[Tue Feb  9 11:33:27 2021] usb 1-8: new high-speed USB device number 3 using xhci_hcd
[Tue Feb  9 11:33:27 2021] ata1: SATA link up 6.0 Gbps (SStatus 133 SControl 300)
[Tue Feb  9 11:33:27 2021] ata2: SATA link down (SStatus 4 SControl 300)
[Tue Feb  9 11:33:27 2021] usb 1-8: New USB device found, idVendor=04f2, idProduct=b5d8, bcdDevice=26.22
[Tue Feb  9 11:33:27 2021] usb 1-8: New USB device strings: Mfr=3, Product=1, SerialNumber=2
[Tue Feb  9 11:33:27 2021] usb 1-8: Product: EasyCamera
[Tue Feb  9 11:33:27 2021] usb 1-8: Manufacturer: Chicony Electronics Co.,Ltd.
[Tue Feb  9 11:33:27 2021] usb 1-8: SerialNumber: 0001
[Tue Feb  9 11:33:27 2021] tsc: Refined TSC clocksource calibration: 1991.999 MHz
[Tue Feb  9 11:33:27 2021] clocksource: tsc: mask: 0xffffffffffffffff max_cycles: 0x396d4ffc055, max_idle_ns: 881590662783 ns
[Tue Feb  9 11:33:27 2021] clocksource: Switched to clocksource tsc
[Tue Feb  9 11:33:28 2021] ata1.00: ATA-10: ST1000LM035-1RK172, LCM2, max UDMA/100
[Tue Feb  9 11:33:28 2021] ata1.00: 1953525168 sectors, multi 16: LBA48 NCQ (depth 32), AA
[Tue Feb  9 11:33:28 2021] ata1.00: configured for UDMA/100
[Tue Feb  9 11:33:28 2021] scsi 0:0:0:0: Direct-Access     ATA      ST1000LM035-1RK1 LCM2 PQ: 0 ANSI: 5
[Tue Feb  9 11:33:28 2021] sd 0:0:0:0: Attached scsi generic sg0 type 0
[Tue Feb  9 11:33:28 2021] sd 0:0:0:0: [sda] 1953525168 512-byte logical blocks: (1.00 TB/932 GiB)
[Tue Feb  9 11:33:28 2021] sd 0:0:0:0: [sda] 4096-byte physical blocks
[Tue Feb  9 11:33:28 2021] sd 0:0:0:0: [sda] Write Protect is off
[Tue Feb  9 11:33:28 2021] sd 0:0:0:0: [sda] Mode Sense: 00 3a 00 00
[Tue Feb  9 11:33:28 2021] sd 0:0:0:0: [sda] Write cache: enabled, read cache: enabled, doesn't support DPO or FUA
[Tue Feb  9 11:33:28 2021]  sda: sda1 sda2 sda3 sda4 sda5 sda6 sda7 sda8
[Tue Feb  9 11:33:28 2021] sd 0:0:0:0: [sda] Attached SCSI disk
[Tue Feb  9 11:33:28 2021] input: SYNA2B33:00 06CB:8251 Mouse as /devices/pci0000:00/0000:00:15.0/i2c_designware.0/i2c-1/i2c-SYNA2B33:00/0018:06CB:8251.0001/input/input3
[Tue Feb  9 11:33:28 2021] hid-generic 0018:06CB:8251.0001: input,hidraw0: I2C HID v1.00 Mouse [SYNA2B33:00 06CB:8251] on i2c-SYNA2B33:00
[Tue Feb  9 11:33:28 2021] mc: Linux media interface: v0.10
[Tue Feb  9 11:33:28 2021] videodev: Linux video capture interface: v2.00
[Tue Feb  9 11:33:28 2021] rmi4_f01 rmi4-00.fn01: found RMI device, manufacturer: Synaptics, product: TM3336-001, fw id: 2529025
[Tue Feb  9 11:33:28 2021] input: Synaptics TM3336-001 as /devices/pci0000:00/0000:00:15.0/i2c_designware.0/i2c-1/i2c-SYNA2B33:00/0018:06CB:8251.0001/input/input5
[Tue Feb  9 11:33:28 2021] hid-rmi 0018:06CB:8251.0001: input,hidraw0: I2C HID v1.00 Mouse [SYNA2B33:00 06CB:8251] on i2c-SYNA2B33:00
[Tue Feb  9 11:33:30 2021] fbcon: Taking over console
[Tue Feb  9 11:33:30 2021] Console: switching to colour frame buffer device 170x48
[Tue Feb  9 11:33:30 2021] EXT4-fs (sda6): mounted filesystem with ordered data mode. Opts: (null)
[Tue Feb  9 11:33:32 2021] systemd[1]: Inserted module 'autofs4'
[Tue Feb  9 11:33:32 2021] systemd[1]: systemd 245.4-4ubuntu3.2 running in system mode. (+PAM +AUDIT +SELINUX +IMA +APPARMOR +SMACK +SYSVINIT +UTMP +LIBCRYPTSETUP +GCRYPT +GNUTLS +ACL +XZ +LZ4 +SECCOMP +BLKID +ELFUTILS +KMOD +IDN2 -IDN +PCRE2 default-hierarchy=hybrid)
[Tue Feb  9 11:33:32 2021] systemd[1]: Detected architecture x86-64.
[Tue Feb  9 11:33:32 2021] systemd[1]: Set hostname to <ckadithya>.
[Tue Feb  9 11:33:34 2021] systemd[1]: /lib/systemd/system/dbus.socket:5: ListenStream= references a path below legacy directory /var/run/, updating /var/run/dbus/system_bus_socket → /run/dbus/system_bus_socket; please update the unit file accordingly.
[Tue Feb  9 11:33:35 2021] systemd[1]: /etc/systemd/system/teamviewerd.service:9: PIDFile= references a path below legacy directory /var/run/, updating /var/run/teamviewerd.pid → /run/teamviewerd.pid; please update the unit file accordingly.
[Tue Feb  9 11:33:35 2021] systemd[1]: /lib/systemd/system/docker.socket:6: ListenStream= references a path below legacy directory /var/run/, updating /var/run/docker.sock → /run/docker.sock; please update the unit file accordingly.
[Tue Feb  9 11:33:35 2021] systemd[1]: Created slice system-modprobe.slice.
[Tue Feb  9 11:33:35 2021] systemd[1]: Created slice system-systemd\x2dfsck.slice.
[Tue Feb  9 11:33:35 2021] systemd[1]: Created slice User and Session Slice.
[Tue Feb  9 11:33:35 2021] systemd[1]: Started Forward Password Requests to Wall Directory Watch.
[Tue Feb  9 11:33:35 2021] systemd[1]: Set up automount Arbitrary Executable File Formats File System Automount Point.
[Tue Feb  9 11:33:35 2021] systemd[1]: Reached target User and Group Name Lookups.
[Tue Feb  9 11:33:35 2021] systemd[1]: Reached target Remote File Systems.
[Tue Feb  9 11:33:35 2021] systemd[1]: Reached target Slices.
[Tue Feb  9 11:33:35 2021] systemd[1]: Listening on Syslog Socket.
[Tue Feb  9 11:33:35 2021] systemd[1]: Listening on fsck to fsckd communication Socket.
[Tue Feb  9 11:33:35 2021] systemd[1]: Listening on initctl Compatibility Named Pipe.
[Tue Feb  9 11:33:35 2021] systemd[1]: Listening on Journal Audit Socket.
[Tue Feb  9 11:33:35 2021] systemd[1]: Listening on Journal Socket (/dev/log).
[Tue Feb  9 11:33:35 2021] systemd[1]: Listening on Journal Socket.
[Tue Feb  9 11:33:35 2021] systemd[1]: Listening on udev Control Socket.
[Tue Feb  9 11:33:35 2021] systemd[1]: Listening on udev Kernel Socket.
[Tue Feb  9 11:33:35 2021] systemd[1]: Mounting Huge Pages File System...
[Tue Feb  9 11:33:35 2021] systemd[1]: Mounting POSIX Message Queue File System...
[Tue Feb  9 11:33:35 2021] systemd[1]: Mounting Kernel Debug File System...
[Tue Feb  9 11:33:35 2021] systemd[1]: Mounting Kernel Trace File System...
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Journal Service...
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Set the console keyboard layout...
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Create list of static device nodes for the current kernel...
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Load Kernel Module drm...
[Tue Feb  9 11:33:35 2021] systemd[1]: Condition check resulted in Set Up Additional Binary Formats being skipped.
[Tue Feb  9 11:33:35 2021] systemd[1]: Condition check resulted in File System Check on Root Device being skipped.
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Load Kernel Modules...
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Remount Root and Kernel File Systems...
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting udev Coldplug all Devices...
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Uncomplicated firewall...
[Tue Feb  9 11:33:35 2021] systemd[1]: Started Read required files in advance.
[Tue Feb  9 11:33:35 2021] systemd[1]: Mounted Huge Pages File System.
[Tue Feb  9 11:33:35 2021] systemd[1]: Mounted POSIX Message Queue File System.
[Tue Feb  9 11:33:35 2021] systemd[1]: Mounted Kernel Debug File System.
[Tue Feb  9 11:33:35 2021] systemd[1]: Mounted Kernel Trace File System.
[Tue Feb  9 11:33:35 2021] EXT4-fs (sda6): re-mounted. Opts: errors=remount-ro
[Tue Feb  9 11:33:35 2021] systemd[1]: Finished Remount Root and Kernel File Systems.
[Tue Feb  9 11:33:35 2021] systemd[1]: Condition check resulted in Rebuild Hardware Database being skipped.
[Tue Feb  9 11:33:35 2021] systemd[1]: Condition check resulted in Platform Persistent Storage Archival being skipped.
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Load/Save Random Seed...
[Tue Feb  9 11:33:35 2021] systemd[1]: Starting Create System Users...
[Tue Feb  9 11:33:35 2021] systemd[1]: Finished Uncomplicated firewall.
[Tue Feb  9 11:33:35 2021] systemd[1]: Finished Create list of static device nodes for the current kernel.
[Tue Feb  9 11:33:36 2021] lp: driver loaded but no devices found
[Tue Feb  9 11:33:36 2021] ppdev: user-space parallel port driver
[Tue Feb  9 11:33:36 2021] systemd[1]: Finished udev Coldplug all Devices.
[Tue Feb  9 11:33:36 2021] systemd[1]: modprobe@drm.service: Succeeded.
[Tue Feb  9 11:33:36 2021] systemd[1]: Finished Load Kernel Module drm.
[Tue Feb  9 11:33:36 2021] systemd[1]: Finished Load Kernel Modules.
[Tue Feb  9 11:33:36 2021] systemd[1]: Mounting FUSE Control File System...
[Tue Feb  9 11:33:36 2021] systemd[1]: Mounting Kernel Configuration File System...
[Tue Feb  9 11:33:36 2021] systemd[1]: Starting Apply Kernel Variables...
[Tue Feb  9 11:33:36 2021] systemd[1]: Mounted FUSE Control File System.
[Tue Feb  9 11:33:36 2021] systemd[1]: Mounted Kernel Configuration File System.
[Tue Feb  9 11:33:36 2021] systemd[1]: Finished Set the console keyboard layout.
[Tue Feb  9 11:33:36 2021] systemd[1]: Finished Apply Kernel Variables.
[Tue Feb  9 11:33:36 2021] systemd[1]: Started Journal Service.
[Tue Feb  9 11:33:36 2021] systemd-journald[268]: Received client request to flush runtime journal.
[Tue Feb  9 11:33:40 2021] input: Ideapad extra buttons as /devices/pci0000:00/0000:00:1f.0/PNP0C09:00/VPC2004:00/input/input6
[Tue Feb  9 11:33:41 2021] RAPL PMU: API unit is 2^-32 Joules, 5 fixed counters, 655360 ms ovfl timer
[Tue Feb  9 11:33:41 2021] RAPL PMU: hw unit of domain pp0-core 2^-14 Joules
[Tue Feb  9 11:33:41 2021] RAPL PMU: hw unit of domain package 2^-14 Joules
[Tue Feb  9 11:33:41 2021] RAPL PMU: hw unit of domain dram 2^-14 Joules
[Tue Feb  9 11:33:41 2021] RAPL PMU: hw unit of domain pp1-gpu 2^-14 Joules
[Tue Feb  9 11:33:41 2021] RAPL PMU: hw unit of domain psys 2^-14 Joules
[Tue Feb  9 11:33:41 2021] cryptd: max_cpu_qlen set to 1000
[Tue Feb  9 11:33:42 2021] AVX2 version of gcm_enc/dec engaged.
[Tue Feb  9 11:33:42 2021] AES CTR mode by8 optimization enabled
[Tue Feb  9 11:33:42 2021] mei_me 0000:00:16.0: enabling device (0000 -> 0002)
[Tue Feb  9 11:33:42 2021] cfg80211: Loading compiled-in X.509 certificates for regulatory database
[Tue Feb  9 11:33:42 2021] cfg80211: Loaded X.509 cert 'sforshee: 00b28ddf47aef9cea7'
[Tue Feb  9 11:33:42 2021] Bluetooth: Core ver 2.22
[Tue Feb  9 11:33:42 2021] NET: Registered protocol family 31
[Tue Feb  9 11:33:42 2021] Bluetooth: HCI device and connection manager initialized
[Tue Feb  9 11:33:42 2021] Bluetooth: HCI socket layer initialized
[Tue Feb  9 11:33:42 2021] Bluetooth: L2CAP socket layer initialized
[Tue Feb  9 11:33:42 2021] Bluetooth: SCO socket layer initialized
[Tue Feb  9 11:33:42 2021] usbcore: registered new interface driver btusb
[Tue Feb  9 11:33:42 2021] checking generic (90000000 408000) vs hw (90000000 10000000)
[Tue Feb  9 11:33:42 2021] fb0: switching to inteldrmfb from EFI VGA
[Tue Feb  9 11:33:42 2021] Console: switching to colour dummy device 80x25
[Tue Feb  9 11:33:42 2021] i915 0000:00:02.0: vgaarb: deactivate vga console
[Tue Feb  9 11:33:42 2021] [drm] Supports vblank timestamp caching Rev 2 (21.10.2013).
[Tue Feb  9 11:33:42 2021] [drm] Driver supports precise vblank timestamp query.
[Tue Feb  9 11:33:42 2021] ath10k_pci 0000:02:00.0: pci irq msi oper_irq_mode 2 irq_mode 0 reset_mode 0
[Tue Feb  9 11:33:42 2021] i915 0000:00:02.0: vgaarb: changed VGA decodes: olddecodes=io+mem,decodes=io+mem:owns=io+mem
[Tue Feb  9 11:33:42 2021] [drm] Initialized i915 1.6.0 20190822 for 0000:00:02.0 on minor 0
[Tue Feb  9 11:33:42 2021] ACPI: Video Device [GFX0] (multi-head: yes  rom: no  post: no)
[Tue Feb  9 11:33:42 2021] input: Video Bus as /devices/LNXSYSTM:00/LNXSYBUS:00/PNP0A08:00/LNXVIDEO:00/input/input7
[Tue Feb  9 11:33:42 2021] fbcon: i915drmfb (fb0) is primary device
[Tue Feb  9 11:33:42 2021] Console: switching to colour frame buffer device 170x48
[Tue Feb  9 11:33:42 2021] i915 0000:00:02.0: fb0: i915drmfb frame buffer device
[Tue Feb  9 11:33:42 2021] snd_hda_intel 0000:00:1f.3: bound 0000:00:02.0 (ops i915_audio_component_bind_ops [i915])
[Tue Feb  9 11:33:43 2021] snd_hda_codec_generic hdaudioC0D0: autoconfig for Generic: line_outs=1 (0x14/0x0/0x0/0x0/0x0) type:speaker
[Tue Feb  9 11:33:43 2021] snd_hda_codec_generic hdaudioC0D0:    speaker_outs=0 (0x0/0x0/0x0/0x0/0x0)
[Tue Feb  9 11:33:43 2021] snd_hda_codec_generic hdaudioC0D0:    hp_outs=1 (0x21/0x0/0x0/0x0/0x0)
[Tue Feb  9 11:33:43 2021] snd_hda_codec_generic hdaudioC0D0:    mono: mono_out=0x0
[Tue Feb  9 11:33:43 2021] snd_hda_codec_generic hdaudioC0D0:    inputs:
[Tue Feb  9 11:33:43 2021] snd_hda_codec_generic hdaudioC0D0:      Mic=0x19
[Tue Feb  9 11:33:43 2021] snd_hda_codec_generic hdaudioC0D0:      Internal Mic=0x12
[Tue Feb  9 11:33:43 2021] input: HDA Intel PCH Mic as /devices/pci0000:00/0000:00:1f.3/sound/card0/input8
[Tue Feb  9 11:33:43 2021] input: HDA Intel PCH Headphone as /devices/pci0000:00/0000:00:1f.3/sound/card0/input9
[Tue Feb  9 11:33:43 2021] input: HDA Intel PCH HDMI/DP,pcm=3 as /devices/pci0000:00/0000:00:1f.3/sound/card0/input10
[Tue Feb  9 11:33:43 2021] input: HDA Intel PCH HDMI/DP,pcm=7 as /devices/pci0000:00/0000:00:1f.3/sound/card0/input11
[Tue Feb  9 11:33:43 2021] input: HDA Intel PCH HDMI/DP,pcm=8 as /devices/pci0000:00/0000:00:1f.3/sound/card0/input12
[Tue Feb  9 11:33:43 2021] input: HDA Intel PCH HDMI/DP,pcm=9 as /devices/pci0000:00/0000:00:1f.3/sound/card0/input13
[Tue Feb  9 11:33:43 2021] input: HDA Intel PCH HDMI/DP,pcm=10 as /devices/pci0000:00/0000:00:1f.3/sound/card0/input14
[Tue Feb  9 11:33:44 2021] [drm] Finished loading DMC firmware i915/skl_dmc_ver1_27.bin (v1.27)
[Tue Feb  9 11:33:44 2021] ath10k_pci 0000:02:00.0: qca9377 hw1.1 target 0x05020001 chip_id 0x003821ff sub 17aa:0901
[Tue Feb  9 11:33:44 2021] ath10k_pci 0000:02:00.0: kconfig debug 0 debugfs 1 tracing 1 dfs 0 testmode 0
[Tue Feb  9 11:33:44 2021] ath10k_pci 0000:02:00.0: firmware ver WLAN.TF.2.1-00021-QCARMSWP-1 api 6 features wowlan,ignore-otp crc32 42e41877
[Tue Feb  9 11:33:44 2021] intel_rapl_common: Found RAPL domain package
[Tue Feb  9 11:33:44 2021] intel_rapl_common: Found RAPL domain core
[Tue Feb  9 11:33:44 2021] intel_rapl_common: Found RAPL domain uncore
[Tue Feb  9 11:33:44 2021] intel_rapl_common: Found RAPL domain dram
[Tue Feb  9 11:33:44 2021] ath10k_pci 0000:02:00.0: board_file api 2 bmi_id N/A crc32 8aedfa4a
[Tue Feb  9 11:33:44 2021] ath10k_pci 0000:02:00.0: unsupported HTC service id: 1536
[Tue Feb  9 11:33:44 2021] ath10k_pci 0000:02:00.0: htt-ver 3.56 wmi-op 4 htt-op 3 cal otp max-sta 32 raw 0 hwcrypto 1
[Tue Feb  9 11:33:44 2021] ath: EEPROM regdomain: 0x6c
[Tue Feb  9 11:33:44 2021] ath: EEPROM indicates we should expect a direct regpair map
[Tue Feb  9 11:33:44 2021] ath: Country alpha2 being used: 00
[Tue Feb  9 11:33:44 2021] ath: Regpair used: 0x6c
[Tue Feb  9 11:33:44 2021] ath10k_pci 0000:02:00.0 wlp2s0: renamed from wlan0
[Tue Feb  9 11:33:46 2021] uvcvideo: Found UVC 1.00 device EasyCamera (04f2:b5d8)
[Tue Feb  9 11:33:46 2021] input: EasyCamera: EasyCamera as /devices/pci0000:00/0000:00:14.0/usb1/1-8/1-8:1.0/input/input15
[Tue Feb  9 11:33:46 2021] usbcore: registered new interface driver uvcvideo
[Tue Feb  9 11:33:46 2021] USB Video Class driver (1.1.1)
[Tue Feb  9 11:33:50 2021] Adding 8000508k swap on /dev/sda7.  Priority:-2 extents:1 across:8000508k FS
[Tue Feb  9 11:33:54 2021] EXT4-fs (sda8): mounted filesystem with ordered data mode. Opts: (null)
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.782:2): apparmor="STATUS" operation="profile_load" profile="unconfined" name="lsb_release" pid=729 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.818:3): apparmor="STATUS" operation="profile_load" profile="unconfined" name="libreoffice-senddoc" pid=733 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.858:4): apparmor="STATUS" operation="profile_load" profile="unconfined" name="libreoffice-oopslash" pid=734 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.874:5): apparmor="STATUS" operation="profile_load" profile="unconfined" name="/usr/bin/man" pid=732 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.874:6): apparmor="STATUS" operation="profile_load" profile="unconfined" name="man_filter" pid=732 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.874:7): apparmor="STATUS" operation="profile_load" profile="unconfined" name="man_groff" pid=732 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.958:8): apparmor="STATUS" operation="profile_load" profile="unconfined" name="/usr/lib/NetworkManager/nm-dhcp-client.action" pid=730 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.962:9): apparmor="STATUS" operation="profile_load" profile="unconfined" name="/usr/lib/NetworkManager/nm-dhcp-helper" pid=730 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.962:10): apparmor="STATUS" operation="profile_load" profile="unconfined" name="/usr/lib/connman/scripts/dhclient-script" pid=730 comm="apparmor_parser"
[Tue Feb  9 11:33:55 2021] audit: type=1400 audit(1612899235.962:11): apparmor="STATUS" operation="profile_load" profile="unconfined" name="/{,usr/}sbin/dhclient" pid=730 comm="apparmor_parser"
[Tue Feb  9 11:34:18 2021] Bluetooth: BNEP (Ethernet Emulation) ver 1.3
[Tue Feb  9 11:34:18 2021] Bluetooth: BNEP filters: protocol multicast
[Tue Feb  9 11:34:18 2021] Bluetooth: BNEP socket layer initialized
[Tue Feb  9 11:34:18 2021] NET: Registered protocol family 38
[Tue Feb  9 11:34:19 2021] kauditd_printk_skb: 36 callbacks suppressed
[Tue Feb  9 11:34:19 2021] audit: type=1400 audit(1612899260.522:48): apparmor="DENIED" operation="capable" profile="/usr/sbin/cups-browsed" pid=908 comm="cups-browsed" capability=23  capname="sys_nice"
[Tue Feb  9 11:34:21 2021] Generic FE-GE Realtek PHY r8169-100:00: attached PHY driver [Generic FE-GE Realtek PHY] (mii_bus:phy_addr=r8169-100:00, irq=IGNORE)
[Tue Feb  9 11:34:21 2021] r8169 0000:01:00.0 enp1s0: Link is Down
[Tue Feb  9 11:34:21 2021] ath10k_pci 0000:02:00.0: unsupported HTC service id: 1536
[Tue Feb  9 11:34:24 2021] aufs 5.4.3-20200302
[Tue Feb  9 11:34:27 2021] wlp2s0: authenticate with dc:eb:69:a6:d3:ca
[Tue Feb  9 11:34:27 2021] wlp2s0: send auth to dc:eb:69:a6:d3:ca (try 1/3)
[Tue Feb  9 11:34:27 2021] wlp2s0: authenticated
[Tue Feb  9 11:34:27 2021] wlp2s0: associate with dc:eb:69:a6:d3:ca (try 1/3)
[Tue Feb  9 11:34:27 2021] wlp2s0: RX AssocResp from dc:eb:69:a6:d3:ca (capab=0x1511 status=0 aid=8)
[Tue Feb  9 11:34:27 2021] wlp2s0: associated
[Tue Feb  9 11:34:27 2021] ath: EEPROM regdomain: 0x8348
[Tue Feb  9 11:34:27 2021] ath: EEPROM indicates we should expect a country code
[Tue Feb  9 11:34:27 2021] ath: doing EEPROM country->regdmn map search
[Tue Feb  9 11:34:27 2021] ath: country maps to regdmn code: 0x3a
[Tue Feb  9 11:34:27 2021] ath: Country alpha2 being used: US
[Tue Feb  9 11:34:27 2021] ath: Regpair used: 0x3a
[Tue Feb  9 11:34:27 2021] ath: regdomain 0x8348 dynamically updated by country element
[Tue Feb  9 11:34:27 2021] IPv6: ADDRCONF(NETDEV_CHANGE): wlp2s0: link becomes ready
[Tue Feb  9 11:34:27 2021] wlp2s0: Limiting TX power to 30 (30 - 0) dBm as advertised by dc:eb:69:a6:d3:ca
[Tue Feb  9 11:34:33 2021] audit: type=1400 audit(1612899273.942:49): apparmor="STATUS" operation="profile_load" profile="unconfined" name="docker-default" pid=1317 comm="apparmor_parser"
[Tue Feb  9 11:34:35 2021] bridge: filtering via arp/ip/ip6tables is no longer available by default. Update your scripts to load br_netfilter if you need this.
[Tue Feb  9 11:34:35 2021] Bridge firewalling registered
[Tue Feb  9 11:34:35 2021] bpfilter: Loaded bpfilter_umh pid 1329
[Tue Feb  9 11:34:35 2021] Started bpfilter
[Tue Feb  9 11:34:35 2021] Initializing XFRM netlink socket
[Tue Feb  9 11:34:38 2021] rfkill: input handler disabled
[Tue Feb  9 11:34:54 2021] Bluetooth: RFCOMM TTY layer initialized
[Tue Feb  9 11:34:54 2021] Bluetooth: RFCOMM socket layer initialized
[Tue Feb  9 11:34:54 2021] Bluetooth: RFCOMM ver 1.11
[Tue Feb  9 11:34:54 2021] rfkill: input handler enabled
[Tue Feb  9 11:35:07 2021] rfkill: input handler disabled
[Tue Feb  9 12:04:42 2021] eudyptula_challenge: loading out-of-tree module taints kernel.
[Tue Feb  9 12:04:42 2021] eudyptula_challenge: module verification failed: signature and/or required key missing - tainting kernel
[Tue Feb  9 12:04:42 2021] Hello, World !
[Tue Feb  9 12:04:42 2021] Bye, World !
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# 
```
- The command `insmod` inserts the compiled module into the kernel. It is used to insert a module from any location and does not consider the dependancies.
- The command `rmmod` removes the compiled module from the kernel.
- You can see the `dmesg -T` show the message `Hello, World !` and `Bye, World !` with different time stamp value.
- This console output show that the kernel module successfully inserted and remove from the kernel

Module is allowed to build against any kernel source tree on the filesystem. As showed below
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# ls -lt /lib/modules/
total 16
drwxr-xr-x 5 root root 4096 Feb  8 22:58 5.4.0-65-generic
drwxr-xr-x 5 root root 4096 Feb  8 22:36 5.4.0-050400-generic
drwxr-xr-x 3 root root 4096 Jul 19  2020 5.8.0-rc4+
drwxr-xr-x 3 root root 4096 Jul 10  2020 5.7.7
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# export KDIR="/lib/modules/5.7.7/build"
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make all
make -C /lib/modules/5.7.7/build M=/home/adithya/Workspace/eudyptula_challenge/task01 modules
make[1]: Entering directory '/home/adithya/Workspace/linux_work/linux_stable'
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.o
  MODPOST 1 modules
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.mod.o
  LD [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.ko
make[1]: Leaving directory '/home/adithya/Workspace/linux_work/linux_stable'
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# modinfo eudyptula_challenge.ko 
filename:       /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.ko
version:        0.01
description:    Eudyptula Challenge Linux module.
author:         Adithya Chandrakasan
license:        GPL
srcversion:     54BBF3D35381C8DD3D9AE08
depends:        
retpoline:      Y
name:           eudyptula_challenge
vermagic:       5.7.7 SMP mod_unload
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# export KDIR="/lib/modules/5.8.0-rc4+/build"
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make all
make -C /lib/modules/5.8.0-rc4+/build M=/home/adithya/Workspace/eudyptula_challenge/task01 modules
make[1]: Entering directory '/home/adithya/Workspace/linux_work/linux_mainline'
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.o
  MODPOST /home/adithya/Workspace/eudyptula_challenge/task01/Module.symvers
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.mod.o
  LD [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.ko
make[1]: Leaving directory '/home/adithya/Workspace/linux_work/linux_mainline'
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# modinfo eudyptula_challenge.ko 
filename:       /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.ko
version:        0.01
description:    Eudyptula Challenge Linux module.
author:         Adithya Chandrakasan
license:        GPL
srcversion:     54BBF3D35381C8DD3D9AE08
depends:        
retpoline:      Y
name:           eudyptula_challenge
vermagic:       5.8.0-rc4+ SMP mod_unload
```

## Conclusion
This task helped me workout the skills I learnt from the training LFD103- A Beginners guide to linux kernel development. Through this task I have successfully created my first kernel module.
