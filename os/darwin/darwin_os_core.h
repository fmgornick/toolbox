#ifndef DARWIN_OS_CORE_H
#define DARWIN_OS_CORE_H

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum OS_Darwin_EntityKind {
    OS_Darwin_EntityKind_None,
    OS_Darwin_EntityKind_Thread,
    OS_Darwin_EntityKind_Mutex,
    OS_Darwin_EntityKind_RWMutex,
    OS_Darwin_EntityKind_Condvar,
    OS_Darwin_EntityKind_Semaphore,
    OS_Darwin_EntityKind_Barrier
} OS_Darwin_EntityKind;

typedef struct OS_Darwin_Entity {
    OS_Darwin_EntityKind kind;
    union {
        struct {
            ThreadEntryPoint *func;
            void *args;
            pthread_t handle;
        } thread;
        pthread_mutex_t mutex;
        pthread_rwlock_t rw_mutex;
        struct {
            pthread_cond_t cv_handle;
            pthread_mutex_t mutex_handle;
        } condvar;
        struct {
            sem_t *sem_handle;
            pthread_mutex_t mutex_handle;
            U64 count;
            U64 max;
        } semaphore;
        struct {
            pthread_mutex_t mutex;
            pthread_cond_t condvar;
        } barrier;
    } type;
} OS_Darwin_Entity;

typedef struct OS_Darwin_State {
    pthread_mutex_t entity_mutex;
    Pool *entity_pool;
} OS_Darwin_State;

internal OS_Darwin_Entity *os_darwin_entity_alloc(OS_Darwin_EntityKind kind);
internal void os_darwin_entity_release(OS_Darwin_Entity *entity);
internal void *os_darwin_thread_entry(void *ptr);

global OS_Darwin_State os_darwin_state = {0};

#endif /* DARWIN_OS_CORE_H */
