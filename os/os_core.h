#ifndef OS_CORE_H
#define OS_CORE_H

internal void *os_memory_reserve(U64 size);
internal B32 os_memory_commit(void *ptr, U64 size);
internal void os_memory_decommit(void *ptr, U64 size);
internal void os_memory_release(void *ptr, U64 size);

#endif // OS_CORE_H
