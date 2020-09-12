#pragma once

#include "Types/vector.h"
#include "Types/transform.h"

//Forward declaration
namespace sf
{
    class VertexArray;
}

namespace SWIFT
{
    struct RENDER_OBJECT
    {
        RENDER_OBJECT(TRANSFORM const&);
        virtual ~RENDER_OBJECT() = default;

        virtual void draw(sf::VertexArray&, int index, MATRIX3X3 const& camera_transform_matrix) = 0;

        TRANSFORM transform;
        int       priority = 0;
    };

    struct RECT : public RENDER_OBJECT
    {
        RECT(TRANSFORM const& transform, VECTOR2F const& size);
        virtual ~RECT() = default;

        void draw(sf::VertexArray&, int index, MATRIX3X3 const& camera_transform_matrix) override;

        VECTOR2F size;
    };

}