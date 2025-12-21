#ifndef BASE_ARENA_H
#define BASE_ARENA_H

#define ARENA_DEFAULT_RESERVE_SIZE GB(1)
#define ARENA_DEFAULT_COMMIT_SIZE MB(64)
#define ARENA_HEADER_SIZE 128

typedef struct Arena {
    struct Arena *current;
    struct Arena *prev;
    U64 alignment;
    B32 chain;
    U64 base_pos;
    U64 pos;
    U64 cmt;
    U64 res;
} Arena;

internal Arena *arena_alloc();
internal Arena *arena_alloc_reserve(U64 reserve_size, B32 chain);
internal void arena_release(Arena *arena);
internal void *arena_push(Arena *arena, U64 size);
internal void *arena_push_no_zero(Arena *arena, U64 size);
internal void arena_pop_to(Arena *arena, U64 pos);
internal void arena_pop_amount(Arena *arena, U64 amount);
internal void arena_clear(Arena *arena);
internal void arena_align(Arena *arena, U64 pow2_align);
internal U64 arena_pos(Arena *arena);

#endif // BASE_ARENA_H
