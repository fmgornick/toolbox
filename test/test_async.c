#ifndef TEST_ASYNC_C
#define TEST_ASYNC_C

typedef struct TLS {
    U64 param1;
    U64 param2;
} TLS;

internal void
thread_func(void *params)
{
    TLS *tls = (TLS *)params;
    tctx_thread_name_set(str8_lit("worker"));
    printf("thread: %.*s\n", str8_varg(tctx_thread_name_get()));
    printf("%llu + %llu = %llu\n", tls->param1, tls->param2, tls->param1 + tls->param2);
}

internal void
test_async(void)
{
    TLS tls = {2, 2};
    OS_Thread thread = os_thread_launch(thread_func, (void *)&tls);
    printf("thread: %.*s\n", str8_varg(tctx_thread_name_get()));
    os_thread_join(thread);
}

#endif /* TEST_ASYNC_C */
