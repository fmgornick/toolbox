#ifndef WINDOWS_OS_CORE_C
#define WINDOWS_OS_CORE_C

internal void *
os_memory_reserve(U64 size)
{
    void *result = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
    return result;
}

internal B32
os_memory_commit(void *ptr, U64 size)
{
    B32 result = (VirtualAlloc(ptr, size, MEM_COMMIT, PAGE_READWRITE) != 0);
    return result;
}

internal void
os_memory_decommit(void *ptr, U64 size)
{
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

internal void
os_memory_release(void *ptr, U64 size)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}

#endif // WINDOWS_OS_CORE_C
