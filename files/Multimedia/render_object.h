#pragma once

#include "Types/vector.h"

namespace SWIFT
{
    struct BACKEND_VERTEX_ARRAY;

    struct RENDER_OBJECT
    {
        virtual ~RENDER_OBJECT() = default;

        virtual void draw(BACKEND_VERTEX_ARRAY&) = 0;

        int priority = 0;
    };

    struct RECT : public RENDER_OBJECT
    {
        RECT(VECTOR2F const& pos, VECTOR2F const& size);
        virtual ~RECT() = default;

        void draw(BACKEND_VERTEX_ARRAY&) override;

        VECTOR2F pos;
        VECTOR2F size;
    };

    struct CIRCLE : public RENDER_OBJECT
    {
        CIRCLE(VECTOR2F const& pos, float const& radius);
        virtual ~CIRCLE() = default;

        void draw(BACKEND_VERTEX_ARRAY&) override;

        VECTOR2F pos;
        float    radius;
    };

}