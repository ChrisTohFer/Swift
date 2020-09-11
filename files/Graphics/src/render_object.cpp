#include "render_object.h"
#include "SFML/Graphics.hpp"

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

void SWIFT::RECT::draw(sf::VertexArray& vertices)
{
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

void SWIFT::CIRCLE::draw(sf::VertexArray&)
{
}