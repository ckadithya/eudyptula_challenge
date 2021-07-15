obj-m := eudyptula_challenge_task06.o
CFLAGS_eudyptula_challenge_task06.o := -DDEBUG

KDIR  :=  /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=$(PWD) modules
install:
	make -C $(KDIR) M=$(PWD) modules_install
clean:
	make -C $(KDIR) M=$(PWD) clean
