#include "base/base_inc.h"
#include "os/os_inc.h"
#include "base/base_inc.c"
#include "os/os_inc.c"
#include <stdio.h>

S32
main()
{
    U32 *arr = os_memory_reserve(100000 * sizeof(U32));
    os_memory_commit(arr, 100000 * sizeof(U32));
    for (U32 i = 0; i < 100000; i++)
    {
        arr[i] = i + 1;
    }

    for (U32 i = 0; i < 100000; i++)
    {
        printf("%u\n", arr[i]);
    }

    os_memory_release(arr, 100000 * sizeof(U32));
    return 0;
}
