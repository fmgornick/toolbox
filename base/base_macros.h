#ifndef BASE_MACROS_H
#define BASE_MACROS_H

#if COMPILER_MSVC
#  define DebugBreak() __debugbreak()
#elif COMPILER_CLANG || COMPILER_GCC
#  if ARCH_X64
#    define DebugBreak() __builtin_trap()
#  elif ARCH_ARM64
#    if OS_DARWIN
#      define DebugBreak() __builtin_debugtrap()
#    else
#      define DebugBreak() __asm__ volatile(".inst 0xd4200000")
#    endif
#  endif
#else
#  error unknown trap intrinsic for this compiler
#endif

// clang-format off
#define Statement(S) do { S } while (0)
// clang-format on
#if BUILD_DEBUG
#  define Assert(S) Statement(if (!(S)) { DebugBreak(); })
#else
#  define Assert(S)
#endif

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

#if LANG_CXX
#  define C_LINKAGE extern "C"
#else
#  define C_LINKAGE extern
#endif

#if COMPILER_CLANG || COMPILER_GCC
#  define threadvar __thread
#elif COMPILER_CL
#  define threadvar __declspec(thread)
#else
#  error threadvar not defined for this compiler
#endif

#if OS_WINDOWS
#  define shared_function C_LINKAGE __declspec(dllexport)
#else
#  define shared_function C_LINKAGE
#endif

#if COMPILER_CLANG
#  if defined(__has_feature)
#    if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#      define ASAN_ENABLED 1
#    endif
#  endif
#  define NO_ASAN __attribute__((no_sanitize("address")))
#elif COMPILER_MSVC
#  if defined(__SANITIZE_ADDRESS__)
#    define ASAN_ENABLED 1
#    define NO_ASAN __declspec(no_sanitize_address)
#  else
#    define NO_ASAN
#  endif
#endif

#if ASAN_ENABLED
C_LINKAGE void __asan_poison_memory_region(void const volatile *addr, size_t size);
C_LINKAGE void __asan_unpoison_memory_region(void const volatile *addr, size_t size);
#  define AsanPoison(addr, size) __asan_poison_memory_region((addr), (size))
#  define AsanUnpoison(addr, size) __asan_unpoison_memory_region((addr), (size))
#else
#  define AsanPoison(addr, size)
#  define AsanUnpoison(addr, size)
#endif

#define internal static
#define global static
#define local_persist static

#endif // BASE_MACROS_H
