#ifndef BASE_ENTRY_POINT_C
#define BASE_ENTRY_POINT_C

internal void
main_thread_entry(int argc, char **argv)
{

    tctx_set_thread_name(str8_lit("main thread"));
    entry_point(argc, argv);
}

internal void
worker_thread_entry(void (*func)(void *params), void *params)
{
    ThreadContext *tctx = tctx_alloc();
    tctx_select(tctx);
    func(params);
    tctx_release(tctx);
}

#endif /* BASE_ENTRY_POINT_C */
