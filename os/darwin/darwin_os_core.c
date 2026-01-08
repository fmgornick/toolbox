#ifndef DARWIN_OS_CORE_C
#define DARWIN_OS_CORE_C

/*
 * TODO(fletcher)
 *
 * - implement semaphores
 * - implement barrier
 * - tests
 */

internal OS_Darwin_Entity *
os_darwin_entity_alloc(OS_Darwin_EntityKind kind)
{
    OS_Darwin_Entity *result = 0;
    pthread_mutex_lock(&os_darwin_state.entity_mutex);
    {
        result = pool_push(os_darwin_state.entity_pool);
    }
    pthread_mutex_unlock(&os_darwin_state.entity_mutex);
    result->kind = kind;
    return result;
}

internal void
os_darwin_entity_release(OS_Darwin_Entity *entity)
{
    pthread_mutex_lock(&os_darwin_state.entity_mutex);
    {
        pool_pop(os_darwin_state.entity_pool, entity);
    }
    pthread_mutex_unlock(&os_darwin_state.entity_mutex);
}

internal void *
os_darwin_thread_entry(void *ptr)
{
    OS_Darwin_Entity *entity = (OS_Darwin_Entity *)ptr;
    ThreadEntryPoint *func = entity->thread.func;
    void *args = entity->thread.args;
    func(args);
}

internal void
os_init(void)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int mutex_result = pthread_mutex_init(&os_darwin_state.entity_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    if (mutex_result != 0)
    {
        os_abort(1);
    }
    os_darwin_state.entity_pool = pool_alloc(sizeof(OS_Darwin_Entity));
}

internal void
os_abort(S32 exit_code)
{
    exit(exit_code);
}

internal OS_Thread
os_thread_launch(ThreadEntryPoint *func, void *args)
{
    OS_Thread result = { 0 };
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_Thread);
    entity->thread.func = func;
    entity->thread.args = args;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    int thread_result = pthread_create(&entity->thread.handle, &attr, os_darwin_thread_entry, entity);
    pthread_attr_destroy(&attr);
    if (thread_result == 0) /* success */
    {
        result = (OS_Thread){ (U64)entity };
    }
    else /* fail */
    {
        os_darwin_entity_release(entity);
    }
    return result;
}

internal B32
os_thread_join(OS_Thread thread)
{
    B32 result = 0;
    if (!MemoryIsZeroStruct(&thread))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)thread.u64[0];
        int thread_result = pthread_join(entity->thread.handle, 0);
        os_darwin_entity_release(entity);
        result = (thread_result == 0);
    }
    return result;
}

internal void
os_thread_detach(OS_Thread thread)
{
    if (!MemoryIsZeroStruct(&thread))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)thread.u64[0];
        os_darwin_entity_release(entity);
    }
}

internal OS_Mutex
os_mutex_alloc(void)
{
    OS_Mutex result = { 0 };
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_Mutex);
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    int mutex_result = pthread_mutex_init(&entity->mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    if (mutex_result == 0) /* success */
    {
        result = (OS_Mutex){ (U64)entity };
    }
    else /* fail */
    {
        os_darwin_entity_release(entity);
    }
    return result;
}

internal void
os_mutex_release(OS_Mutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_mutex_destroy(&entity->mutex);
        os_darwin_entity_release(entity);
    }
}

internal void
os_mutex_lock(OS_Mutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_mutex_lock(&entity->mutex);
    }
}

internal void
os_mutex_unlock(OS_Mutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_mutex_unlock(&entity->mutex);
    }
}

internal OS_RWMutex
os_rw_mutex_alloc(void)
{
    OS_RWMutex result = { 0 };
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_RWMutex);
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    int rwlock_result = pthread_rwlock_init(&entity->rw_mutex, &attr);
    pthread_rwlockattr_destroy(&attr);
    if (rwlock_result == 0) /* success */
    {
        result = (OS_RWMutex){ (U64)entity };
    }
    else /* fail */
    {
        os_darwin_entity_release(entity);
    }
    return result;
}

internal void
os_rw_mutex_release(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_destroy(&entity->rw_mutex);
        os_darwin_entity_release(entity);
    }
}

internal void
os_rw_mutex_lock_r(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_rdlock(&entity->rw_mutex);
    }
}

internal void
os_rw_mutex_lock_w(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_wrlock(&entity->rw_mutex);
    }
}

internal void
os_rw_mutex_unlock_r(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_unlock(&entity->rw_mutex);
    }
}

internal void
os_rw_mutex_unlock_w(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_unlock(&entity->rw_mutex);
    }
}

internal OS_Condvar
os_condvar_alloc(void)
{
    OS_Condvar result = { 0 };
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_Condvar);
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);
    int condvar_result = pthread_cond_init(&entity->condvar.cv_handle, &attr);
    pthread_condattr_destroy(&attr);
    if (condvar_result == 0) /* success */
    {
        result = (OS_Condvar){ (U64)entity };
    }
    else /* fail */
    {
        os_darwin_entity_release(entity);
    }
    return result;
}

internal void
os_condvar_release(OS_Condvar condvar)
{
    if (!MemoryIsZeroStruct(&condvar))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)condvar.u64[0];
        pthread_cond_destroy(&entity->condvar.cv_handle);
        os_darwin_entity_release(entity);
    }
}

internal B32
os_condvar_wait(OS_Condvar condvar, OS_Mutex mutex)
{
    B32 result = 0;
    if (!MemoryIsZeroStruct(&condvar) && !MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *condvar_entity = (OS_Darwin_Entity *)condvar.u64[0];
        OS_Darwin_Entity *mutex_entity = (OS_Darwin_Entity *)mutex.u64[0];
        int wait_result = pthread_cond_wait(&condvar_entity->condvar.cv_handle, &mutex_entity->mutex);
        result = (wait_result == 0);
    }
    return result;
}

internal B32
os_condvar_wait_ms(OS_Condvar condvar, OS_Mutex mutex, U64 max_wait_ms)
{
    B32 result = 0;
    if (!MemoryIsZeroStruct(&condvar) && !MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *condvar_entity = (OS_Darwin_Entity *)condvar.u64[0];
        OS_Darwin_Entity *mutex_entity = (OS_Darwin_Entity *)mutex.u64[0];
        struct timeval now;
        struct timespec wait;
        gettimeofday(&now, 0);
        wait.tv_sec = now.tv_sec + (max_wait_ms / 1000L);
        wait.tv_nsec = (now.tv_usec * 1000L) + (max_wait_ms % 1000) * 1000000L;
        int wait_result = pthread_cond_timedwait(&condvar_entity->condvar.cv_handle, &mutex_entity->mutex, &wait);
        result = (wait_result == 0);
    }
    return result;
}

internal B32
os_condvar_wait_r(OS_Condvar condvar, OS_RWMutex rw_mutex)
{
    B32 result = 0;
    if (!MemoryIsZeroStruct(&condvar) && !MemoryIsZeroStruct(&rw_mutex))
    {
        OS_Darwin_Entity *condvar_entity = (OS_Darwin_Entity *)condvar.u64[0];
        OS_Darwin_Entity *rw_mutex_entity = (OS_Darwin_Entity *)rw_mutex.u64[0];
        pthread_mutex_lock(&condvar_entity->condvar.rw_mutex_handle);
        pthread_rwlock_unlock(&rw_mutex_entity->rw_mutex);
        int wait_result = pthread_cond_wait(&condvar_entity->condvar.cv_handle, &condvar_entity->condvar.rw_mutex_handle);
        pthread_rwlock_rdlock(&rw_mutex_entity->rw_mutex);
        pthread_mutex_unlock(&condvar_entity->condvar.rw_mutex_handle);
        result = (wait_result == 0);
    }
    return result;
}

internal B32
os_condvar_wait_w(OS_Condvar condvar, OS_RWMutex rw_mutex)
{
    B32 result = 0;
    if (!MemoryIsZeroStruct(&condvar) && !MemoryIsZeroStruct(&rw_mutex))
    {
        OS_Darwin_Entity *condvar_entity = (OS_Darwin_Entity *)condvar.u64[0];
        OS_Darwin_Entity *rw_mutex_entity = (OS_Darwin_Entity *)rw_mutex.u64[0];
        pthread_mutex_lock(&condvar_entity->condvar.rw_mutex_handle);
        pthread_rwlock_unlock(&rw_mutex_entity->rw_mutex);
        int wait_result = pthread_cond_wait(&condvar_entity->condvar.cv_handle, &condvar_entity->condvar.rw_mutex_handle);
        pthread_rwlock_wrlock(&rw_mutex_entity->rw_mutex);
        pthread_mutex_unlock(&condvar_entity->condvar.rw_mutex_handle);
        result = (wait_result == 0);
    }
    return result;
}

internal B32
os_condvar_wait_r_ms(OS_Condvar condvar, OS_RWMutex rw_mutex, U64 max_wait_ms)
{
    B32 result = 0;
    if (!MemoryIsZeroStruct(&condvar) && !MemoryIsZeroStruct(&rw_mutex))
    {
        OS_Darwin_Entity *condvar_entity = (OS_Darwin_Entity *)condvar.u64[0];
        OS_Darwin_Entity *rw_mutex_entity = (OS_Darwin_Entity *)rw_mutex.u64[0];
        struct timeval now;
        struct timespec wait;
        gettimeofday(&now, 0);
        wait.tv_sec = now.tv_sec + (max_wait_ms / 1000L);
        wait.tv_nsec = (now.tv_usec * 1000L) + (max_wait_ms % 1000) * 1000000L;
        pthread_mutex_lock(&condvar_entity->condvar.rw_mutex_handle);
        pthread_rwlock_unlock(&rw_mutex_entity->rw_mutex);
        int wait_result = pthread_cond_timedwait(&condvar_entity->condvar.cv_handle, &condvar_entity->condvar.rw_mutex_handle, &wait);
        pthread_rwlock_rdlock(&rw_mutex_entity->rw_mutex);
        pthread_mutex_unlock(&condvar_entity->condvar.rw_mutex_handle);
        result = (wait_result == 0);
    }
    return result;
}

internal B32
os_condvar_wait_w_ms(OS_Condvar condvar, OS_RWMutex rw_mutex, U64 max_wait_ms)
{
    B32 result = 0;
    if (!MemoryIsZeroStruct(&condvar) && !MemoryIsZeroStruct(&rw_mutex))
    {
        OS_Darwin_Entity *condvar_entity = (OS_Darwin_Entity *)condvar.u64[0];
        OS_Darwin_Entity *rw_mutex_entity = (OS_Darwin_Entity *)rw_mutex.u64[0];
        struct timeval now;
        struct timespec wait;
        gettimeofday(&now, 0);
        wait.tv_sec = now.tv_sec + (max_wait_ms / 1000L);
        wait.tv_nsec = (now.tv_usec * 1000L) + (max_wait_ms % 1000) * 1000000L;
        pthread_mutex_lock(&condvar_entity->condvar.rw_mutex_handle);
        pthread_rwlock_unlock(&rw_mutex_entity->rw_mutex);
        int wait_result = pthread_cond_timedwait(&condvar_entity->condvar.cv_handle, &condvar_entity->condvar.rw_mutex_handle, &wait);
        pthread_rwlock_wrlock(&rw_mutex_entity->rw_mutex);
        pthread_mutex_unlock(&condvar_entity->condvar.rw_mutex_handle);
        result = (wait_result == 0);
    }
    return result;
}

internal void
os_condvar_notify_one(OS_Condvar condvar)
{
    if (!MemoryIsZeroStruct(&condvar))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)condvar.u64[0];
        pthread_cond_signal(&entity->condvar.cv_handle);
    }
}

internal void
os_condvar_notify_all(OS_Condvar condvar)
{
    if (!MemoryIsZeroStruct(&condvar))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)condvar.u64[0];
        pthread_cond_broadcast(&entity->condvar.cv_handle);
    }
}

internal OS_Semaphore
os_semaphore_alloc(U64 initial_count, U64 max_count, String8 name)
{
    NotImplemented;
}

internal void
os_semaphore_release(OS_Semaphore semaphore)
{
    NotImplemented;
}

internal B32
os_semaphore_take(OS_Semaphore semaphore)
{
    NotImplemented;
}

internal B32
os_semaphore_take_n(OS_Semaphore semaphore, U32 n)
{
    NotImplemented;
}

internal B32
os_semaphore_take_timeout_ms(OS_Semaphore semaphore, U64 max_wait_ms)
{
    NotImplemented;
}

internal B32
os_semaphore_take_n_timeout_ms(OS_Semaphore semaphore, U32 n, U64 max_wait_ms)
{
    NotImplemented;
}

internal void
os_semaphore_drop(OS_Semaphore semaphore)
{
    NotImplemented;
}

internal void
os_semaphore_drop_n(OS_Semaphore semaphore, U32 n)
{
    NotImplemented;
}

internal OS_Barrier
os_barrier_alloc(U64 count)
{
    NotImplemented;
}

internal void
os_barrier_release(OS_Barrier barrier)
{
    NotImplemented;
}

internal void
os_barrier_wait(OS_Barrier barrier)
{
    NotImplemented;
}

internal void *
os_memory_reserve(U64 size)
{
    void *result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (result == MAP_FAILED)
    {
        result = 0;
    }
    return result;
}

internal B32
os_memory_commit(void *ptr, U64 size)
{
    B32 result = (mprotect(ptr, size, PROT_READ | PROT_WRITE) == 0);
    return result;
}

internal void
os_memory_decommit(void *ptr, U64 size)
{
    madvise(ptr, size, MADV_DONTNEED);
    mprotect(ptr, size, PROT_NONE);
}

internal void
os_memory_release(void *ptr, U64 size)
{
    munmap(ptr, size);
}

internal void
os_sleep_ms(U64 ms)
{
    usleep(ms * Thousand(1));
}

#endif // DARWIN_OS_CORE_C
