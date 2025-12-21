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
        void *memory = os_memory_reserve(reserve_size);
        if (os_memory_commit(memory, ARENA_DEFAULT_COMMIT_SIZE))
        {
            result = (Arena *)memory;
            result->current = result;
            result->prev = 0;
            result->alignment = sizeof(void *);
            result->chain = chain;
            result->base_pos = 0;
            result->pos = ARENA_HEADER_SIZE;
            result->cmt = ARENA_DEFAULT_COMMIT_SIZE;
            result->res = reserve_size;
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
        os_memory_release(current, current->res);
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
            U64 enough_to_fit = size + ARENA_HEADER_SIZE;
            if (new_reserve_size < enough_to_fit)
            {
                new_reserve_size = AlignUpPow2(enough_to_fit, KB(4));
            }
            void *memory = os_memory_reserve(new_reserve_size);
            if (os_memory_commit(memory, ARENA_DEFAULT_COMMIT_SIZE))
            {
                Arena *next = (Arena *)current;
                next->current = next;
                next->prev = current;
                next->alignment = sizeof(void *);
                next->chain = 1;
                next->base_pos = current->base_pos + current->res;
                next->pos = ARENA_HEADER_SIZE;
                next->cmt = ARENA_DEFAULT_COMMIT_SIZE;
                next->res = new_reserve_size;
                current = next;
                arena->current = next;
            }
        }
    }

    U64 result_pos = AlignUpPow2(current->pos, arena->alignment);
    U64 next_chunk_pos = size + result_pos;
    if (next_chunk_pos <= current->res)
    {
        if (next_chunk_pos > current->cmt)
        {
            U64 next_commit_pos_aligned = AlignUpPow2(next_chunk_pos, ARENA_DEFAULT_COMMIT_SIZE);
            U64 next_commit_pos = ClampTop(next_commit_pos_aligned, current->res);
            U64 commit_size = next_commit_pos - current->cmt;
            if (os_memory_commit((void *)current + current->cmt, commit_size))
            {
                current->cmt = next_commit_pos;
            }
        }
        if (next_chunk_pos <= current->cmt)
        {
            result = (void *)current + result_pos;
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
    U64 total_pos = current->base_pos + current->pos;
    if (pos < total_pos)
    {
        U64 clamped_total_pos = ClampBot(pos, ARENA_HEADER_SIZE);
        while (clamped_total_pos < current->base_pos)
        {
            Arena *prev = current->prev;
            os_memory_release(current, current->res);
            current = prev;
        }
        arena->current = current;
        U64 new_pos = clamped_total_pos - current->base_pos;
        U64 clamped_new_pos = ClampBot(new_pos, ARENA_HEADER_SIZE);
        current->pos = clamped_new_pos;
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
