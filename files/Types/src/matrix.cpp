#include "matrix.h"

SWIFT::MATRIX3X3::MATRIX3X3()
    : m_values{}
{}

SWIFT::MATRIX3X3::MATRIX3X3(
    float v11, float v12, float v13,
    float v21, float v22, float v23,
    float v31, float v32, float v33
)
    : m_values{
    v11, v12, v13,
    v21, v22, v23,
    v31, v32, v33
}
{}

float SWIFT::MATRIX3X3::value(int row, int column) const
{
    return m_values[3*row + column];
}

SWIFT::VECTOR3F SWIFT::MATRIX3X3::operator*(VECTOR3F const& vector) const
{
    return VECTOR3F(
        m_values[0] * vector.x + m_values[1] * vector.y + m_values[2] * vector.z,
        m_values[3] * vector.x + m_values[4] * vector.y + m_values[5] * vector.z,
        m_values[6] * vector.x + m_values[7] * vector.y + m_values[8] * vector.z
    );
}

SWIFT::MATRIX3X3 SWIFT::MATRIX3X3::operator*(MATRIX3X3 const& other) const
{
    return MATRIX3X3(
        value(0,0) * other.value(0,0) + value(0,1) * other.value(1,0) + value(0,2) * other.value(2,0),
        value(0,0) * other.value(0,1) + value(0,1) * other.value(1,1) + value(0,2) * other.value(2,1),
        value(0,0) * other.value(0,2) + value(0,1) * other.value(1,2) + value(0,2) * other.value(2,2),
        
        value(1,0) * other.value(0,0) + value(1,1) * other.value(1,0) + value(1,2) * other.value(2,0),
        value(1,0) * other.value(0,1) + value(1,1) * other.value(1,1) + value(1,2) * other.value(2,1),
        value(1,0) * other.value(0,2) + value(1,1) * other.value(1,2) + value(1,2) * other.value(2,2),
        
        value(2,0) * other.value(0,0) + value(2,1) * other.value(1,0) + value(2,2) * other.value(2,0),
        value(2,0) * other.value(0,1) + value(2,1) * other.value(1,1) + value(2,2) * other.value(2,1),
        value(2,0) * other.value(0,2) + value(2,1) * other.value(1,2) + value(2,2) * other.value(2,2)
    );
}
