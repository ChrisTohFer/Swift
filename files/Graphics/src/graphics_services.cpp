#include "graphics_services.h"

SWIFT::CAMERA& SWIFT::MAIN_CAMERA::camera()
{
    return m_camera;
}

void SWIFT::RENDER_SYSTEM::clear_and_reserve(size_t capacity)
{
    m_render_scene.clear_and_reserve(capacity);
}

void SWIFT::RENDER_SYSTEM::add_object(UP_RENDER_OBJECT&& obj)
{
    m_render_scene.add_object(std::move(obj));
}