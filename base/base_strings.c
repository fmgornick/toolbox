#ifndef BASE_STRING_C
#define BASE_STRING_C

internal B32
char_is_whitespace(U8 c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v');
}

internal B32
char_is_upper(U8 c)
{
    return ('A' <= c && c <= 'Z');
}

internal B32
char_is_lower(U8 c)
{
    return ('a' <= c && c <= 'z');
}

internal B32
char_is_alpha(U8 c)
{
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

internal B32
char_is_digit(U8 c, U32 base)
{
    NotImplemented;
}

internal U8
char_lower(U8 c)
{
    U8 new_c = c;
    if (char_is_upper(new_c))
    {
        new_c += ('a' - 'A');
    }
    return new_c;
}

internal U8
char_upper(U8 c)
{
    U8 new_c = c;
    if (char_is_lower(new_c))
    {
        new_c += ('A' - 'a');
    }
    return new_c;
}

internal String8
str8(U8 *str, U64 size)
{
    String8 result = {0};
    result.str = str;
    result.size = size;
    return result;
}

internal String8
str8_range(U8 *first, U8 *opl)
{
    String8 result = {0};
    result.str = first;
    result.size = (U64)(opl - first);
    return result;
}

internal String8
str8_cstring(U8 *cstr)
{
    U8 *ptr;
    String8 result;
    ptr = cstr;
    while (*ptr != '\0') ptr += 1;
    result = str8_range(cstr, ptr);
    return result;
}

internal String8
str8_prefix(String8 str, U64 size)
{
    String8 result = {0};
    U64 size_clamped = ClampTop(size, str.size);
    result.str = str.str;
    result.size = size_clamped;
    return result;
}

internal String8
str8_postfix(String8 str, U64 size)
{
    String8 result = {0};
    U64 size_clamped = ClampTop(size, str.size);
    U64 offset = str.size - size_clamped;
    result.str = str.str + size_clamped;
    result.size = size_clamped;
    return result;
}

internal String8
str8_skip(String8 str, U64 amt)
{
    String8 result = {0};
    U64 amt_clamped = ClampTop(amt, str.size);
    result.str = str.str + amt_clamped;
    result.size = str.size - amt_clamped;
    return result;
}

internal String8
str8_chop(String8 str, U64 amt)
{
    String8 result = {0};
    U64 amt_clamped = ClampTop(amt, str.size);
    result.str = str.str;
    result.size = str.size - amt_clamped;
    return result;
}

internal String8
str8_substr_opl(String8 str, U64 first, U64 opl)
{
    String8 result = {0};
    U64 opl_clamped = ClampTop(opl, str.size);
    U64 first_clamped = ClampTop(first, opl_clamped);
    result.str = str.str + first_clamped;
    result.size = opl_clamped - first_clamped;
    return result;
}

internal String8
str8_substr_size(String8 str, U64 first, U64 size)
{
    String8 result = {0};
    U64 first_clamped = ClampTop(first, str.size);
    U64 size_clamped = ClampTop(size, str.size - first_clamped);
    result.str = str.str + first_clamped;
    result.size = size_clamped;
    return result;
}

internal String8List
str8_split_pattern(Arena *arena, String8 string, String8 pattern)
{
    String8List result = {0};
    U8 *ptr = string.str;
    U8 *split_first = ptr;
    U8 *opl = string.str + string.size;
    for (; ptr < opl; ptr += 1)
    {
        U8 byte = *ptr;
        B32 is_split_byte = 0;
        U64 i;
        for (i = 0; i < pattern.size; i++)
        {
            if (byte == pattern.str[i])
            {
                is_split_byte = 1;
                break;
            }
        }
        if (is_split_byte)
        {
            if (split_first < ptr)
            {
                String8 substr = str8_range(split_first, ptr);
                str8_list_push(arena, &result, substr);
            }
            split_first = ptr + 1;
        }
    }
    if (split_first < ptr)
    {
        String8 substr = str8_range(split_first, ptr);
        str8_list_push(arena, &result, substr);
    }
    return result;
}

internal String8List
str8_split_substr(Arena *arena, String8 string, String8 substr)
{
    String8List result = {0};
    U8 *ptr = string.str;
    U8 *split_first = ptr;
    U8 *opl = string.str + string.size;
    for (; ptr < opl - substr.size; ptr += 1)
    {
        B32 is_split_substr = 1;
        U64 i;
        for (i = 0; i < substr.size; i++)
        {
            if (ptr[i] != substr.str[i])
            {
                is_split_substr = 0;
                break;
            }
        }
        if (is_split_substr)
        {
            if (split_first < ptr)
            {
                String8 substr = str8_range(split_first, ptr);
                str8_list_push(arena, &result, substr);
            }
            ptr = ptr + substr.size - 1;
            split_first = ptr + 1;
        }
    }
    if (split_first < ptr)
    {
        String8 substr = str8_range(split_first, ptr);
        str8_list_push(arena, &result, substr);
    }
    return result;
}

internal String8
str8_join(Arena *arena, String8List *list)
{
    String8 result = {0};
    U8 *str = arena_push(arena, list->total_size);
    U8 *ptr = str;
    String8Node *node;
    for (node = list->first; node != 0; node = node->next)
    {
        MemoryCopy(ptr, node->string.str, node->string.size);
        ptr += node->string.size;
    }
    result.str = str;
    result.size = list->total_size;
    return result;
}

internal String8Array
str8_array_from_list(Arena *arena, String8List *list)
{
    String8Array result = {0};
    U64 idx;
    String8Node *node;
    result.count = list->node_count;
    result.total_size = list->total_size;
    result.v = arena_push(arena, result.count * sizeof(String8));
    for (idx = 0, node = list->first; node != 0; node = node->next, idx += 1)
    {
        result.v[idx] = node->string;
    }
    return result;
}

internal String8
str8_pushf(Arena *arena, const char *fmt, ...)
{
    String8 result = {0};
    va_list args;
    U64 buffer_size;
    S64 actual_size;
    U8 *buffer;
    buffer_size = 1024;
    buffer = arena_push(arena, buffer_size);
    va_start(args, fmt);
    actual_size = vsnprintf((char *)buffer, buffer_size, fmt, args);
    va_end(args);
    if (actual_size < 0)
    {
        arena_pop(arena, buffer_size);
    }
    else if (actual_size < buffer_size)
    {
        arena_pop(arena, buffer_size - (U64)actual_size - 1);
        result.str = buffer;
        result.size = actual_size;
    }
    else
    {
        arena_pop(arena, buffer_size);
        buffer = arena_push(arena, (U64)actual_size + 1);
        va_start(args, fmt);
        actual_size = vsnprintf((char *)buffer, actual_size + 1, fmt, args);
        va_end(args);
        if (actual_size < 0)
        {
            arena_pop(arena, buffer_size);
        }
        else
        {
            result.str = buffer;
            result.size = actual_size;
        }
    }
    return result;
}

internal void
str8_list_push(Arena *arena, String8List *list, String8 string)
{
    String8Node *node = arena_push(arena, sizeof(String8Node));
    node->string = string;
    if (list->first == 0)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        list->last->next = node;
        list->last = node;
    }
    node->next = 0;
    list->node_count += 1;
    list->total_size += string.size;
}

internal void
str8_list_pushf(Arena *arena, String8List *list, const char *fmt, ...)
{
    va_list args;
    String8 string;
    va_start(args, fmt);
    string = str8_pushf(arena, fmt, args);
    va_end(args);
    str8_list_push(arena, list, string);
}

/* NOTE(fletcher): used for determining utf-8 sizes from first byte */
global U8 utf8_bytecount[32] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, /* 0XXXXXXX (1 byte)  */
    0, 0, 0, 0, 0, 0, 0, 0,                         /* 10XXXXXX (invalid) */
    2, 2, 2, 2,                                     /* 110XXXXX (2 bytes) */
    3, 3,                                           /* 1110XXXX (3 bytes) */
    4,                                              /* 11110XXX (4 bytes) */
    5,                                              /* 11111XXX (invalid) */
};

/* NOTE(fletcher): rfc2044 (section 2) */
internal UnicodeDecode
utf8_decode(U8 *str, U64 max)
{
    UnicodeDecode result = {max_U32, {1}};
    U8 bytecount = utf8_bytecount[str[0] >> 3];
    switch (bytecount)
    {
    case 1: {
        result.codepoint = str[0];
        result.representation.bytes = 1;
    }
    break;
    case 2: {
        if (max >= 2 && utf8_bytecount[str[1] >> 3] == 0)
        {
            result.codepoint = (str[0] & bitmask5) << 6;
            result.codepoint |= (str[1] & bitmask6);
            result.representation.bytes = 2;
        }
    }
    break;
    case 3: {
        if (max >= 3 &&                         //
            utf8_bytecount[str[1] >> 3] == 0 && //
            utf8_bytecount[str[2] >> 3] == 0)
        {
            result.codepoint = (str[0] & bitmask4) << 12;
            result.codepoint |= (str[1] & bitmask6) << 6;
            result.codepoint |= (str[2] & bitmask6);
            result.representation.bytes = 3;
        }
    }
    break;
    case 4: {
        if (max >= 4 &&                         //
            utf8_bytecount[str[1] >> 3] == 0 && //
            utf8_bytecount[str[2] >> 3] == 0 && //
            utf8_bytecount[str[3] >> 3] == 0)
        {
            result.codepoint = (str[0] & bitmask3) << 18;
            result.codepoint |= (str[1] & bitmask6) << 12;
            result.codepoint |= (str[2] & bitmask6) << 6;
            result.codepoint |= (str[3] & bitmask6);
            result.representation.bytes = 4;
        }
    }
    break;
    }
    return result;
}

/* NOTE(fletcher): rfc2781 (section 2.2) */
internal UnicodeDecode
utf16_decode(U16 *str, U64 max)
{
    UnicodeDecode result = {max_U32, {1}};
    result.codepoint = str[0];
    result.representation.words = 1;
    if (max >= 2 &&                             //
        0xD800 <= str[0] && str[0] <= 0xDBFF && //
        0xDC00 <= str[1] && str[1] <= 0xDFFF)
    {
        result.codepoint = ((str[0] & bitmask10) << 10);
        result.codepoint |= (str[1] & bitmask10);
        result.codepoint += 0x10000;
        result.representation.words = 2;
    }
    return result;
}

/* NOTE(fletcher): rfc2044 (section 2) */
internal U32
utf8_encode(U8 *str, U32 codepoint)
{
    U8 bytecount;
    if (codepoint <= bitmask7)
    { /* 1 byte  => 7 bits */
        str[0] = (U8)codepoint;
        bytecount = 1;
    }
    else if (codepoint <= bitmask11)
    { /* 2 bytes => 11 bits */
        str[0] = (bitmask2 << 6) | ((codepoint >> 6) & bitmask5);
        str[1] = (bit2 << 6) | (codepoint & bitmask6);
        bytecount = 2;
    }
    else if (codepoint <= bitmask16)
    { /* 3 bytes => 16 bits */
        str[0] = (bitmask3 << 5) | ((codepoint >> 12) & bitmask4);
        str[1] = (bitmask2 << 7) | ((codepoint >> 6) & bitmask6);
        str[2] = (bitmask2 << 7) | (codepoint & bitmask6);
        bytecount = 3;
    }
    else if (codepoint <= bitmask21)
    { /* 4 bytes => 21 bits */
        str[0] = (bitmask4 << 4) | ((codepoint >> 18) & bitmask3);
        str[1] = (bit1 << 7) | ((codepoint >> 12) & bitmask6);
        str[2] = (bit1 << 7) | ((codepoint >> 6) & bitmask6);
        str[3] = (bit1 << 7) | (codepoint & bitmask6);
        bytecount = 4;
    }
    else
    { /* anything else => invalid */
        str[0] = '?';
        bytecount = 1;
    }
    return bytecount;
}

/* NOTE(fletcher): rfc2781 (section 2.1) */
internal U32
utf16_encode(U16 *str, U32 codepoint)
{
    U32 wordcount;
    if (codepoint >= 0x110000)
    {
        str[0] = '?';
        wordcount = 1;
    }
    if (codepoint < 0x10000)
    {
        str[0] = (U16)codepoint;
        wordcount = 1;
    }
    else
    {
        U32 v = codepoint - 0x10000;
        str[0] = ((v >> 10) & bitmask10) + 0xD800;
        str[1] = (v & bitmask10) + 0xDC00;
        wordcount = 2;
    }
    return wordcount;
}

internal String8
str8_from_16(Arena *arena, String16 in)
{
    String8 result = {0};
    if (in.size > 0)
    {
        U64 cap = in.size * 3;
        U8 *str = arena_push(arena, sizeof(U8) * cap);
        U16 *ptr = in.str;
        U16 *opl = ptr + in.size;
        U64 size = 0;
        while (ptr < opl)
        {
            UnicodeDecode decoded = utf16_decode(ptr, opl - ptr);
            size += utf8_encode(str + size, decoded.codepoint);
            ptr += decoded.representation.words;
        }
        arena_pop(arena, sizeof(U8) * (cap - size));
        result.str = str;
        result.size = size;
    }
    return result;
}

internal String16
str16_from_8(Arena *arena, String8 in)
{
    String16 result = {0};
    if (in.size > 0)
    {
        U64 cap = in.size;
        U16 *str = arena_push(arena, sizeof(U16) * cap);
        U8 *ptr = in.str;
        U8 *opl = ptr + in.size;
        U8 size = 0;
        while (ptr < opl)
        {
            UnicodeDecode decoded = utf8_decode(ptr, opl - ptr);
            size += utf16_encode(str + size, decoded.codepoint);
            ptr += decoded.representation.bytes;
        }
        arena_pop(arena, sizeof(U16) * (cap - size));
        result.str = str;
        result.size = size;
    }
    return result;
}

internal String8
str8_from_32(Arena *arena, String32 in)
{
    String8 result = {0};
    if (in.size > 0)
    {
        U64 cap = in.size * 4;
        U8 *str = arena_push(arena, sizeof(U8) * cap);
        U32 *ptr = in.str;
        U32 *opl = ptr + in.size;
        U64 size = 0;
        while (ptr < opl)
        {
            size += utf8_encode(str + size, *ptr);
            ptr += 1;
        }
        arena_pop(arena, sizeof(U8) * (cap - size));
        result.str = str;
        result.size = size;
    }
    return result;
}

internal String32
str32_from_8(Arena *arena, String8 in)
{
    String32 result = {0};
    if (in.size > 0)
    {
        U64 cap = in.size;
        U32 *str = arena_push(arena, sizeof(U32) * cap);
        U8 *ptr = in.str;
        U8 *opl = in.str + in.size;
        U64 size = 0;
        while (ptr < opl)
        {
            UnicodeDecode decoded = utf8_decode(ptr, opl - ptr);
            str[size] = decoded.codepoint;
            ptr += decoded.representation.bytes;
            size += 1;
        }
        arena_pop(arena, sizeof(U32) * (cap - size));
        result.str = str;
        result.size = size;
    }
    return result;
}

#endif // BASE_STRING_C
