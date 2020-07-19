#include "render_scene.h"

#include "internal_types.h"

#include "SFML/Graphics.hpp"

namespace
{
    sf::Vector2f convert(SWIFT::VECTOR2F vec)
    {
        return sf::Vector2f(vec.x, vec.y);
    }
}

SWIFT::SHAPE::SHAPE(VECTOR2F const& pos, VECTOR2F const& s)
{
    m_position = pos;
    m_size = s;
}

SWIFT::VECTOR2F SWIFT::SHAPE::position() const
{
    return m_position;
}

SWIFT::VECTOR2F SWIFT::SHAPE::centre() const
{
    return m_position + m_size / 2.f;
}

SWIFT::VECTOR2F SWIFT::SHAPE::size() const
{
    return m_size;
}

void SWIFT::SHAPE::position(VECTOR2F const& pos)
{
    m_position = pos;
}

void SWIFT::SHAPE::centre(VECTOR2F const& cen)
{
    m_position = cen - m_size / 2;
}

void SWIFT::SHAPE::size(VECTOR2F const& s)
{
    m_size = s;
}

SWIFT::RECT::RECT(VECTOR2F const& pos, VECTOR2F const& s)
    : SHAPE(pos, s)
{}

void SWIFT::RECT::draw(BACKEND_WINDOW& window)
{
    sf::RectangleShape rect(convert(m_size));
    rect.setPosition(convert(m_position));

    window.draw(rect);
}

void SWIFT::RENDER_SCENE::addRectangle(RECT const& rect)
{
    m_rectangles.push_back(rect);
}

void SWIFT::RENDER_SCENE::draw(BACKEND_WINDOW& window)
{
    for (RECT& rect : m_rectangles)
    {
        rect.draw(window);
    }

    m_drawn = true;
}

bool SWIFT::RENDER_SCENE::drawn() const
{
    return m_drawn;
}
