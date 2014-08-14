osdev
=====

This repository contains my operating system. Called "BT/OS", it's a 32-bit pre-emtive multitasking OS for Intel-compatible i686 (and possibly i586) processors.
It's not a UNIX-like system, but has some UNIX-like concepts (e.g. there is an equivelent of '/dev', but not a single-rooted filesystem).

Building should be fairly simple, as long as it's cloned to ~/Projects/os, running setup-binutils.sh and setup-gcc.sh will setup the compiler/linker/assembler, etc. Then "make" can build the OS. "make test" will run it in QEMU.