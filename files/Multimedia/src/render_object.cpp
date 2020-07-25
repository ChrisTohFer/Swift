#include "render_object.h"
#include "internal_types.h"

namespace
{
    sf::Vector2f convert(SWIFT::VECTOR2F vec)
    {
        return sf::Vector2f(vec.x, vec.y);
    }
}

bool SWIFT::RENDER_OBJECT::operator<(RENDER_OBJECT const& rhs) const
{
    return priority < rhs.priority;
}

SWIFT::RECT::RECT(VECTOR2F const& pos, VECTOR2F const& size)
    : pos(pos), size(size)
{}

void SWIFT::RECT::draw(BACKEND_WINDOW& window)
{
    sf::RectangleShape rect(convert(size));
    rect.setPosition(convert(pos));

    window.draw(rect);
}

SWIFT::CIRCLE::CIRCLE(VECTOR2F const& pos, float const& radius)
    : pos(pos), radius(radius)
{}

void SWIFT::CIRCLE::draw(BACKEND_WINDOW& window)
{
    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setPosition(convert(pos));

    window.draw(circle);
}