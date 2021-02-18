/*
 *  eudyptula_challenge.c - Write a Linux kernel module, and stand-alone
 * Makefile, that when loaded prints to the kernel debug log level,
 * Be sure to make the module able to be unloaded as well.
 */
#include <linux/init.h>   /* Needed for the macros */
#include <linux/kernel.h> /* Needed for KERN_INFO */
#include <linux/module.h> /* Needed by all modules */

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
