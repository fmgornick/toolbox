#ifndef BASE_THREAD_CONTEXT_C
#define BASE_THREAD_CONTEXT_C

thread_local ThreadContext *tctx_thread_local = 0;

internal ThreadContext *
tctx_alloc(void)
{
    Arena *arena1 = arena_alloc();
    Arena *arena2 = arena_alloc();
    ThreadContext *tctx = arena_push(arena1, sizeof(ThreadContext));
    tctx->arenas[0] = arena1;
    tctx->arenas[1] = arena2;
    return tctx;
}

internal void
tctx_release(ThreadContext *tctx)
{
    arena_release(tctx->arenas[1]);
    arena_release(tctx->arenas[0]);
}

internal ThreadContext *
tctx_selected(void)
{
    return tctx_thread_local;
}

internal void
tctx_select(ThreadContext *tctx)
{
    tctx_thread_local = tctx;
}

internal void
tctx_set_thread_name(String8 name)
{
    ThreadContext *tctx = tctx_selected();
    U64 size = ClampTop(name.size, sizeof(tctx->thread_name));
    MemoryCopy(tctx->thread_name, name.str, size);
    tctx->thread_name_size = size;
}

internal Arena *
tctx_get_scratch(Arena **conflicting, U64 count)
{
    Arena *result = 0;
    ThreadContext *tctx = tctx_selected();
    U64 i;
    for (i = 0; i < ArrayCount(tctx->arenas); i += 1)
    {
        B32 has_conflict = 0;
        U64 j;
        for (j = 0; j < count; j += 1)
        {
            if (tctx->arenas[i] == conflicting[j])
            {
                has_conflict = 1;
                break;
            }
        }
        if (!has_conflict)
        {
            result = tctx->arenas[i];
        }
    }
    return result;
}

#endif /* BASE_THREAD_CONTEXT_C */
