#ifndef BASE_CORE_C
#define BASE_CORE_C

internal B32
memory_is_zero(void *ptr, U64 size)
{
    U64 octcount = (size >> 3);
    U64 excess = (size & 0x7);
    U64 *p64 = (U64 *)ptr;
    for (U64 i = 0; i < octcount; i += 1, p64 += 1)
    {
        if (*p64 != 0)
        {
            return 0;
        }
    }
    U8 *p8 = (U8 *)p64;
    for (U64 i = 0; i < excess; i += 1, p8 += 1)
    {
        if (*p8 != 0)
        {
            return 0;
        }
    }
    return 1;
}

#endif // BASE_CORE_C
