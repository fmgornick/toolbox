#ifndef TEST_STRING_C
#define TEST_STRING_C

#include <string.h>

internal void
test_string(Arena *arena)
{
    String8 str = str8_lit("blahfooblahbarblahbazblah");
    String8 substr = str8_lit("blah");
    String8 expected1[3] = {
        str8_lit_comp("foo"),
        str8_lit_comp("bar"),
        str8_lit_comp("baz"),
    };
    String8List list = str8_split_substr(arena, str, substr);
    String8Array actual1 = str8_array_from_list(arena, &list);
    for (U64 i = 0; i < 3; i++)
    {
        Assert(expected1[i].size == actual1.v[i].size);
        Assert(strncmp(expected1[i].str, actual1.v[i].str, expected1[i].size) == 0);
    }

    String8 expected2 = str8_lit("blahfooblahbarblahbazblah foo bar baz");
    String8 actual2 = str8_pushf(arena, "%.*s %.*s %.*s %.*s", str8_varg(str), str8_varg(expected1[0]), str8_varg(expected1[1]), str8_varg(expected1[2]));
    Assert(expected2.size == actual2.size);
    Assert(strncmp(expected2.str, actual2.str, expected2.size) == 0);
}

#endif // TEST_STRING_C
