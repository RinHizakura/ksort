MODULENAME := ksort
obj-m += $(MODULENAME).o
$(MODULENAME)-y += main.o

GIT_HOOKS := .git/hooks/applied

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: $(GIT_HOOKS)
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

insmod: all
	sudo insmod $(MODULENAME).ko

rmmod:
	sudo rmmod $(MODULENAME)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
