#pragma once

#include "vector.h"
#include "matrix.h"

namespace SWIFT
{
    struct TRANSFORM
    {
        VECTOR2F position;
        VECTOR2F scale;
        float    rotation;  //Clockwise, in radians

        MATRIX3X3 matrix() const;
    };
}