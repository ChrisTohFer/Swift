#include "render_object.h"

#include "SFML/Graphics.hpp"

namespace
{
    sf::Vector2f convert(SWIFT::VECTOR2F vec)
    {
        return sf::Vector2f(vec.x, vec.y);
    }
    sf::Vector2f convert(SWIFT::VECTOR3F vec)
    {
        return sf::Vector2f(vec.x, vec.y);
    }
}

SWIFT::RENDER_OBJECT::RENDER_OBJECT(TRANSFORM const& t)
    : transform(t)
{
}

SWIFT::RECT::RECT(TRANSFORM const& t, VECTOR2F const& size)
    : RENDER_OBJECT(t), size(size)
{}

void SWIFT::RECT::draw(sf::VertexArray& vertices, int index, MATRIX3X3 const& camera_transform_matrix)
{
    auto complete_transform = camera_transform_matrix * transform.matrix();

    auto p1 = complete_transform * SWIFT::VECTOR3F(-size.x / 2.f, -size.y / 2.f, 1.f);
    auto p2 = complete_transform * SWIFT::VECTOR3F(size.x / 2.f, -size.y / 2.f, 1.f);
    auto p3 = complete_transform * SWIFT::VECTOR3F(size.x / 2.f, size.y / 2.f, 1.f);
    auto p4 = complete_transform * SWIFT::VECTOR3F(-size.x / 2.f, size.y / 2.f, 1.f);

    vertices[index    ].position = convert(p1);
    vertices[index + 1].position = convert(p2);
    vertices[index + 2].position = convert(p3);
    vertices[index + 3].position = convert(p4);
}
