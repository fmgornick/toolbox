#ifndef BASE_MATH_C
#define BASE_MATH_C

internal U64
range_u64_len(RangeU64 range)
{
    U64 result = 0;
    if (range.max > range.min)
    {
        result = range.max - range.min;
    }
    return result;
}

#endif /* BASE_MATH_C */
