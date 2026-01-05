#ifndef OS_INC_C
#define OS_INC_C

#if OS_LINUX
#  include "linux/linux_os_core.c"
#elif OS_WINDOWS
#  include "windows/windows_os_core.c"
#elif OS_DARWIN
#  include "darwin/darwin_os_core.c"
#else
#  error no backend for os_inc.c for this operating system
#endif

#endif // OS_INC_C
