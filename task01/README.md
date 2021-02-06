
## Introduction
Writing a kernel module and a Makefile to print "Hello World!" to the kernel debug log level.
### Getting Started
I am using Ubuntu to do this task. I refered [this blog](https://blog.sourcerer.io/writing-a-simple-linux-kernel-module-d9dc3762c234) and https://tldp.org/LDP/lkmpg/2.6/html/c119.html  where I learnt to use the following commands and write the module and Makefile.
- To get started with the developing environment
`apt-get install build-essential linux-headers-`uname -r``
- It installs the essential tools required to buid and run the module.

```
mkdir -p eudyptula_challenge/task01
cd eudyptula_challenge/task01
```
-I used vim editor to write the following program.
```
cat eudyptula_challenge.c
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
  printk(KERN_INFO "Hello, World !\n");
  return 0;
}
static void __exit eudyptula_challenge_exit(void) {
  printk(KERN_INFO "Bye, World !\n");
}

module_init(eudyptula_challenge_init);
module_exit(eudyptula_challenge_exit);
```
#### Explanation of the code
- The include files are essential for including macros, kernel info and modules respectively in the code.
- The macro's license,author,description and version are added as a good coding practice. The code is licensed GPL so that people can be warned that the code is not open sourced. If the license macro is not included the following warning will be seen when we run make all.
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

- The init function for loading and the exit fuction for unloading the module to the kernel are defined as static and returning an int.
- The printk() is used to communicate infomation with the user, a logging mechanism for the kernel. They come with 8 priority log levels for which the kernel has macros. In our code here we use log level 6 KERN_INFO is used which is used for the informational message about the kernel actions performed.
- The function call for init and exit functions are given at the end.

cat Makefile

obj-m += eudyptula_challenge.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

test:
	sudo dmesg -C
	sudo insmod eudyptula_challenge.ko
	sudo rmmod eudyptula_challenge.ko
	sudo dmesg
##### Explanation of the Makefile
- The first line tells the kbuild that there is one object in that directory called eudyptula_challenge.o. The `-m` tells that it is a module.
- The target all builds the module and creates the objects as seen below when we run `make all`. The -c flag tells to change to the directory first.
```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make all
make -C /lib/modules/5.4.0-050400-generic/build M=/home/adithya/Workspace/eudyptula_challenge/task01 modules
make[1]: Entering directory '/usr/src/linux-headers-5.4.0-050400-generic'
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.mod.o
  LD [M]  /home/adithya/Workspace/eudyptula_challenge/task01/eudyptula_challenge.ko
make[1]: Leaving directory '/usr/src/linux-headers-5.4.0-050400-generic'

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
vermagic:       5.4.0-050400-generic SMP mod_unload
```
-The target clean tells to get rid of all the objects before created.
-When we run `make clean` it removes all the files generated in the modules directory. `Module.symvers` contains all the exported symbols from the kernel and compiled modules which gets cleaned now.

```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make clean
make -C /lib/modules/5.4.0-050400-generic/build M=/home/adithya/Workspace/eudyptula_challenge/task01 clean
make[1]: Entering directory '/usr/src/linux-headers-5.4.0-050400-generic'
  CLEAN   /home/adithya/Workspace/eudyptula_challenge/task01/Module.symvers
make[1]: Leaving directory '/usr/src/linux-headers-5.4.0-050400-generic'

```
-The target test is added to see the output of the kernel log without running separate commands.
-When we run the command `make test` we get the following.

```
# make test
sudo dmesg -C
sudo insmod eudyptula_challenge.ko
sudo rmmod eudyptula_challenge.ko
sudo dmesg
[33385.180106] eudyptula_challenge: loading out-of-tree module taints kernel.
[33385.180151] eudyptula_challenge: module verification failed: signature and/or required key missing - tainting kernel
[33385.180529] Hello, World !
[33385.189404] Bye, World !

```
-Here the message tells that the module taints the kernel. Inorder to avoid that message we should include the line `CONFIG_MODULE_SIG=n` at the beginning of the Makefile.
-The CONFIG_MODULE_SIG checks for a valid signature on the module.It is set to 'n' so that module with bad signature is accepted.
-Now when we make test we get the following

```
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01# make test
sudo dmesg -C
sudo insmod eudyptula_challenge.ko
sudo rmmod eudyptula_challenge.ko
sudo dmesg
[35322.871721] Hello, World !
[35322.882058] Bye, World !
root@ckadithya:/home/adithya/Workspace/eudyptula_challenge/task01#

```
-The command `dmesg -c` clears the kernel ring buffer.
-The command `insmod` inserts the compiled module into the kernel. It is used to insert a module from any location and does not consider the dependancies.
-The command `rmmod` removes the compiled module from the kernel.
-I used `dmesg -c` under the `test` target in the Makefile to clear all the dmesg from the sytem, and only get the dmesg when the eudyptula_challenge.ko module is inserted and removed from the kernel. You can see the `dmesg` show the message `Hello, World !` and `Bye, World !` with different time stamp value.
- This console output show that the kernel module successfully inserted and remove from the kernel
###### Conclusion
This task helped me workout the skills I learnt from the training LFD103- A Beginners guide to linux kernel development. Through this task I have successfully created my first kernel module.












