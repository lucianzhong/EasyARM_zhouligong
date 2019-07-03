# Makefile2.6
	ifneq ($(KERNELRELEASE),)
#kbuild syntax. dependency relationshsip of files and target modules are listed here.
obj-m := led.o
else
PWD:= $(shell pwd)
KVER = 2.6.35.3
KDIR := /home/zy/Desktop/linux-2.6.35.3
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions
endif