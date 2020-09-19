#include "render_object.h"

#include "SFML/Graphics.hpp"

namespace
{
    sf::Vector2f convert(SWIFT::VECTOR2F const& vec)
    {
        return sf::Vector2f(vec.x, vec.y);
    }
    sf::Vector2f convert(SWIFT::VECTOR3F const& vec)
    {
        return sf::Vector2f(vec.x, vec.y);
    }
    sf::Color convert(SWIFT::COLOUR const& c)
    {
        return sf::Color(c.r, c.g, c.b, c.a);
    }
}

SWIFT::RENDER_OBJECT::RENDER_OBJECT(TRANSFORM const& t)
    : transform(t)
{
}

SWIFT::SPRITE::SPRITE(TRANSFORM const& transform, VECTOR2F const& size, VECTOR2F const& pivot, COLOUR colour)
    : RENDER_OBJECT(transform), size(size), pivot(pivot), colour(colour)
{}

void SWIFT::SPRITE::draw(sf::VertexArray& vertices, int index, MATRIX3X3 const& camera_transform_matrix)
{
    auto complete_transform = camera_transform_matrix * transform.matrix();

    //offset from pivot - 0,0 pivot means top left corner is at position
    auto offset = SWIFT::VECTOR3F(-size.x * pivot.x, -size.y * pivot.y, 0.f);

    auto p1 = complete_transform * (offset + SWIFT::VECTOR3F(0.f, 0.f, 1.f));
    auto p2 = complete_transform * (offset + SWIFT::VECTOR3F(size.x, 0.f, 1.f));
    auto p3 = complete_transform * (offset + SWIFT::VECTOR3F(size.x, size.y, 1.f));
    auto p4 = complete_transform * (offset + SWIFT::VECTOR3F(0.f, size.y, 1.f));

    vertices[index    ].position = convert(p1);
    vertices[index + 1].position = convert(p2);
    vertices[index + 2].position = convert(p3);
    vertices[index + 3].position = convert(p4);

    vertices[index    ].color = convert(colour);
    vertices[index + 1].color = convert(colour);
    vertices[index + 2].color = convert(colour);
    vertices[index + 3].color = convert(colour);
}
