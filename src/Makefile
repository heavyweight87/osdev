export PREFIX:=$(HOME)/Projects/os/cross
export TARGET:=i686-elf
export PATH:=$(PREFIX)/bin:$(PATH)

all: modules user kernel tarinit packages iso disk
clean: kernel-clean modules-clean user-clean disk-clean tarinit-clean packages-clean iso-clean
rebuild: clean all

kernel: dummy.file
	$(MAKE) -C kernel

kernel-clean: dummy.file
	$(MAKE) -C kernel clean

modules: dummy.file
	$(MAKE) -C modules

modules-clean: dummy.file
	$(MAKE) -C modules clean

user: dummy.file
	$(MAKE) -C user

user-clean:
	$(MAKE) -C user clean

test: dummy.file disk qemu-launch-opts.txt
	cd kernel && ./test.sh

testtxt: dummy.file disk
	cd kernel && ./testtxt.sh

disk: packages disk/btos.img

disk/btos.img: dummy.file
	make -C disk

disk-clean: dummy.file
	make -C disk clean

tarinit: dummy.file
	make -C tarinit

tarinit-clean: dummy.file
	make -C tarinit clean

iso: dummy.file
	make -C iso

iso-clean:
	make -C iso clean

packages: dummy.file kernel user modules
	make -C packages

packages-clean: dummy.file
	make -C packages clean

newlib: dummy.file
	make -C user/libs/btoscore all
	make -C user/libs/newlib all

newlib-clean: dummy.file
	make -C user/libs/newlib clean

dummy.file:

qemu-launch-opts.txt: | qemu-launch-opts.template
	cp qemu-launch-opts.template qemu-launch-opts.txt
	
