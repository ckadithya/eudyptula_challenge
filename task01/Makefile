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
