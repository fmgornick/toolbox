#ifndef TEST_STRING_C
#define TEST_STRING_C

#include <string.h>

internal void
test_string(Arena *arena)
{
    String8 str = str8_lit("blahfooblahbarblahbazblah");
    String8 substr = str8_lit("blah");
    U64 num_expected = 3;
    String8 expected[3] = {
        str8_lit_comp("foo"),
        str8_lit_comp("bar"),
        str8_lit_comp("baz"),
    };
    String8List list = str8_split_substr(arena, str, substr);
    String8Array actual = str8_array_from_list(arena, &list);
    for (U64 i = 0; i < num_expected; i++)
    {
        Assert(expected[i].size == actual.v[i].size);
        Assert(strncmp(expected[i].str, actual.v[i].str, expected[i].size) == 0);
    }
}

#endif // TEST_STRING_C
