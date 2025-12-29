#ifndef BASE_ARENA_C
#define BASE_ARENA_C

internal Arena *
arena_alloc()
{
    ArenaParams params = { 0 };
    params.reserve_size = ARENA_DEFAULT_RESERVE_SIZE;
    params.commit_size = ARENA_DEFAULT_COMMIT_SIZE;
    params.alignment = sizeof(void *);
    params.chain = 1;
    params.zero = 1;
    Arena *result = arena_alloc_params(&params);
    return result;
}

internal Arena *
arena_alloc_params(ArenaParams *params)
{
    Arena *result = 0;
    if (params->reserve_size > params->commit_size)
    {
        U64 reserve_size_aligned = AlignUpPow2(params->reserve_size, KB(4));
        U64 commit_size_aligned = AlignUpPow2(params->commit_size, KB(4));
        void *memory = os_memory_reserve(reserve_size_aligned);
        if (os_memory_commit(memory, commit_size_aligned))
        {
            AsanPoison(memory, reserve_size_aligned);
            AsanUnpoison(memory, ARENA_HEADER_SIZE);
            result = (Arena *)memory;
            result->current = result;
            result->prev = 0;
            result->res_size = reserve_size_aligned;
            result->cmt_size = commit_size_aligned;
            result->alignment = sizeof(void *);
            result->chain = params->chain;
            result->zero = params->zero;
            result->base_pos = 0;
            result->pos = ARENA_HEADER_SIZE;
            result->res = reserve_size_aligned;
            result->cmt = commit_size_aligned;
        }
    }
    Assert(result != 0);
    return result;
}

internal void
arena_release(Arena *arena)
{
    Arena *current = arena->current;
    while (current != 0)
    {
        Arena *prev = current->prev;
        U64 reserve_size = current->res;
        AsanPoison(current, reserve_size);
        os_memory_release(current, reserve_size);
        current = prev;
    }
}

internal void *
arena_push(Arena *arena, U64 size)
{
    void *result = 0;
    Arena *current = arena->current;
    if (arena->chain)
    {
        U64 next_chunk_pos = AlignUpPow2(current->pos, arena->alignment);
        next_chunk_pos += size;
        if (next_chunk_pos > current->res)
        {
            U64 reserve_size = AlignUpPow2(arena->res_size, KB(4));
            U64 commit_size = AlignUpPow2(arena->cmt_size, KB(4));
            U64 enough_to_fit = ARENA_HEADER_SIZE + size;
            if (reserve_size < enough_to_fit)
            {
                reserve_size = AlignUpPow2(enough_to_fit, KB(4));
            }
            void *memory = os_memory_reserve(reserve_size);
            if (os_memory_commit(memory, commit_size))
            {
                AsanPoison(memory, new_reserve_size);
                AsanUnpoison(memory, ARENA_HEADER_SIZE);
                Arena *new_chunk = (Arena *)memory;
                new_chunk->current = new_chunk;
                new_chunk->prev = current;
                new_chunk->res_size = arena->res_size;
                new_chunk->cmt_size = arena->cmt_size;
                new_chunk->alignment = sizeof(void *);
                new_chunk->chain = 1;
                new_chunk->zero = current->zero;
                new_chunk->base_pos = current->base_pos + current->res;
                new_chunk->pos = ARENA_HEADER_SIZE;
                new_chunk->res = reserve_size;
                new_chunk->cmt = commit_size;
                arena->current = current = new_chunk;
            }
        }
    }

    U64 result_pos = AlignUpPow2(current->pos, arena->alignment);
    U64 next_chunk_pos = result_pos + size;
    if (next_chunk_pos <= current->res)
    {
        if (next_chunk_pos > current->cmt)
        {
            U64 next_commit_pos_aligned = next_chunk_pos + current->cmt_size - 1;
            next_commit_pos_aligned -= next_commit_pos_aligned % current->cmt_size;
            U64 next_commit_pos = ClampTop(next_commit_pos_aligned, current->res);
            U64 commit_size = next_commit_pos - current->cmt;
            if (os_memory_commit((U8 *)current + current->cmt, commit_size))
            {
                current->cmt = next_commit_pos;
            }
        }
        if (next_chunk_pos <= current->cmt)
        {
            AsanUnpoison((U8 *)current + current->pos, next_chunk_pos - current->pos);
            result = (U8 *)current + result_pos;
            current->pos = next_chunk_pos;
        }
    }
    Assert(result != 0);
    if (current->zero)
    {
        MemoryZero(result, size);
    }
    return result;
}

internal void
arena_pop(Arena *arena, U64 size)
{
    Arena *current = arena->current;
    U64 total_pos = current->base_pos + current->pos;
    if (size <= total_pos)
    {
        U64 new_pos = total_pos - size;
        arena_pop_to(arena, new_pos);
    }
}

internal void
arena_pop_to(Arena *arena, U64 pos)
{
    Arena *current = arena->current;
    U64 current_pos = current->base_pos + current->pos;
    if (pos < current_pos)
    {
        U64 total_pos_clamped = ClampBot(pos, ARENA_HEADER_SIZE);
        while (total_pos_clamped < current->base_pos)
        {
            Arena *prev = current->prev;
            U64 reserve_size = current->res;
            AsanPoison(current, reserve_size);
            os_memory_release(current, reserve_size);
            current = prev;
        }
        arena->current = current;
        U64 new_pos = total_pos_clamped - current->base_pos;
        U64 new_pos_clamped = ClampBot(new_pos, ARENA_HEADER_SIZE);
        AsanPoison((U8 *)current + new_pos_clamped, current->pos - new_pos_clamped);
        current->pos = new_pos_clamped;
    }
}

internal void
arena_align(Arena *arena, U64 pow2_align)
{
    Arena *current = arena->current;
    U64 pos = current->pos;
    U64 pos_aligned = AlignUpPow2(pos, pow2_align);
    U64 dif = pos_aligned - pos;
    if (dif > 0)
    {
        arena_push(arena, dif);
    }
}

internal U64
arena_pos(Arena *arena)
{
    Arena *current = arena->current;
    U64 result = current->base_pos + current->pos;
    return result;
}

#endif // BASE_ARENA_C
