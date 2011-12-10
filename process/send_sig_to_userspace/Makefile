# Comment/uncomment the following line to disable/enable debugging
#DEBUG = y

# EXTRA_CFLAGS += -Werror -Wall

# Add your debugging flag (or not) to CFLAGS
ifeq ($(DEBUG),y)
  EXTRA_CFLAGS += -O -g -DDEBUG # "-O" is needed to expand inlines
else
  EXTRA_CFLAGS += -O2
endif

ifneq ($(KERNELRELEASE),)
# call from kernel build system
obj-m	:= send_sig.o

else

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
endif

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions

depend .depend dep:
	$(CC) $(CFLAGS) -M *.c > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
