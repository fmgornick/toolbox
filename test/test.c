#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"

#include "test/test_string.c"

internal void
entry_point(int argc, char **argv)
{
    test_string();
}
