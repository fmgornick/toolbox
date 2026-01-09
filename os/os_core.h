#ifndef OS_CORE_H
#define OS_CORE_H

typedef struct OS_Thread {
    U64 u64[1];
} OS_Thread;

typedef void ThreadEntryPoint(void *p);

typedef struct OS_Mutex {
    U64 u64[1];
} OS_Mutex;

typedef struct OS_RWMutex {
    U64 u64[1];
} OS_RWMutex;

typedef struct OS_Condvar {
    U64 u64[1];
} OS_Condvar;

typedef struct OS_Semaphore {
    U64 u64[1];
} OS_Semaphore;

typedef struct OS_Barrier {
    U64 u64[1];
} OS_Barrier;

/* initialize and abort ----------------------------------------------------- */

internal void os_init(void);
internal void os_abort(S32 exit_code);

/* threads ------------------------------------------------------------------ */

internal OS_Thread os_thread_launch(ThreadEntryPoint *func, void *args);
internal B32 os_thread_join(OS_Thread thread);
internal void os_thread_detach(OS_Thread thread);

/* recursive mutexs --------------------------------------------------------- */

internal OS_Mutex os_mutex_alloc(void);
internal void os_mutex_release(OS_Mutex mutex);
internal void os_mutex_lock(OS_Mutex mutex);
internal void os_mutex_unlock(OS_Mutex mutex);

/* read/write mutexs -------------------------------------------------------- */

internal OS_RWMutex os_rw_mutex_alloc(void);
internal void os_rw_mutex_release(OS_RWMutex mutex);
internal void os_rw_mutex_lock_r(OS_RWMutex mutex);
internal void os_rw_mutex_lock_w(OS_RWMutex mutex);
internal void os_rw_mutex_unlock_r(OS_RWMutex mutex);
internal void os_rw_mutex_unlock_w(OS_RWMutex mutex);

/* condition variables ------------------------------------------------------ */

internal OS_Condvar os_condvar_alloc(void);
internal void os_condvar_release(OS_Condvar condvar);
internal B32 os_condvar_wait(OS_Condvar condvar, OS_Mutex mutex);
internal B32 os_condvar_wait_r(OS_Condvar condvar, OS_RWMutex rw_mutex);
internal B32 os_condvar_wait_w(OS_Condvar condvar, OS_RWMutex rw_mutex);
internal B32 os_condvar_wait_ms(OS_Condvar condvar, OS_Mutex mutex, U64 max_wait_ms);
internal B32 os_condvar_wait_r_ms(OS_Condvar condvar, OS_RWMutex mutex, U64 max_wait_ms);
internal B32 os_condvar_wait_w_ms(OS_Condvar condvar, OS_RWMutex mutex, U64 max_wait_ms);
internal void os_condvar_notify_one(OS_Condvar condvar);
internal void os_condvar_notify_all(OS_Condvar condvar);

/* semaphores --------------------------------------------------------------- */

internal OS_Semaphore os_semaphore_alloc(U64 initial_count, U64 max_count, String8 name);
internal void os_semaphore_release(OS_Semaphore semaphore);
internal B32 os_semaphore_take(OS_Semaphore semaphore);
internal B32 os_semaphore_take_n(OS_Semaphore semaphore, U32 n);
internal B32 os_semaphore_take_timeout_ms(OS_Semaphore semaphore, U64 max_wait_ms);
internal B32 os_semaphore_take_n_timeout_ms(OS_Semaphore semaphore, U32 n, U64 max_wait_ms);
internal void os_semaphore_drop(OS_Semaphore semaphore);
internal void os_semaphore_drop_n(OS_Semaphore semaphore, U32 n);

/* barriers ----------------------------------------------------------------- */

internal OS_Barrier os_barrier_alloc(U64 count);
internal void os_barrier_release(OS_Barrier barrier);
internal void os_barrier_wait(OS_Barrier barrier);

/* memory allocation -------------------------------------------------------- */

internal void *os_memory_reserve(U64 size);
internal B32 os_memory_commit(void *ptr, U64 size);
internal void os_memory_decommit(void *ptr, U64 size);
internal void os_memory_release(void *ptr, U64 size);

/* time --------------------------------------------------------------------- */

internal void os_sleep_ms(U64 ms);

#endif // OS_CORE_H
