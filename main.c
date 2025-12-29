#include "base/base_inc.h"
#include "os/os_inc.h"

#include "base/base_inc.c"
#include "os/os_inc.c"

#include <stdio.h>

S32
main()
{
    Arena *arena = arena_alloc();
    String8 str = str8_lit("blahfooblahbarblahbazblah");
    String8 substr = str8_lit("blah");
    String8List list = str8_split_substr(arena, str, substr);
    for (String8Node *node = list.first; node != 0; node = node->next)
    {
        // printf("%s: %llu\n", node->string.str, node->string.size);
        printf("%.*s\n", str8_varg(node->string));
    }
    return 0;
}
