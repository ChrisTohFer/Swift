#pragma once

#include "Types/vector.h"

namespace SWIFT
{
    struct BACKEND_WINDOW;

    struct RENDER_OBJECT
    {
        virtual void draw(BACKEND_WINDOW&) = 0;

        int priority = 0;
    };

    struct RECT : public RENDER_OBJECT
    {
        RECT(VECTOR2F const& pos, VECTOR2F const& size);

        void draw(BACKEND_WINDOW&) override;

        VECTOR2F pos;
        VECTOR2F size;
    };

    struct CIRCLE : public RENDER_OBJECT
    {
        CIRCLE(VECTOR2F const& pos, float const& radius);

        void draw(BACKEND_WINDOW&) override;

        VECTOR2F pos;
        float    radius;
    };

}