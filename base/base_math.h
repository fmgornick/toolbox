#ifndef BASE_MATH_H
#define BASE_MATH_H

#include <math.h>

/* clang-format off */
typedef struct { F32 v[2]; } Vec2F32;
typedef struct { F64 v[2]; } Vec2F64;
typedef struct { F32 v[3]; } Vec3F32;
typedef struct { F64 v[3]; } Vec3F64;
typedef struct { F32 v[4]; } Vec4F32;
typedef struct { F64 v[4]; } Vec4F64;

typedef struct { S32 v[2]; } Vec2S32;
typedef struct { S64 v[2]; } Vec2S64;
typedef struct { S32 v[3]; } Vec3S32;
typedef struct { S64 v[3]; } Vec3S64;
typedef struct { S32 v[4]; } Vec4S32;
typedef struct { S64 v[4]; } Vec4S64;

typedef struct { F32 v[2][2]; } Mat2x2F32;
typedef struct { F64 v[2][2]; } Mat2x2F64;
typedef struct { F32 v[3][3]; } Mat3x3F32;
typedef struct { F64 v[3][3]; } Mat3x3F64;
typedef struct { F32 v[4][4]; } Mat4x4F32;
typedef struct { F64 v[4][4]; } Mat4x4F64;
/* clang-format on */

#define sqrt_f32(x) sqrtf(x)
#define sqrt_f64(x) sqrt(x)

#define v2f32(x, y) vec_2f32((x), (y))
internal Vec2F32 vec_2f32(F32 x, F32 y);
internal Vec2F32 add_2f32(Vec2F32 u, Vec2F32 v);
internal Vec2F32 sub_2f32(Vec2F32 u, Vec2F32 v);
internal Vec2F32 mul_2f32(Vec2F32 u, Vec2F32 v);
internal Vec2F32 div_2f32(Vec2F32 u, Vec2F32 v);
internal Vec2F32 scale_2f32(Vec2F32 v, F32 s);
internal F32 dot_2f32(Vec2F32 u, Vec2F32 v);
internal F32 length_squared_2f32(Vec2F32 v);
internal F32 length_2f32(Vec2F32 v);
internal Vec2F32 normalize_2f32(Vec2F32 v);
/* internal Vec2F32 mix_2f32(Vec2F32 a, Vec2F32 b, F32 t); */

#endif /* BASE_MATH_H */
