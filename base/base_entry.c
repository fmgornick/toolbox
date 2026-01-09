#ifndef BASE_ENTRY_C
#define BASE_ENTRY_C

internal void
main_thread_entry(int argc, char **argv)
{
    NotImplemented;
}

internal void
worker_thread_entry(void (*entry_point)(void *params), void *params)
{
    NotImplemented;
}

#endif // BASE_ENTRY_C
