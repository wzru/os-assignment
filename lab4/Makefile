CONFIG_MODULE_SIG=n
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m := hello_kmod.o hello_kmod_param.o hello_driver.o bufio_driver.o

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

test: test_hello_driver.c
	gcc test_hello_driver.c -o test_hello_driver
	gcc test_bufio_driver.c -o test_bufio_driver

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order *~ .*.cmd *.mod