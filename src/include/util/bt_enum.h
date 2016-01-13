#ifndef _BT_ENUM_H
#define _BT_ENUM_H

#ifdef __cplusplus
#define ENUM_START(x) namespace x { enum Enum {
#define ENUM_END }; }
#define ENUM_GET(x, y) x::y
#define ENUM_SET(x, y, z) y = z
#define ENUM_TYPE(x)
#define ENUM_NAME(x) x::Enum
#define BT_STRUCT_TYPE(x)
#else
#define ENUM_START(x) enum x {
#define ENUM_END };
#define ENUM_GET(x, y) x ## _ ## y
#define ENUM_SET(x, y, z) x ## _ ## y = z
#define ENUM_TYPE(x) typedef enum x x
#define ENUM_NAME(x) x
#define BT_STRUCT_TYPE(x) typedef struct x x
#endif

#endif