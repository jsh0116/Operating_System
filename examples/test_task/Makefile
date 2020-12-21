obj-m += displaytask.o # module object name

KDIR := /lib/modules/$(shell uname -r)/build #kernel module directory
PWD := $(shell pwd) #cwd

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules #-C is change directory opt
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
