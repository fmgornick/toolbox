#ifndef BASE_ARENA_H
#define BASE_ARENA_H

#define M_DEFAULT_RESERVE_SIZE GB(4)
#define M_COMMIT_BLOCK_SIZE MB(64)

// TODO(fletcher): linked list arena blocks
typedef struct Arena {
    U8 *mem;
    U64 cap;
    U64 pos;
    U64 commit_pos;
} Arena;

internal Arena arena_alloc();
internal void arena_release(Arena *arena);
internal void *arena_push(Arena *arena, U64 size);
internal void arena_pop_to(Arena *arena, U64 pos);
internal void arena_clear(Arena *arena);

#endif // BASE_ARENA_H
