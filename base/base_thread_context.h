#ifndef BASE_THREAD_CONTEXT_H
#define BASE_THREAD_CONTEXT_H

/*
   NOTE(fletcher): This is ~technically~ independent of the OS layer.

   - os layer included => Define `os_main` in main application, and thread
     context will be automatically allocated and set, system/process info can
     immediately be accessed through exposed os layer operations.

   - os layer NOT included => Define the main entrypoint in application and
     manually allocate the thread context using tctx_alloc and tctx_select.
     System/process info is not exposed without #including the os layer.
 */

typedef struct ThreadContext {
    Arena *arenas[2];
    U8 thread_name[64];
    U64 thread_name_size;
} ThreadContext;

/* global (per thread) context ---------------------------------------------- */
C_LINKAGE thread_local ThreadContext *tctx_thread_local;

/* allocation + getters/setters --------------------------------------------- */
internal ThreadContext *tctx_alloc(void);
internal void tctx_release(ThreadContext *tctx);
internal ThreadContext *tctx_get(void);
internal void tctx_set(ThreadContext *tctx);

/* thread names ------------------------------------------------------------- */
internal void tctx_thread_name_set(String8 name);
internal String8 tctx_thread_name_get(void);

internal Arena *tctx_scratch_get(Arena **conflicting, U64 count);
#define scratch_begin(conflicts, count) scratch_push(tctx_scratch_get((conflicts), (count)))
#define scratch_end(scratch) scratch_pop((scratch))

#endif /* BASE_THREAD_CONTEXT_H */
