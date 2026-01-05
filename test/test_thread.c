#include <unistd.h>

global B32 done = 0;

typedef struct TLS {
    OS_Mutex mutex;
    OS_Condvar condvar;
} TLS;

internal void *
thread_func(void *arg)
{
    TLS *tls = (TLS *)arg;
    sleep(3);
    os_mutex_lock(tls->mutex);
    printf("child\n");
    done = 1;
    os_condvar_notify_one(tls->condvar);
    printf("notified\n");
    sleep(1);
    os_mutex_unlock(tls->mutex);
}

internal void
test_threads(Arena *arena)
{
    os_init();
    OS_Mutex mutex = os_mutex_alloc();
    OS_Condvar condvar = os_condvar_alloc();
    TLS tls = { 0 };
    tls.mutex = mutex;
    tls.condvar = condvar;

    OS_Thread thread = os_thread_launch((ThreadEntryPoint *)thread_func, (void *)&tls);
    printf("initial wait (1 sec)...\n");

    printf("waiting on condvar...\n");
    os_mutex_lock(mutex);
    while (done == 0)
    {
        printf("parent\n");
        B32 res = os_condvar_wait_ms(condvar, mutex, 5000);
        printf("res: %d\n", res);
    }

    os_mutex_unlock(mutex);
    os_thread_join(thread);
}
