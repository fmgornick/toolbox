#ifndef BASE_ARENA_H
#define BASE_ARENA_H

#define ARENA_DEFAULT_RESERVE_SIZE MB(64)
#define ARENA_DEFAULT_COMMIT_SIZE KB(64)
#define ARENA_HEADER_SIZE 128

typedef struct Arena {
    struct Arena *current;
    struct Arena *prev;
    U64 res_size;
    U64 cmt_size;
    U64 alignment;
    B32 chain;
    B32 zero;
    U64 base_pos;
    U64 pos;
    U64 cmt;
    U64 res;
} Arena;

typedef struct ArenaParams {
    U64 reserve_size;
    U64 commit_size;
    U64 alignment;
    B32 chain;
    B32 zero;
} ArenaParams;

internal Arena *arena_alloc();
internal Arena *arena_alloc_params(ArenaParams *params);
internal void arena_release(Arena *arena);

internal void *arena_push(Arena *arena, U64 size);
#define arena_push_array(a, T, c) arena_push((a), sizeof(T) * (c))

internal void arena_pop(Arena *arena, U64 size);
internal void arena_pop_to(Arena *arena, U64 pos);
#define arena_clear(a) arena_pop_to((a), 0)

internal void arena_align(Arena *arena, U64 pow2_align);
internal U64 arena_pos(Arena *arena);

#endif // BASE_ARENA_H
