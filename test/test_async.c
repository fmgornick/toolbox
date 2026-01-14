#ifndef TEST_ASYNC_C
#define TEST_ASYNC_C

typedef struct TLS {
    OS_Mutex mutex;
    OS_RWMutex rw_mutex;
    OS_Condvar condvar;
    OS_Semaphore semaphore;
    OS_Barrier barrier;
} TLS;

internal void thread1_func(void *params);
internal void thread2_func(void *params);

internal void
test_async(void)
{
    TLS tls = {0};
    OS_Thread thread1, thread2;
    tls.mutex = os_mutex_alloc();
    tls.rw_mutex = os_rw_mutex_alloc();
    tls.condvar = os_condvar_alloc();
    tls.semaphore = os_semaphore_alloc(0, 2);
    tls.barrier = os_barrier_alloc(2);

    thread1 = os_thread_launch(thread1_func, &tls);
    thread2 = os_thread_launch(thread2_func, &tls);

    /*
       NOTE(fletcher): not really necessary

       OS layer creates/deletes everything with a global pool allocator that
       gets freed at the end anyway.
     */
    os_mutex_release(tls.mutex);
    os_rw_mutex_release(tls.rw_mutex);
    os_condvar_release(tls.condvar);
    os_semaphore_release(tls.semaphore);
    os_barrier_release(tls.barrier);
}

internal void
thread1_func(void *params)
{
    tctx_thread_name_set(str8_lit("thread 1"));
}

internal void
thread2_func(void *params)
{
    tctx_thread_name_set(str8_lit("thread 2"));
}

#endif /* TEST_ASYNC_C */
