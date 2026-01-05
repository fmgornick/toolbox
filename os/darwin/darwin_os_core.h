#ifndef DARWIN_OS_CORE_H
#define DARWIN_OS_CORE_H

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/time.h>

typedef enum OS_Darwin_EntityKind {
    OS_Darwin_EntityKind_None,
    OS_Darwin_EntityKind_Thread,
    OS_Darwin_EntityKind_Mutex,
    OS_Darwin_EntityKind_RWMutex,
    OS_Darwin_EntityKind_Condvar,
    OS_Darwin_EntityKind_Semaphore,
    OS_Darwin_EntityKind_Barrier,
} OS_Darwin_EntityKind;

typedef struct OS_Darwin_Entity OS_Darwin_Entity;
struct OS_Darwin_Entity {
    OS_Darwin_EntityKind kind;
    union {
        struct {
            void *func;
            void *args;
            pthread_t handle;
        } thread;
        pthread_mutex_t mutex;
        pthread_rwlock_t rw_mutex;
        pthread_cond_t condvar;
        sem_t semaphore;
        struct {
            pthread_mutex_t mutex;
            pthread_cond_t condvar;
        } barrier;
    };
};

typedef struct OS_Darwin_State {
    pthread_mutex_t entity_mutex;
    Pool *entity_pool;
} OS_Darwin_State;

internal OS_Darwin_Entity *os_darwin_entity_alloc(OS_Darwin_EntityKind kind);
internal void os_darwin_entity_release(OS_Darwin_Entity *entity);
internal void *os_darwin_thread_entry(void *ptr);

#endif // DARWIN_OS_CORE_H
