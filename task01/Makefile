CONFIG_MODULE_SIG=n
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
