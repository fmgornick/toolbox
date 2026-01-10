#ifndef OS_INC_H
#define OS_INC_H

#include "os_core.h"

#if OS_LINUX
#  include "linux/linux_os_core.h"
#elif OS_WINDOWS
#  include "windows/windows_os_core.h"
#elif OS_DARWIN
#  include "darwin/darwin_os_core.h"
#else
#  error no backend for os_inc.h for this operating system
#endif

#endif /* OS_INC_H */
