#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"

#include <stdio.h>

S32
main()
{
    Arena arena = arena_alloc();
    U32 size = 100000;
    U32 *arr = arena_push(&arena, size * sizeof(U32));
    for (U32 i = 0; i < 100000; i++)
    {
        arr[i] = i + 1;
    }

    for (U32 i = 0; i < 100000; i++)
    {
        printf("%u\n", arr[i]);
    }

    arena_release(&arena);
    return 0;
}
