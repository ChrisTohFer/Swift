#include "transform.h"

#include <math.h>

SWIFT::MATRIX3X3 SWIFT::TRANSFORM::matrix() const
{
    MATRIX3X3 apply_scale(
        scale.x, 0.f, 0.f,
        0.f, scale.y, 0.f,
        0.f, 0.f, 1.f
    );
    MATRIX3X3 apply_rotation(
        cosf(rotation), -sinf(rotation), 0.f,
        sinf(rotation), cosf(rotation), 0.f,
        0.f, 0.f, 1.f
    );
    MATRIX3X3 translate_back(
        1.f, 0.f, position.x,
        0.f, 1.f, position.y,
        0.f, 0.f, 1.f
    );
    
    return (translate_back * (apply_rotation * apply_scale));
}
