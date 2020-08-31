#include "render_object.h"
#include "internal_types.h"

namespace
{
    sf::Vector2f convert(SWIFT::VECTOR2F vec)
    {
        return sf::Vector2f(vec.x, vec.y);
    }
}

SWIFT::RECT::RECT(VECTOR2F const& pos, VECTOR2F const& size)
    : pos(pos), size(size)
{}

void SWIFT::RECT::draw(BACKEND_VERTEX_ARRAY& vertices)
{
    sf::RectangleShape rect(convert(size));
    rect.setPosition(convert(pos));

    if (vertices.getPrimitiveType() == sf::PrimitiveType::Quads)
    {
        auto v1 = sf::Vertex(convert(pos));
        auto v2 = sf::Vertex(sf::Vector2(pos.x + 10.f, pos.y));
        auto v3 = sf::Vertex(sf::Vector2(pos.x + 10.f, pos.y + 10.f));
        auto v4 = sf::Vertex(sf::Vector2(pos.x, pos.y + 10.f));
        
        vertices.append(v1);
        vertices.append(v2);
        vertices.append(v3);
        vertices.append(v4);
    }
}

SWIFT::CIRCLE::CIRCLE(VECTOR2F const& pos, float const& radius)
    : pos(pos), radius(radius)
{}

void SWIFT::CIRCLE::draw(BACKEND_VERTEX_ARRAY& vertices)
{
    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setPosition(convert(pos));
    circle.setFillColor(sf::Color::Red);
}