#ifndef BASE_THREAD_CONTEXT_H
#define BASE_THREAD_CONTEXT_H

/*
 * NOTE(fletcher): This is independent of the OS layer implementation.
 *
 * Creation/deletion of threads is dependent on the OS, but if we assume the
 * calling code is single threaded, then thread local context can be used
 * without knowing the OS.
 */

typedef struct ThreadContext {
    Arena *arenas[2];
    U8 thread_name[64];
    U64 thread_name_size;
} ThreadContext;

C_LINKAGE thread_local ThreadContext *tctx_thread_local;

internal ThreadContext *tctx_alloc(void);
internal void tctx_release(ThreadContext *tctx);
internal ThreadContext *tctx_selected(void);
internal void tctx_select(ThreadContext *tctx);

internal void tctx_set_thread_name(String8 name);

internal Arena *tctx_get_scratch(Arena **conflicting, U64 count);
#define scratch_begin(conflicts, count) scratch_push(tctx_get_scratch((conflicts), (count)))
#define scratch_end(scratch) scratch_pop((scratch))

#endif /* BASE_THREAD_CONTEXT_H */
