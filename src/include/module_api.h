#ifndef _MODULE_API_H
#define	_MODULE_API_H

#ifndef KERNEL
#include <stddef.h>

typedef int (*syscall_vector)(int, void*);
extern syscall_vector syscall;
#endif

enum syscalls{
	SYS_PANIC 	=0x00,
	SYS_MALLOC 	=0x01,
	SYS_FREE 	=0x02,
	SYS_MEMSET	=0x03,
	SYS_MEMCPY	=0x04,
	SYS_MEMMOVE	=0x05,
	SYS_STRCMP	=0x06,
	SYS_STRNCPY	=0x07,
	SYS_INITLOCK	=0x08,
	SYS_TAKELOCK	=0x09,
	SYS_TRYLOCK	=0x0a,
	SYS_UNLOCK	=0x0b,
	SYS_DBGOUT	=0x0c,

	SYS_NEWTHREAD	=0x10,
	SYS_BLOCK	=0x11,
	SYS_YIELD	=0x12,
	SYS_THREADID	=0x13,
	SYS_PRIORITY	=0x14,
	SYS_ENDTHREAD	=0x15,
	SYS_UNBLOCK	=0x16,

	SYS_ADDDEVICE	=0x20,
	SYS_GETDEVICE	=0x21,
	SYS_FIRSTDEVICE	=0x22,
	SYS_NEXTDEVICE	=0x23,
	SYS_DEVOPEN	=0x24,
	SYS_DEVCLOSE	=0x25,
	SYS_DEVREAD	=0x26,
	SYS_DEVWRITE	=0x27,
	SYS_DEVSEEK	=0x28,
	SYS_DEVIOCTL	=0x29,
	SYS_DEVTYPE	=0x2a,
	SYS_DEVDESC	=0x2b,
	SYS_HANDLEINT	=0x2c,
	SYS_HANDLEIRQ	=0x2d,

	SYS_ADDFS	=0x30,
	SYS_FSMOUNT	=0x31,
	SYS_FSUNMOUNT	=0x32,
	SYS_FOPEN	=0x33,
	SYS_FCLOSE	=0x34,
	SYS_FREAD	=0x35,
	SYS_FWRITE	=0x36,
	SYS_FSEEK	=0x37,
	SYS_FIOCTL	=0x38,
	SYS_DIROPEN	=0x39,
	SYS_DIRCLOSE	=0x3a,
	SYS_DIRREAD	=0x3b,
	SYS_DIRSEEK	=0x3c,
	SYS_FSTAT	=0x3d,

	SYS_MODLOAD	=0x40,
	SYS_MODUNLOAD	=0x41,
	SYS_MODINFO	=0x42,
};

#endif
