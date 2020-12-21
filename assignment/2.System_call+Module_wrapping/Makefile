# File Name       : Makefile
# Date            : 2020/09/26 ~ 2020/10/22
# OS              : Ubuntu 16.04.3 LTS
# Student Name    : Seung Hoon Jeong
# Student ID      : 2015707003
# Description     : Makefile for module

obj-m := my_module.o # module object name


KDIR := /lib/modules/$(shell uname -r)/build #kernel module directory
PWD := $(shell pwd) #cwd

all: test # Excutable file of test_add.c
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules #-C is change directory opt

test : test_add.c # Source code compile and store excutable file
	gcc -o test test_add.c

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f *.o test


