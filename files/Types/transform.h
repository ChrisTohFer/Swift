#pragma once

#include "vector.h"
#include "matrix.h"

namespace SWIFT
{
    struct TRANSFORM
    {
        VECTOR2F position   = VECTOR2F::zero();
        VECTOR2F scale      = VECTOR2F(1.f, 1.f);
        float    rotation   = 0;  //Clockwise, in radians

        MATRIX3X3 matrix() const;
    };
}