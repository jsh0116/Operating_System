
obj-m := file_varea.o # module object name


KDIR := /lib/modules/$(shell uname -r)/build #kernel module directory
PWD := $(shell pwd) #cwd

all: 
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules #-C is change directory opt

test : test.c # Source code compile and store excutable file
	gcc -o test test.c

testwrapping : test.c
	gcc -o test test.c -DWRAPPING

clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
	rm -f *.o test
