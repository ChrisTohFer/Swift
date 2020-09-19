#pragma once

#include "Types/vector.h"
#include "Types/colour.h"

#include "render_object.h"
#include "render_scene.h"

namespace SWIFT
{
    struct RENDER_COMPONENT
    {
        virtual UP_RENDER_OBJECT create_object(TRANSFORM const&) = 0;
    };

    struct SPRITE_COMPONENT : public RENDER_COMPONENT
    {
        VECTOR2F size;
        VECTOR2F pivot = VECTOR2F(0.5f, 0.5f); //As a fraction of size
        COLOUR   colour = COLOURS::white;

        UP_RENDER_OBJECT create_object(TRANSFORM const&) override;
    };
}