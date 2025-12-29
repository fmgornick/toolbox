#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"
#include "test/test_string.c"

S32
main()
{
    Arena *arena = arena_alloc();
    test_string(arena);
    return 0;
}
