#pragma once

#include "Types/vector.h"

//Forward declaration
namespace sf
{
    class VertexArray;
}

namespace SWIFT
{
    struct RENDER_OBJECT
    {
        virtual ~RENDER_OBJECT() = default;

        virtual void draw(sf::VertexArray&) = 0;

        int priority = 0;
    };

    struct RECT : public RENDER_OBJECT
    {
        RECT(VECTOR2F const& pos, VECTOR2F const& size);
        virtual ~RECT() = default;

        void draw(sf::VertexArray&) override;

        VECTOR2F pos;
        VECTOR2F size;
    };

    struct CIRCLE : public RENDER_OBJECT
    {
        CIRCLE(VECTOR2F const& pos, float const& radius);
        virtual ~CIRCLE() = default;

        void draw(sf::VertexArray&) override;

        VECTOR2F pos;
        float    radius;
    };

}