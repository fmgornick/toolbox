#ifndef BASE_MATH_C
#define BASE_MATH_C

internal Vec2F32
vec_2f32(F32 x, F32 y)
{
    Vec2F32 result = {0};
    result.v[0] = x;
    result.v[1] = y;
    return result;
}

internal Vec2F32
add_2f32(Vec2F32 u, Vec2F32 v)
{
    Vec2F32 result = {0};
    result.v[0] = u.v[0] + v.v[0];
    result.v[1] = u.v[1] + v.v[1];
    return result;
}

internal Vec2F32
sub_2f32(Vec2F32 u, Vec2F32 v)
{
    Vec2F32 result = {0};
    result.v[0] = u.v[0] - v.v[0];
    result.v[1] = u.v[1] - v.v[1];
    return result;
}

internal Vec2F32
mul_2f32(Vec2F32 u, Vec2F32 v)
{
    Vec2F32 result = {0};
    result.v[0] = u.v[0] * v.v[0];
    result.v[1] = u.v[1] * v.v[1];
    return result;
}

internal Vec2F32
div_2f32(Vec2F32 u, Vec2F32 v)
{
    Vec2F32 result = {0};
    result.v[0] = u.v[0] / v.v[0];
    result.v[1] = u.v[1] / v.v[1];
    return result;
}

internal Vec2F32
scale_2f32(Vec2F32 v, F32 s)
{
    Vec2F32 result = {0};
    result.v[0] = v.v[0] * s;
    result.v[1] = v.v[1] * s;
    return result;
}

internal F32
dot_2f32(Vec2F32 u, Vec2F32 v)
{
    F32 result;
    F32 uxvx = u.v[0] * v.v[0];
    F32 uyvy = u.v[1] * v.v[1];
    result = uxvx + uyvy;
    return result;
}

internal F32
length_squared_2f32(Vec2F32 v)
{
    F32 result;
    F32 vxvx = v.v[0] * v.v[0];
    F32 vyvy = v.v[1] * v.v[1];
    result = vxvx + vyvy;
    return result;
}

internal F32
length_2f32(Vec2F32 v)
{
    F32 result;
    F32 vxvx = v.v[0] * v.v[0];
    F32 vyvy = v.v[1] * v.v[1];
    result = sqrt_f32(vxvx + vyvy);
    return result;
}

internal Vec2F32
normalize_2f32(Vec2F32 v)
{
    Vec2F32 result;
    F32 length;
    F32 vxvx = v.v[0] * v.v[0];
    F32 vyvy = v.v[1] * v.v[1];
    length = sqrt_f32(vxvx + vyvy);
    result.v[0] = v.v[0] / length;
    result.v[1] = v.v[1] / length;
    return result;
}

internal Vec2F64
vec_2f64(F64 x, F64 y)
{
    Vec2F64 result = {0};
    result.v[0] = x;
    result.v[1] = y;
    return result;
}

internal Vec2F64
add_2f64(Vec2F64 u, Vec2F64 v)
{
    Vec2F64 result = {0};
    result.v[0] = u.v[0] + v.v[0];
    result.v[1] = u.v[1] + v.v[1];
    return result;
}

internal Vec2F64
sub_2f64(Vec2F64 u, Vec2F64 v)
{
    Vec2F64 result = {0};
    result.v[0] = u.v[0] - v.v[0];
    result.v[1] = u.v[1] - v.v[1];
    return result;
}

internal Vec2F64
mul_2f64(Vec2F64 u, Vec2F64 v)
{
    Vec2F64 result = {0};
    result.v[0] = u.v[0] * v.v[0];
    result.v[1] = u.v[1] * v.v[1];
    return result;
}

internal Vec2F64
div_2f64(Vec2F64 u, Vec2F64 v)
{
    Vec2F64 result = {0};
    result.v[0] = u.v[0] / v.v[0];
    result.v[1] = u.v[1] / v.v[1];
    return result;
}

internal Vec2F64
scale_2f64(Vec2F64 v, F64 s)
{
    Vec2F64 result = {0};
    result.v[0] = v.v[0] * s;
    result.v[1] = v.v[1] * s;
    return result;
}

internal F64
dot_2f64(Vec2F64 u, Vec2F64 v)
{
    F64 result;
    F64 uxvx = u.v[0] * v.v[0];
    F64 uyvy = u.v[1] * v.v[1];
    result = uxvx + uyvy;
    return result;
}

internal F64
length_squared_2f64(Vec2F64 v)
{
    F64 result;
    F64 vxvx = v.v[0] * v.v[0];
    F64 vyvy = v.v[1] * v.v[1];
    result = vxvx + vyvy;
    return result;
}

internal F64
length_2f64(Vec2F64 v)
{
    F64 result;
    F64 vxvx = v.v[0] * v.v[0];
    F64 vyvy = v.v[1] * v.v[1];
    result = sqrt_f64(vxvx + vyvy);
    return result;
}

internal Vec2F64
normalize_2f64(Vec2F64 v)
{
    Vec2F64 result;
    F64 length;
    F64 vxvx = v.v[0] * v.v[0];
    F64 vyvy = v.v[1] * v.v[1];
    length = sqrt_f64(vxvx + vyvy);
    result.v[0] = v.v[0] / length;
    result.v[1] = v.v[1] / length;
    return result;
}

#endif /* BASE_MATH_C */
