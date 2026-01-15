#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"

#include "test/test_string.c"
/* #include "test/test_async.c" */

internal void
os_main(int argc, char **argv)
{
    /* OS_SystemInfo *sysinfo = os_system_info_get(); */
    /* OS_ProcessInfo *procinfo = os_process_info_get(); */
    /* printf("pid: %d\n", procinfo->pid); */
    /* printf("machine name: (%d) %s\n", str8_varg(sysinfo->machine_name)); */
    /* printf("binary path:  (%d) %s\n", str8_varg(procinfo->binary_path)); */
    /* printf("initial path: (%d) %s\n", str8_varg(procinfo->initial_path)); */
    test_string();
    /* test_async(); */
}
