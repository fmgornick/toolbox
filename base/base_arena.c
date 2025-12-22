#ifndef BASE_ARENA_C
#define BASE_ARENA_C

internal Arena *
arena_alloc()
{
    Arena *result = arena_alloc_reserve(ARENA_DEFAULT_RESERVE_SIZE, 1);
    return result;
}

internal Arena *
arena_alloc_reserve(U64 reserve_size, B32 chain)
{
    Arena *result = 0;
    if (reserve_size > ARENA_DEFAULT_COMMIT_SIZE)
    {
        U64 reserve_size_aligned = AlignUpPow2(reserve_size, KB(4));
        void *memory = os_memory_reserve(reserve_size_aligned);
        if (os_memory_commit(memory, ARENA_DEFAULT_COMMIT_SIZE))
        {
            AsanPoison(memory, reserve_size_aligned);
            AsanUnpoison(memory, ARENA_HEADER_SIZE);
            result = (Arena *)memory;
            result->current = result;
            result->prev = 0;
            result->alignment = sizeof(void *);
            result->chain = chain;
            result->base_pos = 0;
            result->pos = ARENA_HEADER_SIZE;
            result->cmt = ARENA_DEFAULT_COMMIT_SIZE;
            result->res = reserve_size_aligned;
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
    void *result = arena_push_no_zero(arena, size);
    MemoryZero(result, size);
    return result;
}

internal void *
arena_push_no_zero(Arena *arena, U64 size)
{
    void *result = 0;
    Arena *current = arena->current;
    if (arena->chain)
    {
        U64 next_chunk_pos = AlignUpPow2(current->pos, arena->alignment);
        next_chunk_pos += size;
        if (next_chunk_pos > current->res)
        {
            U64 new_reserve_size = ARENA_DEFAULT_RESERVE_SIZE;
            U64 enough_to_fit = ARENA_HEADER_SIZE + size;
            if (new_reserve_size < enough_to_fit)
            {
                new_reserve_size = AlignUpPow2(enough_to_fit, KB(4));
            }
            void *memory = os_memory_reserve(new_reserve_size);
            if (os_memory_commit(memory, ARENA_DEFAULT_COMMIT_SIZE))
            {
                AsanPoison(memory, new_reserve_size);
                AsanUnpoison(memory, ARENA_HEADER_SIZE);
                Arena *new_chunk = (Arena *)memory;
                new_chunk->current = new_chunk;
                new_chunk->prev = current;
                new_chunk->alignment = sizeof(void *);
                new_chunk->chain = 1;
                new_chunk->base_pos = current->base_pos + current->res;
                new_chunk->pos = ARENA_HEADER_SIZE;
                new_chunk->cmt = ARENA_DEFAULT_COMMIT_SIZE;
                new_chunk->res = new_reserve_size;
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
            U64 next_commit_pos_aligned = AlignUpPow2(next_chunk_pos, ARENA_DEFAULT_COMMIT_SIZE);
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
    return result;
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
arena_pop_amount(Arena *arena, U64 amount)
{
    Arena *current = arena->current;
    U64 total_pos = current->base_pos + current->pos;
    if (amount <= total_pos)
    {
        U64 new_pos = total_pos - amount;
        arena_pop_to(arena, new_pos);
    }
}

internal void
arena_clear(Arena *arena)
{
    arena_pop_to(arena, 0);
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
