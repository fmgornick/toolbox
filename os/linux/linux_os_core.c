#ifndef LINUX_OS_CORE_C
#define LINUX_OS_CORE_C

#if defined(MADV_DONTNEED)
internal void *
os_memory_reserve(U64 size)
{
    void *result = mmap(0, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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
#else
internal void *
os_memory_reserve(U64 size)
{
    return malloc(size);
}

internal B32
os_memory_commit(void *ptr, U64 size)
{
    return 1;
}

internal void
os_memory_decommit(void *ptr, U64 size)
{
}

internal void
os_memory_release(void *ptr, U64 size)
{
    free(ptr);
}
#endif

#endif /* LINUX_OS_CORE_C */
