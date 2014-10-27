#ifndef _MODULE_API_H
#define	_MODULE_API_H

#ifndef KERNEL
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#endif

#include "drivers.h"
#include "fs_interface.h"
#include "locktype.h"

typedef void(*thread_func)(void*);
typedef uint64_t thread_id_t;
typedef uint64_t pid_t;

typedef char* (*info_function)();

struct kernel_extension;
#ifndef __cplusplus
typedef struct kernel_extension kernel_extension;
#endif

#define	ENV_Global 		(1<<0) //Use PID 0 (kernel) value instead
#define ENV_ReadOnly	(1<<1) //Not changeable by user-mode code
#define	ENV_Private 	(1<<2) //Not visible to user-mode code
#define ENV_NoInherit 	(1<<3), //Do not copy from parent to child

struct syscall_table{
	void (*panic)(char *msg);
	void *(*malloc)(size_t bytes);
	void (*free)(void *ptr);
	void *(*memset)(void* ptr, int value, size_t num);
	void (*memcpy)(void *dst, void *src, size_t size);
	void (*memmove)(void *dst, void *src, size_t size);
	int (*strcmp)(char *s1, char *s2);
	void (*strncpy)(char *dst, char *src, size_t num);

	void (*init_lock)(lock *l);
	void (*take_lock)(lock *l);
    void (*take_lock_recursive)(lock *l);
	bool (*try_take_lock)(lock *l);
	void (*release_lock)(lock *l);

	void (*dbgout)(const char *msg);
	void (*sprintf)(char *buf, const char *fmt, ...);

	thread_id_t (*new_thread)(thread_func entry, void *param);
	void (*block)();
	void (*yield)();
	thread_id_t (*thread_id)();
	void (*thread_priority)(uint32_t p_);
	void (*end_thread)();
	void (*unblock)(thread_id_t id);
	void (*thread_wait)(thread_id_t id);
	void (*thread_setblock)(bool(*blockcheck)(void*), void *param);
	void (*thread_abortable)(bool abortable);
	void (*thread_abort)(thread_id_t id);

	void (*add_device)(char *name, drv_driver *driver, void *id);
	drv_device *(*get_device)(const char *name);
	void *(*get_first_device)(char **name);
	void *(*get_next_device)(void *itr, char **name);
	void *(*devopen)(const char *name);
	bool (*devclose)(void *handle);
	size_t (*devread)(void *handle, size_t bytes, char *buffer);
	size_t (*devwrite)(void *handle, size_t bytes, char *buffer);
	size_t (*devseek)(void *handle, size_t pos, bool relative);
	int (*devioctl)(void *handle, int fn, size_t bytes, char *buffer);
	int (*devtype)(const char *name);
	char *(*devdesc)(const char *name);
	void (*handle_int)(size_t intno, int_handler handler);
	void (*handle_irq)(size_t irqno, int_handler handler);
	void (*mask_irq)(size_t irqno);
	void (*unmask_irq)(size_t irqno);
	void (*irq_ack)(size_t irq_no);

	void (*add_filesystem)(fs_driver *fs);
	bool (*mount)(const char *name, const char *device, const char *fs);
	bool (*unmount)(const char *name);

	file_handle *(*fopen)(const char *path, fs_mode_flags mode);
	bool (*fclose)(file_handle *handle);
	size_t (*fread)(file_handle *handle, size_t bytes, char *buf);
	size_t (*fwrite)(file_handle *handle, size_t bytes, char *buf);
	size_t (*fseek)(file_handle *handle, size_t pos, bool relative);
	int (*fioctl)(file_handle *handle, int fn, size_t bytes, char *buf);
    void (*fflush)(file_handle *handle);
	file_handle *(*fcreate)(const char *path);

	dir_handle *(*diropen)(const char *path, fs_mode_flags mode);
	bool (*dirclose)(dir_handle *handle);
	directory_entry (*dirread)(dir_handle *handle);
	bool (*dirwrite)(dir_handle *handle, directory_entry entry);
	bool (*dirseek)(dir_handle *handle, size_t pos, bool relative);
	dir_handle *(*dircreate)(const char *path);
	directory_entry (*stat)(const char *path);

	void (*module_load)(const char *path, char *params);

	void (*setenv)(const char *name, char *value, uint8_t flags, pid_t pid);
	char *(*getenv)(const char *name, pid_t pid);

	pid_t (*getpid)();
	pid_t (*spawn)(const char *exec, size_t argc, char **argv);
	void (*wait)(pid_t pid);
    void (*kill)(pid_t pid);

	void (*infofs_register)(const char *name, info_function fn);

    uint16_t (*add_extension)(kernel_extension *ext);
    kernel_extension *(*get_extension)(uint16_t ext);
    uint16_t (*get_extension_id)(const char *name);
};

#ifndef __cplusplus
typedef struct syscall_table syscall_table;
#endif

#ifndef KERNEL
extern syscall_table *SYSCALL_TABLE;
#endif

#endif
