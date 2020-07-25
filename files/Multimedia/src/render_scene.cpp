#include "render_scene.h"
#include "internal_types.h"
#include "GlobalHeaders/macros.h"
#include "SFML/Graphics.hpp"

void SWIFT::RENDER_SCENE::add_object(int id, RENDER_OBJECT& obj)
{
    ASSERT(m_objects.find(id) == m_objects.end());  //We mustn't use the same id for two objects at once

    m_objects.emplace(id, &obj);
}

void SWIFT::RENDER_SCENE::remove_object(int id)
{
    m_objects.erase(id);
}

void SWIFT::RENDER_SCENE::draw(BACKEND_WINDOW& window)
{
    for (auto& element : m_objects)
    {
        element.second->draw(window);
    }

    m_drawn = true;
}

bool SWIFT::RENDER_SCENE::drawn() const
{
    return m_drawn;
}
