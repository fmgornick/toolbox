#ifndef BASE_STRING_H
#define BASE_STRING_H

#include <stdarg.h>
#include <stdio.h>

typedef struct String8 {
    U8 *str;
    U64 size;
} String8;

typedef struct String16 {
    U16 *str;
    U64 size;
} String16;

typedef struct String32 {
    U32 *str;
    U64 size;
} String32;

typedef struct String8Node String8Node;
struct String8Node {
    String8Node *next;
    String8 string;
};

typedef struct String8List {
    String8Node *first;
    String8Node *last;
    U64 node_count;
    U64 total_size;
} String8List;

typedef struct String8Array {
    String8 *v;
    U64 count;
    U64 total_size;
} String8Array;

typedef struct UnicodeDecode {
    U32 codepoint;
    union {
        U32 bytes;
        U32 words;
    } representation;
} UnicodeDecode;

internal B32 char_is_whitespace(U8 c);
internal B32 char_is_upper(U8 c);
internal B32 char_is_lower(U8 c);
internal B32 char_is_alpha(U8 c);
internal B32 char_is_digit(U8 c, U32 base);
internal U8 char_lower(U8 c);
internal U8 char_upper(U8 c);

#define str8_lit(s) str8((U8 *)(s), sizeof(s) - 1);
#define str8_lit_comp(s) {(U8 *)(s), sizeof(s) - 1}
#define str8_varg(s) (int)((s).size), ((s).str)

internal String8 str8(U8 *str, U64 size);
internal String8 str8_range(U8 *first, U8 *opl);
internal String8 str8_cstring(U8 *cstr);

internal String8 str8_prefix(String8 str, U64 size);
internal String8 str8_postfix(String8 str, U64 size);
internal String8 str8_skip(String8 str, U64 amt);
internal String8 str8_chop(String8 str, U64 amt);
internal String8 str8_substr_opl(String8 str, U64 first, U64 opl);
internal String8 str8_substr_size(String8 str, U64 first, U64 size);

internal String8List str8_split_pattern(Arena *arena, String8 str, String8 pattern);
internal String8List str8_split_substr(Arena *arena, String8 str, String8 substr);
internal String8 str8_join(Arena *arena, String8List *list);
internal String8Array str8_array_from_list(Arena *arena, String8List *list);

internal String8 str8_pushf(Arena *arena, const char *fmt, ...);
internal void str8_list_push(Arena *arena, String8List *list, String8 string);
internal void str8_list_pushf(Arena *arena, String8List *list, const char *fmt, ...);

internal UnicodeDecode utf8_decode(U8 *str, U64 max);
internal UnicodeDecode utf16_decode(U16 *str, U64 max);
internal U32 utf8_encode(U8 *str, U32 codepoint);
internal U32 utf16_encode(U16 *str, U32 codepoint);

internal String8 str8_from_16(Arena *arena, String16 in);
internal String16 str16_from_8(Arena *arena, String8 in);
internal String8 str8_from_32(Arena *arena, String32 in);
internal String32 str32_from_8(Arena *arena, String8 in);

#endif // BASE_STRING_H
