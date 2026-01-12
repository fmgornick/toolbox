#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"

#include "test/test_string.c"
#include "test/test_async.c"

internal void
os_main(int argc, char **argv)
{
    test_string();
    test_async();
}
