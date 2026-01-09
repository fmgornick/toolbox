#ifndef TEST_STRING_C
#define TEST_STRING_C

#include <string.h>
#include <stdio.h>

internal void
print_bits(String8 str)
{
    U32 i, j;
    for (i = 0; i < str.size; i++)
    {
        for (j = 0; j < 8; j++)
        {
            if (str.str[i] & (1 << j)) fputc('1', stdout);
            else fputc('0', stdout);
        }
        fputc(' ', stdout);
    }
    fputc('\n', stdout);
}

internal void
test_string(Arena *arena)
{
    String8 str = str8_lit_comp("blahfooblahbarblahbazblah");
    String8 expected[3] = {
        str8_lit_comp("foo"),
        str8_lit_comp("bar"),
        str8_lit_comp("baz"),
    };

    {
        String8 substr;
        String8List actual_list;
        String8Array actual_array;
        U64 i;
        substr = str8_lit("blah");
        actual_list = str8_split_substr(arena, str, substr);
        actual_array = str8_array_from_list(arena, &actual_list);
        for (i = 0; i < 3; i++)
        {
            Assert(expected[i].size == actual_array.v[i].size);
            Assert(strncmp(expected[i].str, actual_array.v[i].str, expected[i].size) == 0);
        }
    }

    {
        String8 expected2 = str8_lit_comp("blahfooblahbarblahbazblah foo bar baz");
        String8 actual2 = str8_pushf(arena, "%.*s %.*s %.*s %.*s", str8_varg(str), str8_varg(expected[0]), str8_varg(expected[1]), str8_varg(expected[2]));
        Assert(expected2.size == actual2.size);
        Assert(strncmp(expected2.str, actual2.str, expected2.size) == 0);
    }

    {
        String8 in, out1, out2;
        String16 utf16;
        String32 utf32;
        in = str8_lit("\xf0\x9f\x98\x8a");
        utf32 = str32_from_8(arena, in);
        out1 = str8_from_32(arena, utf32);
        Assert(in.size == out1.size);
        Assert(strncmp(in.str, out1.str, in.size) == 0);

        utf16 = str16_from_8(arena, in);
        out2 = str8_from_16(arena, utf16);
        Assert(in.size == out2.size);
        Assert(strncmp(in.str, out2.str, in.size) == 0);
    }
}

#endif // TEST_STRING_C
