#pragma once

#include "Types/vector.h"
#include "Types/matrix.h"

namespace SWIFT
{
    class CAMERA
    {
        VECTOR2F m_position;
        VECTOR2F m_size;
        float    m_rotation;  //Clockwise, in radians
        float    m_aspect_ratio;

    public:
        VECTOR2F position()  const;
        VECTOR2F size()      const;
        float aspect_ratio() const;
        float height()       const;
        float width()        const;
        float rotation()     const;

        void position(VECTOR2F const&);
        void aspect_ratio(float);
        void height(float);
        void width(float);
        void rotation(float);

        MATRIX3X3 matrix()   const;
    };
}