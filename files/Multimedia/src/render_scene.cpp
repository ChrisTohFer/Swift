#include "render_scene.h"
#include "internal_types.h"
#include "SFML/Graphics.hpp"

void SWIFT::RENDER_SCENE::add_object(UP_RENDER_OBJECT&& ptr)
{
    m_objects.push_back(std::move(ptr));
}

void SWIFT::RENDER_SCENE::clear_and_reserve(size_t capacity)
{
    m_objects.clear();
    m_objects.reserve(capacity);
}

void SWIFT::RENDER_SCENE::draw(BACKEND_WINDOW& window)
{
    //Sort vector
    std::sort(m_objects.begin(), m_objects.end(),
        [](auto const& lhs, auto const& rhs)
        {
            return lhs->priority < rhs->priority;
        }
    );
    
    //Draw the sorted vector in order
    BACKEND_VERTEX_ARRAY vertices;
    vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    for (auto& object : m_objects)
    {
        object->draw(vertices);
    }

    window.draw(vertices);

    m_drawn = true;
}

bool SWIFT::RENDER_SCENE::drawn() const
{
    return m_drawn;
}
