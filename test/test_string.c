#ifndef TEST_STRING_C
#define TEST_STRING_C

/* string assertion helpers ------------------------------------------------- */
internal B32 str8_equal(String8 expected, String8 actual);
internal B32 str16_equal(String16 expected, String16 actual);
internal B32 str32_equal(String32 expected, String32 actual);
internal B32 str8_array_equal(String8Array expected, String8Array actual);
/* -------------------------------------------------------------------------- */

internal void
test_string(void)
{
    Scratch scratch = scratch_begin(0, 0);

    {
        String8 input = str8_lit("word1 word2 word3");

        printf("str8_prefix ... ");
        Assert(str8_equal(str8_lit("word1"), str8_prefix(input, 5)));
        printf("passed\n");

        printf("str8_postfix ... ");
        Assert(str8_equal(str8_lit("word3"), str8_postfix(input, 5)));
        printf("passed\n");

        printf("str8_range ... ");
        Assert(str8_equal(str8_lit("word2"), str8_range(&input.str[6], &input.str[11])));
        printf("passed\n");

        printf("str8_skip ... ");
        Assert(str8_equal(str8_lit("word2 word3"), str8_skip(input, 6)));
        printf("passed\n");

        printf("str8_chop ... ");
        Assert(str8_equal(str8_lit("word1 word2"), str8_chop(input, 6)));
        printf("passed\n");

        printf("str8_substr_opl ... ");
        Assert(str8_equal(str8_lit("rd1 wo"), str8_substr_opl(input, 2, 8)));
        printf("passed\n");

        printf("str8_substr_size ... ");
        Assert(str8_equal(str8_lit("rd2 wo"), str8_substr_size(input, 8, 6)));
        printf("passed\n");
    }

    {
        String8 input = str8_lit("12foo4barfoo78foobar11foo1314foobar");
        String8 split = str8_lit("foobar");
        String8 expected_split_pattern[5] = {
            str8_lit_comp("12"),
            str8_lit_comp("4"),
            str8_lit_comp("78"),
            str8_lit_comp("11"),
            str8_lit_comp("1314"),
        };
        String8 expected_split_substr[2] = {
            str8_lit_comp("12foo4barfoo78"),
            str8_lit_comp("11foo1314"),
        };
        String8Array expected_split_pattern_array, expected_split_substr_array;
        String8List actual_split_pattern_list, actual_split_substr_list;
        String8Array actual_split_pattern_array, actual_split_substr_array;
        expected_split_pattern_array.count = 5;
        expected_split_pattern_array.v = expected_split_pattern;
        expected_split_substr_array.count = 2;
        expected_split_substr_array.v = expected_split_substr;

        printf("str8_split_pattern ... ");
        actual_split_pattern_list = str8_split_pattern(scratch.arena, input, split);
        actual_split_pattern_array = str8_array_from_list(scratch.arena, &actual_split_pattern_list);
        Assert(str8_array_equal(expected_split_pattern_array, actual_split_pattern_array));
        printf("passed\n");

        printf("str8_split_substr ... ");
        actual_split_substr_list = str8_split_substr(scratch.arena, input, split);
        actual_split_substr_array = str8_array_from_list(scratch.arena, &actual_split_substr_list);
        Assert(str8_array_equal(expected_split_substr_array, actual_split_substr_array));
        printf("passed\n");
    }

    {
        U8 utf8[] = {0x46, 0xc3, 0xa9, 0xe2, 0x82, 0xac, 0xf0, 0x9f, 0x98, 0x8e};
        U16 utf16[] = {0x0046, 0x00e9, 0x20ac, 0xd83d, 0xde0e};
        U32 utf32[] = {0x00000046, 0x000000e9, 0x000020ac, 0x0001f60e};
        String8 expected_utf8, actual_utf8;
        String16 expected_utf16, actual_utf16;
        String32 expected_utf32, actual_utf32;
        expected_utf8.str = utf8;
        expected_utf8.size = 10;
        expected_utf16.str = utf16;
        expected_utf16.size = 5;
        expected_utf32.str = utf32;
        expected_utf32.size = 4;

        printf("str8_from_16 ... ");
        actual_utf8 = str8_from_16(scratch.arena, expected_utf16);
        Assert(str8_equal(expected_utf8, actual_utf8));
        printf("passed\n");

        printf("str16_from_8 ... ");
        actual_utf16 = str16_from_8(scratch.arena, expected_utf8);
        Assert(str16_equal(expected_utf16, actual_utf16));
        printf("passed\n");

        printf("str8_from_32 ... ");
        actual_utf8 = str8_from_32(scratch.arena, expected_utf32);
        Assert(str8_equal(expected_utf8, actual_utf8));
        printf("passed\n");

        printf("str32_from_8 ... ");
        actual_utf32 = str32_from_8(scratch.arena, expected_utf8);
        Assert(str32_equal(expected_utf32, actual_utf32));
        printf("passed\n");
    }

    scratch_end(scratch);
}

/* assertion helper implementations ----------------------------------------- */

internal B32
str8_equal(String8 expected, String8 actual)
{
    B32 result = 0;
    if (expected.size == actual.size)
    {
        result = (strncmp((char *)expected.str, (char *)actual.str, expected.size) == 0);
    }
    return result;
}

internal B32
str16_equal(String16 expected, String16 actual)
{
    B32 result = 0;
    if (expected.size == actual.size)
    {
        result = (strncmp((char *)expected.str, (char *)actual.str, expected.size * 2) == 0);
    }
    return result;
}

internal B32
str32_equal(String32 expected, String32 actual)
{
    B32 result = 0;
    if (expected.size == actual.size)
    {
        result = (strncmp((char *)expected.str, (char *)actual.str, expected.size * 4) == 0);
    }
    return result;
}

internal B32
str8_array_equal(String8Array expected, String8Array actual)
{
    B32 result = 0;
    if (expected.count == actual.count)
    {
        U64 i;
        result = 1;
        for (i = 0; i < expected.count; i += 1)
        {
            if (!str8_equal(expected.v[i], actual.v[i]))
            {
                result = 0;
                break;
            }
        }
    }
    return result;
}

#endif /* TEST_STRING_C */
