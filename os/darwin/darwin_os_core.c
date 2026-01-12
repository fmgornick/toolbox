#ifndef DARWIN_OS_CORE_C
#define DARWIN_OS_CORE_C

/*
   TODO(fletcher): finish darwin platform layer

   - semaphores
   - tests
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
    ThreadEntryPoint *func = entity->type.thread.func;
    void *params = entity->type.thread.params;
    ThreadContext *tctx = tctx_alloc();
    tctx_set(tctx);
    func(params);
    tctx_release(tctx);
    return 0;
}

internal void
os_abort(S32 exit_code)
{
    exit(exit_code);
}

internal OS_Thread
os_thread_launch(ThreadEntryPoint *func, void *params)
{
    OS_Thread result = {0};
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_Thread);
    pthread_attr_t attr;
    int thread_result;
    pthread_attr_init(&attr);
    entity->type.thread.func = func;
    entity->type.thread.params = params;
    thread_result = pthread_create(&entity->type.thread.handle, &attr, os_darwin_thread_entry, entity);
    pthread_attr_destroy(&attr);
    if (thread_result == 0)
    {
        result.u64[0] = (U64)entity;
    }
    else
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
        int thread_result = pthread_join(entity->type.thread.handle, 0);
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
    OS_Mutex result = {0};
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_Mutex);
    pthread_mutexattr_t attr;
    int mutex_result;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    mutex_result = pthread_mutex_init(&entity->type.mutex, &attr);
    pthread_mutexattr_destroy(&attr);
    if (mutex_result == 0)
    {
        result.u64[0] = (U64)entity;
    }
    else
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
        pthread_mutex_destroy(&entity->type.mutex);
        os_darwin_entity_release(entity);
    }
}

internal void
os_mutex_lock(OS_Mutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_mutex_lock(&entity->type.mutex);
    }
}

internal void
os_mutex_unlock(OS_Mutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_mutex_unlock(&entity->type.mutex);
    }
}

internal OS_RWMutex
os_rw_mutex_alloc(void)
{
    OS_RWMutex result = {0};
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_RWMutex);
    pthread_rwlockattr_t attr;
    int rwlock_result;
    pthread_rwlockattr_init(&attr);
    rwlock_result = pthread_rwlock_init(&entity->type.rw_mutex, &attr);
    pthread_rwlockattr_destroy(&attr);
    if (rwlock_result == 0)
    {
        result.u64[0] = (U64)entity;
    }
    else
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
        pthread_rwlock_destroy(&entity->type.rw_mutex);
        os_darwin_entity_release(entity);
    }
}

internal void
os_rw_mutex_lock_r(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_rdlock(&entity->type.rw_mutex);
    }
}

internal void
os_rw_mutex_lock_w(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_wrlock(&entity->type.rw_mutex);
    }
}

internal void
os_rw_mutex_unlock_r(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_unlock(&entity->type.rw_mutex);
    }
}

internal void
os_rw_mutex_unlock_w(OS_RWMutex mutex)
{
    if (!MemoryIsZeroStruct(&mutex))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)mutex.u64[0];
        pthread_rwlock_unlock(&entity->type.rw_mutex);
    }
}

internal OS_Condvar
os_condvar_alloc(void)
{
    OS_Condvar result = {0};
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_Condvar);
    pthread_condattr_t cv_attr;
    pthread_mutexattr_t mutex_attr;
    int cv_result, mutex_result;
    pthread_condattr_init(&cv_attr);
    cv_result = pthread_cond_init(&entity->type.condvar.cv_handle, &cv_attr);
    pthread_condattr_destroy(&cv_attr);
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
    mutex_result = pthread_mutex_init(&entity->type.condvar.mutex_handle, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    if (cv_result == 0 && mutex_result == 0)
    {
        result.u64[0] = (U64)entity;
    }
    else
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
        pthread_cond_destroy(&entity->type.condvar.cv_handle);
        pthread_mutex_destroy(&entity->type.condvar.mutex_handle);
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
        int wait_result = pthread_cond_wait(&condvar_entity->type.condvar.cv_handle, &mutex_entity->type.mutex);
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
        int wait_result;
        struct timeval now;
        struct timespec wait;
        gettimeofday(&now, 0);
        wait.tv_sec = now.tv_sec + (max_wait_ms / 1000L);
        wait.tv_nsec = (now.tv_usec * 1000L) + (max_wait_ms % 1000) * 1000000L;
        wait_result = pthread_cond_timedwait(&condvar_entity->type.condvar.cv_handle, &mutex_entity->type.mutex, &wait);
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
        int wait_result;
        pthread_mutex_lock(&condvar_entity->type.condvar.mutex_handle);
        pthread_rwlock_unlock(&rw_mutex_entity->type.rw_mutex);
        wait_result = pthread_cond_wait(&condvar_entity->type.condvar.cv_handle, &condvar_entity->type.condvar.mutex_handle);
        pthread_rwlock_rdlock(&rw_mutex_entity->type.rw_mutex);
        pthread_mutex_unlock(&condvar_entity->type.condvar.mutex_handle);
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
        int wait_result;
        pthread_mutex_lock(&condvar_entity->type.condvar.mutex_handle);
        pthread_rwlock_unlock(&rw_mutex_entity->type.rw_mutex);
        wait_result = pthread_cond_wait(&condvar_entity->type.condvar.cv_handle, &condvar_entity->type.condvar.mutex_handle);
        pthread_rwlock_wrlock(&rw_mutex_entity->type.rw_mutex);
        pthread_mutex_unlock(&condvar_entity->type.condvar.mutex_handle);
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
        int wait_result;
        struct timeval now;
        struct timespec wait;
        gettimeofday(&now, 0);
        wait.tv_sec = now.tv_sec + (max_wait_ms / 1000L);
        wait.tv_nsec = (now.tv_usec * 1000L) + (max_wait_ms % 1000) * 1000000L;
        pthread_mutex_lock(&condvar_entity->type.condvar.mutex_handle);
        pthread_rwlock_unlock(&rw_mutex_entity->type.rw_mutex);
        wait_result = pthread_cond_timedwait(&condvar_entity->type.condvar.cv_handle, &condvar_entity->type.condvar.mutex_handle, &wait);
        pthread_rwlock_rdlock(&rw_mutex_entity->type.rw_mutex);
        pthread_mutex_unlock(&condvar_entity->type.condvar.mutex_handle);
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
        int wait_result;
        struct timeval now;
        struct timespec wait;
        gettimeofday(&now, 0);
        wait.tv_sec = now.tv_sec + (max_wait_ms / 1000L);
        wait.tv_nsec = (now.tv_usec * 1000L) + (max_wait_ms % 1000) * 1000000L;
        pthread_mutex_lock(&condvar_entity->type.condvar.mutex_handle);
        pthread_rwlock_unlock(&rw_mutex_entity->type.rw_mutex);
        wait_result = pthread_cond_timedwait(&condvar_entity->type.condvar.cv_handle, &condvar_entity->type.condvar.mutex_handle, &wait);
        pthread_rwlock_wrlock(&rw_mutex_entity->type.rw_mutex);
        pthread_mutex_unlock(&condvar_entity->type.condvar.mutex_handle);
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
        pthread_cond_signal(&entity->type.condvar.cv_handle);
    }
}

internal void
os_condvar_notify_all(OS_Condvar condvar)
{
    if (!MemoryIsZeroStruct(&condvar))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)condvar.u64[0];
        pthread_cond_broadcast(&entity->type.condvar.cv_handle);
    }
}

internal OS_Semaphore
os_semaphore_alloc(U64 initial_count, U64 max_count)
{
    OS_Semaphore result = {0};
    if (max_count <= SEM_VALUE_MAX)
    {
        OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_Semaphore);
        pthread_mutexattr_t mutex_attr;
        pthread_condattr_t cv_attr;
        int mutex_result, cv_result;
        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
        mutex_result = pthread_mutex_init(&entity->type.semaphore.mutex_handle, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);
        pthread_condattr_init(&cv_attr);
        cv_result = pthread_cond_init(&entity->type.semaphore.cv_handle, &cv_attr);
        pthread_condattr_destroy(&cv_attr);
        if (mutex_result == 0 && cv_result == 0)
        {
            entity->type.semaphore.count = initial_count;
            entity->type.semaphore.max = max_count;
            result.u64[0] = (U64)entity;
        }
        else
        {
            os_darwin_entity_release(entity);
        }
    }
    return result;
}

internal void
os_semaphore_release(OS_Semaphore semaphore)
{
    if (!MemoryIsZeroStruct(&semaphore))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)semaphore.u64[0];
        pthread_mutex_destroy(&entity->type.semaphore.mutex_handle);
        pthread_cond_destroy(&entity->type.semaphore.cv_handle);
        os_darwin_entity_release(entity);
    }
}

internal B32
os_semaphore_take(OS_Semaphore semaphore)
{
    NotImplemented;
    return 0;
}

internal B32
os_semaphore_take_n(OS_Semaphore semaphore, U32 n)
{
    NotImplemented;
    return 0;
}

internal B32
os_semaphore_take_timeout_ms(OS_Semaphore semaphore, U64 max_wait_ms)
{
    NotImplemented;
    return 0;
}

internal B32
os_semaphore_take_n_timeout_ms(OS_Semaphore semaphore, U32 n, U64 max_wait_ms)
{
    NotImplemented;
    return 0;
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
    OS_Barrier result = {0};
    OS_Darwin_Entity *entity = os_darwin_entity_alloc(OS_Darwin_EntityKind_Barrier);
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cv_attr;
    int mutex_result, cv_result;
    pthread_mutexattr_init(&mutex_attr);
    mutex_result = pthread_mutex_init(&entity->type.barrier.mutex_handle, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    pthread_condattr_init(&cv_attr);
    cv_result = pthread_cond_init(&entity->type.barrier.cv_handle, &cv_attr);
    pthread_condattr_destroy(&cv_attr);
    if (mutex_result == 0 && cv_result == 0)
    {
        entity->type.barrier.count = 0;
        entity->type.barrier.generation = 1;
        entity->type.barrier.max = count;
        result.u64[0] = (U64)entity;
    }
    else
    {
        os_darwin_entity_release(entity);
    }
    return result;
}

internal void
os_barrier_release(OS_Barrier barrier)
{
    if (!MemoryIsZeroStruct(&barrier))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)barrier.u64[0];
        pthread_mutex_destroy(&entity->type.barrier.mutex_handle);
        pthread_cond_destroy(&entity->type.barrier.cv_handle);
        os_darwin_entity_release(entity);
    }
}

internal void
os_barrier_wait(OS_Barrier barrier)
{
    if (!MemoryIsZeroStruct(&barrier))
    {
        OS_Darwin_Entity *entity = (OS_Darwin_Entity *)barrier.u64[0];
        U64 generation;
        pthread_mutex_lock(&entity->type.barrier.mutex_handle);
        generation = entity->type.barrier.generation;
        entity->type.barrier.count += 1;
        if (entity->type.barrier.count == entity->type.barrier.max)
        {
            entity->type.barrier.count = 0;
            entity->type.barrier.generation += 1;
            pthread_cond_broadcast(&entity->type.barrier.cv_handle);
        }
        else
        {
            while (generation == entity->type.barrier.generation)
            {
                pthread_cond_wait(&entity->type.barrier.cv_handle, &entity->type.barrier.mutex_handle);
            }
        }
        pthread_mutex_unlock(&entity->type.barrier.mutex_handle);
    }
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

int
main(int argc, char **argv)
{
    ThreadContext *tctx = tctx_alloc();
    tctx_set(tctx);

    /* NOTE(fletcher): set up os layer */
    {
        pthread_mutexattr_t mutex_attr;
        int mutex_result;
        String8 *machine_name = &os_darwin_state.system_info.machine_name;
        U64 host_name_len, host_name_max_len;

        os_darwin_state.system_info.logical_processor_count = (U64)sysconf(_SC_NPROCESSORS_CONF);
        os_darwin_state.system_info.page_size = (U64)getpagesize();
        os_darwin_state.process_info.pid = (U64)getpid();

        os_darwin_state.arena = arena_alloc();
        os_darwin_state.entity_pool = pool_alloc(sizeof(OS_Darwin_Entity));

        pthread_mutexattr_init(&mutex_attr);
        pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE);
        mutex_result = pthread_mutex_init(&os_darwin_state.entity_mutex, &mutex_attr);
        pthread_mutexattr_destroy(&mutex_attr);
        if (mutex_result != 0)
        {
            os_abort(1);
        }

        host_name_max_len = sysconf(_SC_HOST_NAME_MAX);
        machine_name->str = arena_push(os_darwin_state.arena, host_name_max_len + 1);
        gethostname((char *)machine_name->str, host_name_max_len);
        host_name_len = sizeof(machine_name->str);
        arena_pop(os_darwin_state.arena, host_name_max_len - host_name_len - 1);
        machine_name->str[host_name_len] = '\0';
        machine_name->size = host_name_len;
    }

    tctx_thread_name_set(str8_lit("main"));
    os_main(argc, argv);
}

#endif /* DARWIN_OS_CORE_C */
