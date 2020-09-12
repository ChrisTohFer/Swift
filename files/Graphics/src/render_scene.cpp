#include "render_scene.h"
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

void SWIFT::RENDER_SCENE::draw(sf::RenderWindow& window)
{
    //Sort vector
    std::sort(m_objects.begin(), m_objects.end(),
        [](auto const& lhs, auto const& rhs)
        {
            return lhs->priority < rhs->priority;
        }
    );
    
    auto camera_transform_matrix = m_main_camera->matrix();

    //Draw the sorted vector in order
    sf::VertexArray vertices;
    vertices.resize(m_objects.size() * 4);
    vertices.setPrimitiveType(sf::PrimitiveType::Quads);
    for (int i = 0; i < int(m_objects.size()); ++i)
    {
        auto& object = m_objects[i];
        object->draw(vertices, i * 4, camera_transform_matrix);
    }

    window.draw(vertices);

    m_drawn = true;
}

bool SWIFT::RENDER_SCENE::drawn() const
{
    return m_drawn;
}

const SWIFT::CAMERA* SWIFT::RENDER_SCENE::camera() const
{
    return m_main_camera;
}

void SWIFT::RENDER_SCENE::camera(CAMERA const& camera)
{
    m_main_camera = &camera;
}
