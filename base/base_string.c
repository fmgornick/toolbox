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
    B32 result = 0;
    if (1 <= base && base <= 16)
    {
    }
    return result;
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
    String8 result = { str, size };
    return result;
}

internal String8
str8_range(U8 *first, U8 *opl)
{
    String8 result = { first, (U64)(opl - first) };
    return result;
}

internal String8
str8_cstring(U8 *cstr)
{
    U8 *ptr = cstr;
    while (*ptr != '\0') ptr += 1;
    String8 result = str8_range(cstr, ptr);
    return result;
}

internal String8
str8_prefix(String8 str, U64 size)
{
    U64 size_clamped = ClampTop(size, str.size);
    String8 result = { str.str, size_clamped };
    return result;
}

internal String8
str8_postfix(String8 str, U64 size)
{
    U64 size_clamped = ClampTop(size, str.size);
    U64 offset = str.size - size_clamped;
    String8 result = { str.str + size_clamped, size_clamped };
    return result;
}

internal String8
str8_skip(String8 str, U64 amt)
{
    U64 amt_clamped = ClampTop(amt, str.size);
    String8 result = { str.str + amt_clamped, str.size - amt_clamped };
    return result;
}

internal String8
str8_chop(String8 str, U64 amt)
{
    U64 amt_clamped = ClampTop(amt, str.size);
    String8 result = { str.str, str.size - amt_clamped };
    return result;
}

internal String8
str8_substr_opl(String8 str, U64 first, U64 opl)
{
    U64 opl_clamped = ClampTop(opl, str.size);
    U64 first_clamped = ClampTop(first, opl_clamped);
    String8 result = { str.str + first_clamped, opl_clamped - first_clamped };
    return result;
}

internal String8
str8_substr_size(String8 str, U64 first, U64 size)
{
    U64 first_clamped = ClampTop(first, str.size);
    U64 size_clamped = ClampTop(size, str.size - first_clamped);
    String8 result = { str.str + first_clamped, size_clamped };
    return result;
}

internal String8List
str8_split_pattern(Arena *arena, String8 string, String8 pattern)
{
    String8List result = { 0 };
    U8 *ptr = string.str;
    U8 *split_first = ptr;
    U8 *opl = string.str + string.size;
    for (; ptr < opl; ptr += 1)
    {
        U8 byte = *ptr;
        B32 is_split_byte = 0;
        for (U64 i = 0; i < pattern.size; i++)
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
    String8List result = { 0 };
    U8 *ptr = string.str;
    U8 *split_first = ptr;
    U8 *opl = string.str + string.size;
    for (; ptr < opl - substr.size; ptr += 1)
    {
        B32 is_split_substr = 1;
        for (U64 i = 0; i < substr.size; i++)
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
    U8 *str = arena_push(arena, list->total_size);
    U8 *ptr = str;
    for (String8Node *node = list->first; node != 0; node = node->next)
    {
        MemoryCopy(ptr, node->string.str, node->string.size);
        ptr += node->string.size;
    }
    String8 result = { str, list->total_size };
    return result;
}

internal String8Array
str8_array_from_list(Arena *arena, String8List *list)
{
    String8Array array = { 0 };
    array.count = list->node_count;
    array.total_size = list->total_size;
    array.v = arena_push(arena, array.count * sizeof(String8));
    U64 idx = 0;
    for (String8Node *node = list->first; node != 0; node = node->next, idx += 1)
    {
        array.v[idx] = node->string;
    }
    return array;
}

internal String8
str8_pushf(Arena *arena, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    String8 result = str8_pushfv(arena, fmt, args);
    va_end(args);
    return result;
}

#include <stdio.h>
internal String8
str8_pushfv(Arena *arena, const char *fmt, va_list args)
{
    va_list args2;
    va_copy(args2, args);
    U64 buffer_size = 1024;
    U8 *buffer = arena_push(arena, buffer_size);
    U64 actual_size = vsnprintf((char *)buffer, buffer_size, fmt, args);
    if (actual_size < buffer_size)
    {
        arena_pop(arena, buffer_size - actual_size - 1);
    }
    else
    {
        arena_pop(arena, buffer_size);
        buffer = arena_push(arena, actual_size + 1);
        vsnprintf((char *)buffer, actual_size + 1, fmt, args);
    }
    va_end(args2);
    String8 result = str8(buffer, actual_size);
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
    va_start(args, fmt);
    String8 string = str8_pushf(arena, fmt, args);
    va_end(args);
    str8_list_push(arena, list, string);
}

#endif // BASE_STRING_C
