#ifndef BASE_ARENA_C
#define BASE_ARENA_C

internal Arena
arena_alloc()
{
    Arena result = { 0 };
    result.mem = (U8 *)os_memory_reserve(M_DEFAULT_RESERVE_SIZE);
    result.cap = M_DEFAULT_RESERVE_SIZE;
    os_memory_commit(result.mem, M_COMMIT_BLOCK_SIZE);
    result.commit_pos = M_COMMIT_BLOCK_SIZE;
    result.pos = 0;
    return result;
}

internal void
arena_release(Arena *arena)
{
    os_memory_release(arena->mem, arena->cap);
}

internal void *
arena_push(Arena *arena, U64 size)
{
    void *result = 0;
    if (arena->pos + size <= arena->cap)
    {
        result = arena->mem + arena->pos;
        arena->pos += size;
        if (arena->pos > arena->commit_pos)
        {
            U64 pos_aligned = AlignUpPow2(arena->pos, M_COMMIT_BLOCK_SIZE);
            U64 next_commit_pos = ClampTop(pos_aligned, arena->cap);
            U64 commit_size = next_commit_pos - arena->commit_pos;
            os_memory_commit(arena->mem + arena->commit_pos, commit_size);
            arena->commit_pos = next_commit_pos;
        }
    }
    return result;
}

internal void
arena_pop_to(Arena *arena, U64 pos)
{
    if (pos < arena->pos)
    {
        arena->pos = pos;
        // NOTE(fletcher): might want to decommit (idk)
        // U64 next_commit_pos = AlignUpPow2(arena->pos, M_COMMIT_BLOCK_SIZE);
        // if (next_commit_pos < arena->commit_pos)
        // {
        //     U64 decommit_size = arena->commit_pos - next_commit_pos;
        //     os_memory_decommit(arena->mem + arena->commit_pos, decommit_size);
        //     arena->commit_pos = next_commit_pos;
        // }
    }
}

internal void
arena_clear(Arena *arena)
{
    arena_pop_to(arena, 0);
}

#endif // BASE_ARENA_C
