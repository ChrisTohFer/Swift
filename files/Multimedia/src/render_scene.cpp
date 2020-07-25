#include "render_scene.h"
#include "internal_types.h"
#include "GlobalHeaders/macros.h"
#include "SFML/Graphics.hpp"

#include <vector>

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
    //Create a sorted vector
    std::vector<RENDER_OBJECT*> sorted_vector;
    sorted_vector.reserve(m_objects.size());
    for (auto& element : m_objects)
    {
        sorted_vector.push_back(element.second);
    }
    std::sort(sorted_vector.begin(), sorted_vector.end(),
        [](auto lhs, auto rhs)
        {
            return lhs->priority < rhs->priority;
        }
    );
    
    //Draw the sorted list in order
    for (auto& object : sorted_vector)
    {
        object->draw(window);
    }

    m_drawn = true;
}

bool SWIFT::RENDER_SCENE::drawn() const
{
    return m_drawn;
}
