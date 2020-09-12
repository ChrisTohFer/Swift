#include "camera.h"

#include <math.h>

SWIFT::VECTOR2F SWIFT::CAMERA::position() const
{
    return m_position;
}

SWIFT::VECTOR2F SWIFT::CAMERA::size() const
{
    return m_size;
}

float SWIFT::CAMERA::aspect_ratio() const
{
    return m_aspect_ratio;
}

float SWIFT::CAMERA::height() const
{
    return m_size.y;
}

float SWIFT::CAMERA::width() const
{
    return m_size.x;
}

void SWIFT::CAMERA::position(VECTOR2F const& pos)
{
    m_position = pos;
}

void SWIFT::CAMERA::aspect_ratio(float ratio)
{
    m_aspect_ratio = ratio;
    m_size.x = m_size.y * ratio;
}

void SWIFT::CAMERA::height(float height)
{
    m_size.y = height;
    m_size.x = height * m_aspect_ratio;
}

void SWIFT::CAMERA::width(float width)
{
    m_size.x = width;
    m_size.y = width / m_aspect_ratio;
}

SWIFT::MATRIX3X3 SWIFT::CAMERA::matrix() const
{
    MATRIX3X3 apply_translation(
        1.f, 0.f, -m_position.x,
        0.f, 1.f, -m_position.y,
        0.f, 0.f, 1.f
    );
    MATRIX3X3 apply_rotation(
        cosf(-m_rotation), -sinf(-m_rotation), 0.f,
        sinf(-m_rotation), cosf(-m_rotation), 0.f,
        0.f, 0.f, 1.f
    );
    MATRIX3X3 apply_size(
        1.f/m_size.x, 0.f, 0.f,
        0.f, 1.f/m_size.y, 0.f,
        0.f, 0.f, 1.f
    );

    return (apply_size * (apply_rotation * apply_translation));
}
