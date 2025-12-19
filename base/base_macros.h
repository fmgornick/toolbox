#ifndef BASE_MACROS_H
#define BASE_MACROS_H
// clang-format off
#define Statement(s) do { s } while (0)
// clang-format on
#define AssertBreak() (*(volatile int *)0 = 0)

#if BUILD_DEBUG
#  define Assert(s) Statement(if (!(s)) { AssertBreak(); })
#else
#  define Assert(s)
#endif

#define Min(a, b) (((a) < (b)) ? (a) : (b))
#define Max(a, b) (((a) > (b)) ? (a) : (b))
#define Clamp(a, x, b) (((x) < (a)) ? (a) : ((b) < (x)) ? (b) : (x))
#define ClampTop(a, b) Min(a, b)
#define ClampBot(a, b) Max(a, b)
#define Swap(T, a, b) Statement(T t__ = a; a = b; b = t__;)
#define Implies(a, b) ((!a) || (b))

#define AlignUpPow2(n, p) (((n) + (p) - 1) & ~((p) - 1))
#define AlignDownPow2(n, p) ((n) & ~((p) - 1))
#define IsPow2(n) ((n) != 0 && ((n) & ((n) - 1)) == 0)
#define IsPow2OrZero(n) (((n) & ((n) - 1)) == 0)

#define KB(n) ((U64)(n) << 10)
#define MB(n) ((U64)(n) << 20)
#define GB(n) ((U64)(n) << 30)
#define TB(n) ((U64)(n) << 40)
#define Thousand(n) ((n) * 1000)
#define Million(n) ((n) * 1000000)
#define Billion(n) ((n) * 1000000000)
#define Trillion(n) ((n) * 1000000000000)

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

#define internal static
#define global static
#define local_persist static

#endif // BASE_MACROS_H
