typedef struct TLS {
    OS_Mutex mutex;
    OS_Condvar condvar;
} TLS;

global B32 done = 0;

internal void *
thread_func(void *arg)
{
    TLS *tls = (TLS *)arg;
    os_sleep_ms(Thousand(3));
    os_mutex_lock(tls->mutex);
    printf("child\n");
    done = 1;
    os_condvar_notify_one(tls->condvar);
    printf("notified\n");
    os_sleep_ms(Thousand(1));
    os_mutex_unlock(tls->mutex);
}

internal void
test_threads(Arena *arena)
{
    OS_Mutex mutex;
    OS_Condvar condvar;
    OS_Thread thread;
    TLS tls;
    os_init();
    mutex = os_mutex_alloc();
    condvar = os_condvar_alloc();
    tls.mutex = mutex;
    tls.condvar = condvar;

    thread = os_thread_launch((ThreadEntryPoint *)thread_func, (void *)&tls);
    printf("initial wait (1 sec)...\n");

    printf("waiting on condvar...\n");
    os_mutex_lock(mutex);
    while (done == 0)
    {
        B32 res;
        printf("parent\n");
        res = os_condvar_wait_ms(condvar, mutex, 5000);
        printf("res: %d\n", res);
    }

    os_mutex_unlock(mutex);
    os_thread_join(thread);
}
