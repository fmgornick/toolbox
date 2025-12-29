#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdint.h>
typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;
typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef S8 B8;
typedef S16 B16;
typedef S32 B32;
typedef S64 B64;
typedef float F32;
typedef double F64;

global const S8 max_S8 = (S8)0x7f;
global const S16 max_S16 = (S16)0x7fff;
global const S32 max_S32 = (S32)0x7fffffff;
global const S64 max_S64 = (S64)0x7fffffffffffffffll;
global const S8 min_S8 = (S8)0x80;
global const S16 min_S16 = (S16)0x8000;
global const S32 min_S32 = (S32)0x80000000;
global const S64 min_S64 = (S64)0x8000000000000000ll;
global const U8 max_U8 = 0xff;
global const U16 max_U16 = 0xffff;
global const U32 max_U32 = 0xffffffff;
global const U64 max_U64 = 0xffffffffffffffffull;

#endif // BASE_TYPES_H
