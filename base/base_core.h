#ifndef BASE_CORE_H
#define BASE_CORE_H

#define internal static
#define global static
#define local_persist static

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

global const U32 bitmask1 = 0x00000001;
global const U32 bitmask2 = 0x00000003;
global const U32 bitmask3 = 0x00000007;
global const U32 bitmask4 = 0x0000000F;
global const U32 bitmask5 = 0x0000001F;
global const U32 bitmask6 = 0x0000003F;
global const U32 bitmask7 = 0x0000007F;
global const U32 bitmask8 = 0x000000FF;
global const U32 bitmask9 = 0x000001FF;
global const U32 bitmask10 = 0x000003FF;
global const U32 bitmask11 = 0x000007FF;
global const U32 bitmask12 = 0x00000FFF;
global const U32 bitmask13 = 0x00001FFF;
global const U32 bitmask14 = 0x00003FFF;
global const U32 bitmask15 = 0x00007FFF;
global const U32 bitmask16 = 0x0000FFFF;
global const U32 bitmask17 = 0x0001FFFF;
global const U32 bitmask18 = 0x0003FFFF;
global const U32 bitmask19 = 0x0007FFFF;
global const U32 bitmask20 = 0x000FFFFF;
global const U32 bitmask21 = 0x001FFFFF;
global const U32 bitmask22 = 0x003FFFFF;
global const U32 bitmask23 = 0x007FFFFF;
global const U32 bitmask24 = 0x00FFFFFF;
global const U32 bitmask25 = 0x01FFFFFF;
global const U32 bitmask26 = 0x03FFFFFF;
global const U32 bitmask27 = 0x07FFFFFF;
global const U32 bitmask28 = 0x0FFFFFFF;
global const U32 bitmask29 = 0x1FFFFFFF;
global const U32 bitmask30 = 0x3FFFFFFF;
global const U32 bitmask31 = 0x7FFFFFFF;
global const U32 bitmask32 = 0xFFFFFFFF;
global const U64 bitmask33 = 0x00000001FFFFFFFFull;
global const U64 bitmask34 = 0x00000003FFFFFFFFull;
global const U64 bitmask35 = 0x00000007FFFFFFFFull;
global const U64 bitmask36 = 0x0000000FFFFFFFFFull;
global const U64 bitmask37 = 0x0000001FFFFFFFFFull;
global const U64 bitmask38 = 0x0000003FFFFFFFFFull;
global const U64 bitmask39 = 0x0000007FFFFFFFFFull;
global const U64 bitmask40 = 0x000000FFFFFFFFFFull;
global const U64 bitmask41 = 0x000001FFFFFFFFFFull;
global const U64 bitmask42 = 0x000003FFFFFFFFFFull;
global const U64 bitmask43 = 0x000007FFFFFFFFFFull;
global const U64 bitmask44 = 0x00000FFFFFFFFFFFull;
global const U64 bitmask45 = 0x00001FFFFFFFFFFFull;
global const U64 bitmask46 = 0x00003FFFFFFFFFFFull;
global const U64 bitmask47 = 0x00007FFFFFFFFFFFull;
global const U64 bitmask48 = 0x0000FFFFFFFFFFFFull;
global const U64 bitmask49 = 0x0001FFFFFFFFFFFFull;
global const U64 bitmask50 = 0x0003FFFFFFFFFFFFull;
global const U64 bitmask51 = 0x0007FFFFFFFFFFFFull;
global const U64 bitmask52 = 0x000FFFFFFFFFFFFFull;
global const U64 bitmask53 = 0x001FFFFFFFFFFFFFull;
global const U64 bitmask54 = 0x003FFFFFFFFFFFFFull;
global const U64 bitmask55 = 0x007FFFFFFFFFFFFFull;
global const U64 bitmask56 = 0x00FFFFFFFFFFFFFFull;
global const U64 bitmask57 = 0x01FFFFFFFFFFFFFFull;
global const U64 bitmask58 = 0x03FFFFFFFFFFFFFFull;
global const U64 bitmask59 = 0x07FFFFFFFFFFFFFFull;
global const U64 bitmask60 = 0x0FFFFFFFFFFFFFFFull;
global const U64 bitmask61 = 0x1FFFFFFFFFFFFFFFull;
global const U64 bitmask62 = 0x3FFFFFFFFFFFFFFFull;
global const U64 bitmask63 = 0x7FFFFFFFFFFFFFFFull;
global const U64 bitmask64 = 0xFFFFFFFFFFFFFFFFull;

global const U32 bit1 = 0x00000001;
global const U32 bit2 = 0x00000002;
global const U32 bit3 = 0x00000004;
global const U32 bit4 = 0x00000008;
global const U32 bit5 = 0x00000010;
global const U32 bit6 = 0x00000020;
global const U32 bit7 = 0x00000040;
global const U32 bit8 = 0x00000080;
global const U32 bit9 = 0x00000100;
global const U32 bit10 = 0x00000200;
global const U32 bit11 = 0x00000400;
global const U32 bit12 = 0x00000800;
global const U32 bit13 = 0x00001000;
global const U32 bit14 = 0x00002000;
global const U32 bit15 = 0x00004000;
global const U32 bit16 = 0x00008000;
global const U32 bit17 = 0x00010000;
global const U32 bit18 = 0x00020000;
global const U32 bit19 = 0x00040000;
global const U32 bit20 = 0x00080000;
global const U32 bit21 = 0x00100000;
global const U32 bit22 = 0x00200000;
global const U32 bit23 = 0x00400000;
global const U32 bit24 = 0x00800000;
global const U32 bit25 = 0x01000000;
global const U32 bit26 = 0x02000000;
global const U32 bit27 = 0x04000000;
global const U32 bit28 = 0x08000000;
global const U32 bit29 = 0x10000000;
global const U32 bit30 = 0x20000000;
global const U32 bit31 = 0x40000000;
global const U32 bit32 = 0x80000000;
global const U64 bit33 = 0x0000000100000000ull;
global const U64 bit34 = 0x0000000200000000ull;
global const U64 bit35 = 0x0000000400000000ull;
global const U64 bit36 = 0x0000000800000000ull;
global const U64 bit37 = 0x0000001000000000ull;
global const U64 bit38 = 0x0000002000000000ull;
global const U64 bit39 = 0x0000004000000000ull;
global const U64 bit40 = 0x0000008000000000ull;
global const U64 bit41 = 0x0000010000000000ull;
global const U64 bit42 = 0x0000020000000000ull;
global const U64 bit43 = 0x0000040000000000ull;
global const U64 bit44 = 0x0000080000000000ull;
global const U64 bit45 = 0x0000100000000000ull;
global const U64 bit46 = 0x0000200000000000ull;
global const U64 bit47 = 0x0000400000000000ull;
global const U64 bit48 = 0x0000800000000000ull;
global const U64 bit49 = 0x0001000000000000ull;
global const U64 bit50 = 0x0002000000000000ull;
global const U64 bit51 = 0x0004000000000000ull;
global const U64 bit52 = 0x0008000000000000ull;
global const U64 bit53 = 0x0010000000000000ull;
global const U64 bit54 = 0x0020000000000000ull;
global const U64 bit55 = 0x0040000000000000ull;
global const U64 bit56 = 0x0080000000000000ull;
global const U64 bit57 = 0x0100000000000000ull;
global const U64 bit58 = 0x0200000000000000ull;
global const U64 bit59 = 0x0400000000000000ull;
global const U64 bit60 = 0x0800000000000000ull;
global const U64 bit61 = 0x1000000000000000ull;
global const U64 bit62 = 0x2000000000000000ull;
global const U64 bit63 = 0x4000000000000000ull;
global const U64 bit64 = 0x8000000000000000ull;

internal B32 memory_is_zero(void *ptr, U64 size);

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Clamp(a, x, b) (((x) < (a)) ? (a) : ((b) < (x)) ? (b) : (x))
#define ClampTop(a, b) Min(a, b)
#define ClampBot(a, b) Max(a, b)
#define Swap(T, a, b) Statement(T t__ = a; a = b; b = t__;)
#define Implies(a, b) ((!a) || (b))

#define IsPow2(n) ((n) != 0 && ((n) & ((n) - 1)) == 0)
#define IsPow2OrZero(n) (((n) & ((n) - 1)) == 0)
#define AlignUpPow2(n, p) (((n) + (p) - 1) & ~((p) - 1))
#define AlignDownPow2(n, p) ((n) & ~((p) - 1))

#define KB(n) ((U64)(n) << 10)
#define MB(n) ((U64)(n) << 20)
#define GB(n) ((U64)(n) << 30)
#define TB(n) ((U64)(n) << 40)
#define Thousand(n) ((n) * 1000)
#define Million(n) ((n) * 1000000)
#define Billion(n) ((n) * 1000000000)
#define Trillion(n) ((n) * 1000000000000)

#include <string.h>
#define MemoryCopy(dst, src, size) memmove((dst), (src), (size))
#define MemoryCopyStruct(dst, src) memmove((dst), (src), sizeof(*(dst)))
#define MemorySet(dst, byte, size) memset((dst), (byte), (size))
#define MemoryZero(dst, size) memset((dst), 0, (size))
#define MemoryZeroStruct(s) memset((s), 0, sizeof(*(s)))
#define MemoryCompare(a, b, size) memcmp((a), (b), (size))

#define MemoryIsZeroStruct(ptr) memory_is_zero((ptr), sizeof(*(ptr)))

#define InvalidPath Assert(!"Invalid Path!")
#define NotImplemented Assert(!"Not Implemented!")
#define NoOp ((void)0)

#endif // BASE_CORE_H
