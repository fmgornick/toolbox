#ifndef BASE_CONTEXT_H
#define BASE_CONTEXT_H

////////////////////////////////
// NOTE(fletcher): find compiler, os, and architecture

#if defined(__clang__)
#  define COMPILER_CLANG 1

#  if defined(__gnu_linux__) || defined(__linux__)
#    define OS_LINUX 1
#  elif defined(_WIN32) || defined(_WIN64)
#    define OS_WINDOWS 1
#  elif defined(__APPLE__) && defined(__MACH__)
#    define OS_DARWIN 1
#  else
#    error host operating system not found
#  endif

#  if defined(i386) || defined(__i386) || defined(__i386__)
#    define ARCH_X86 1
#  elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#    define ARCH_X64 1
#  elif defined(__arm__)
#    define ARCH_ARM 1
#  elif defined(__aarch64__)
#    define ARCH_ARM64 1
#  else
#    error architecture not found
#  endif

#elif defined(__GNUC__)
#  define COMPILER_GCC 1

#  if defined(__gnu_linux__) || defined(__linux__)
#    define OS_LINUX 1
#  elif defined(_WIN32) || defined(_WIN64)
#    define OS_WINDOWS 1
#  elif defined(__APPLE__) && defined(__MACH__)
#    define OS_DARWIN 1
#  else
#    error host operating system not found
#  endif

#  if defined(i386) || defined(__i386) || defined(__i386__)
#    define ARCH_X86 1
#  elif defined(__amd64__) || defined(__amd64) || defined(__x86_64__) || defined(__x86_64)
#    define ARCH_X64 1
#  elif defined(__arm__)
#    define ARCH_ARM 1
#  elif defined(__aarch64__)
#    define ARCH_ARM64 1
#  else
#    error architecture not found
#  endif

#elif defined(_MSC_VER)
#  define COMPILER_CL 1

#  if defined(_WIN32) || defined(_WIN64)
#    define OS_WINDOWS 1
#  else
#    error host operating system not found
#  endif

#  if defined(_M_IX86)
#    define ARCH_X86 1
#  elif defined(_M_X64) || defined(_M_AMD64)
#    define ARCH_X64 1
#  elif defined(_M_ARM)
#    define ARCH_ARM 1
#  elif defined(_M_ARM64)
#    define ARCH_ARM64 1
#  else
#    error architecture not found
#  endif

#else
#  error compiler not found
#endif

////////////////////////////////
// NOTE(fletcher): set undefined preprocessor macros to 0

#if !defined(BUILD_DEBUG)
#  define BUILD_DEBUG 0
#endif
#if !defined(BUILD_RELEASE)
#  define BUILD_RELEASE 0
#endif
#if !defined(COMPILER_CLANG)
#  define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_GCC)
#  define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CL)
#  define COMPILER_CL 0
#endif
#if !defined(OS_LINUX)
#  define OS_LINUX 0
#endif
#if !defined(OS_WINDOWS)
#  define OS_WINDOWS 0
#endif
#if !defined(OS_DARWIN)
#  define OS_DARWIN 0
#endif
#if !defined(ARCH_X86)
#  define ARCH_X86 0
#endif
#if !defined(ARCH_X64)
#  define ARCH_X64 0
#endif
#if !defined(ARCH_ARM)
#  define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
#  define ARCH_ARM64 0
#endif

#if defined(__cplusplus)
#  define LANG_CPP 1
#else
#  define LANG_C 1
#endif

#if !defined(LANG_C)
#  define LANG_C 0
#endif
#if !defined(LANG_CPP)
#  define LANG_CPP 0
#endif

#if ARCH_X64 || ARCH_ARM64
#  define ARCH_ADDRSIZE 64
#else
#  define ARCH_ADDRSIZE 32
#endif

////////////////////////////////
// NOTE(fletcher): helper context keywords

#if LANG_CXX
#  define C_LINKAGE extern "C"
#else
#  define C_LINKAGE extern
#endif

#if OS_WINDOWS
#  define shared_function C_LINKAGE __declspec(dllexport)
#else
#  define shared_function C_LINKAGE
#endif

#if COMPILER_CLANG || COMPILER_GCC
#  define threadvar __thread
#elif COMPILER_CL
#  define threadvar __declspec(thread)
#else
#  error threadvar not defined for this compiler
#endif

////////////////////////////////
// NOTE(fletcher): debug trap signal context helper

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

////////////////////////////////
// NOTE(fletcher): assert context helper

// clang-format off
#define Statement(s) do { s } while (0)
// clang-format on
#if BUILD_DEBUG
#  define Assert(s) Statement(if (!(s)) { DebugBreak(); })
#elif BUILD_RELEASE
#  define Assert(s) (void)(s)
#else
#  define Assert(s) Statement(if (!(s)) { DebugBreak(); })
#endif

////////////////////////////////
// NOTE(fletcher): address sanitization context helper

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

////////////////////////////////
// NOTE(fletcher): c89 inline processor directive workaround

// TODO(fletcher): uncomment when ported to c89
#if 0
#  if COMPILER_CLANG || COMPILER_GCC
#    define inline __inline__
#  elif COMPILER_CL
#    define inline __inline
#  else
#    define inline
#  endif
#endif

#endif // BASE_CONTEXT_H
