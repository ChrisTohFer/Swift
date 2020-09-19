#pragma once

#include "Types/vector.h"
#include "Types/transform.h"
#include "Types/colour.h"

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

    struct SPRITE : public RENDER_OBJECT
    {
        SPRITE(TRANSFORM const& transform, VECTOR2F const& size, VECTOR2F const& pivot, COLOUR colour);
        virtual ~SPRITE() = default;

        void draw(sf::VertexArray&, int index, MATRIX3X3 const& camera_transform_matrix) override;

        VECTOR2F size;
        VECTOR2F pivot = VECTOR2F(0.5f, 0.5f); //As a fraction of size
        COLOUR   colour = COLOURS::white;
    };

}