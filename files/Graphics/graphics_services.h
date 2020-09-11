#pragma once

#include "render_scene.h"
#include "renderer.h"

#include "Multimedia/multimedia_services.h"

namespace SWIFT
{
    class RENDER_SERVICE
    {
        SWIFT::RENDERER     m_renderer;
        SWIFT::RENDER_SCENE m_render_scene;

    public:
        void clear_and_reserve(size_t capacity)
        {
            m_render_scene.clear_and_reserve(capacity);
        }
        void add_object(UP_RENDER_OBJECT&& obj)
        {
            m_render_scene.add_object(std::move(obj));
        }

        template<typename SCENE>
        void start(SCENE& scene)
        {
            WINDOW_SERVICE& window = scene.template service<WINDOW_SERVICE>();
            window.add_renderer(m_renderer);
        }

        template<typename SCENE>
        void update(SCENE&)
        {
            m_renderer.update_scene(std::move(m_render_scene));
        }
    };
}