#ifndef BASE_MATH_H
#define BASE_MATH_H

typedef struct RangeU64 {
    U64 min;
    U64 max;
} RangeU64;

internal U64 range_u64_len(RangeU64 range);

#endif /* BASE_MATH_H */
