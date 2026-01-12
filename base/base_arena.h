#ifndef BASE_ARENA_H
#define BASE_ARENA_H

#define ARENA_DEFAULT_RESERVE_SIZE MB(64)
#define ARENA_DEFAULT_COMMIT_SIZE KB(64)
#define ARENA_HEADER_SIZE 128

typedef struct ArenaParams {
    U64 reserve_size;
    U64 commit_size;
    U64 alignment;
    B32 chain;
    B32 zero;
} ArenaParams;

typedef struct Arena Arena;
struct Arena {
    Arena *current;
    Arena *prev;
    U64 res_size;
    U64 cmt_size;
    U64 alignment;
    B32 chain;
    B32 zero;
    U64 base_pos;
    U64 pos;
    U64 cmt;
    U64 res;
};

typedef struct Scratch {
    Arena *arena;
    U64 pos;
} Scratch;

typedef struct PoolNode PoolNode;
struct PoolNode {
    PoolNode *next;
};

typedef struct Pool {
    Arena *arena;
    PoolNode *free;
    U64 block_size;
    B32 zero;
} Pool;

/* creation & deletion ------------------------------------------------------ */
internal Arena *arena_alloc(void);
internal Arena *arena_alloc_params(ArenaParams *params);
internal void arena_release(Arena *arena);

/* push & pop core function ------------------------------------------------- */
internal void *arena_push(Arena *arena, U64 size);
internal void arena_pop(Arena *arena, U64 size);
internal void arena_pop_to(Arena *arena, U64 pos);

internal void arena_align(Arena *arena, U64 pow2_align);
internal U64 arena_pos(Arena *arena);

/* temporary scratch arenas ------------------------------------------------- */
internal Scratch scratch_push(Arena *arena);
internal void scratch_pop(Scratch scratch);

/* pool allocator creation & deletion --------------------------------------- */
internal Pool *pool_alloc(U64 block_size);
internal Pool *pool_alloc_params(U64 block_size, ArenaParams *params);
internal void pool_release(Pool *pool);

/* pool allocator push & pop core function ---------------------------------- */
internal void *pool_push(Pool *pool);
internal void pool_pop(Pool *pool, void *ptr);

/* helper macros ------------------------------------------------------------ */
#define arena_push_array(a, T, c) arena_push((a), sizeof(T) * (c))
#define arena_clear(a) arena_pop_to((a), 0)

#endif /* BASE_ARENA_H */
