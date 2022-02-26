MODULE_NAME :=netlink_kernel
obj-m :=$(MODULE_NAME).o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD)
	gcc netlink_user.c -o netlink_user

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf netlink_user
