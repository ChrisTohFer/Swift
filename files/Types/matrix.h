#pragma once

#include "vector.h"

namespace SWIFT
{
    class MATRIX3X3
    {
        float m_values[9];
        
    public:
        MATRIX3X3();
        MATRIX3X3(float, float, float, float, float, float, float, float, float);

        float value(int, int) const;

        VECTOR3F operator*(VECTOR3F const&) const;
        MATRIX3X3 operator*(MATRIX3X3 const&) const;
    };

}
