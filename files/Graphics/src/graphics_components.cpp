#include "graphics_components.h"


SWIFT::UP_RENDER_OBJECT SWIFT::SPRITE_COMPONENT::create_object(TRANSFORM const& transform)
{
    return UP_RENDER_OBJECT(new SPRITE(transform, size, pivot, colour));
}