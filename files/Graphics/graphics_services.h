#pragma once

#include "render_scene.h"
#include "renderer.h"
#include "camera.h"

#include "Multimedia/multimedia_services.h"

namespace SWIFT
{
    class MAIN_CAMERA
    {
        CAMERA m_camera;
        WINDOW_SERVICE* window_service;

    public:
        CAMERA& camera()
        {
            return m_camera;
        }

        template<typename SCENE>
        void start(SCENE& scene)
        {
            window_service = &(scene.template service<WINDOW_SERVICE>());
            auto window_size = window_service->size();
            m_camera.aspect_ratio(window_size.x / window_size.y);
            m_camera.height(window_size.y);
        }

        template<typename SCENE>
        void update(SCENE&)
        {
            auto window_size = window_service->size();
            m_camera.position(window_size / 2.f);
            m_camera.aspect_ratio(window_size.x / window_size.y);
            m_camera.height(window_size.y);
            m_camera.rotation(m_camera.rotation() + 0.01f);
        }
    };

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

            MAIN_CAMERA& camera = scene.template service<MAIN_CAMERA>();
            m_render_scene.camera(camera.camera());
        }

        template<typename SCENE>
        void update(SCENE&)
        {
            m_renderer.update_scene(std::move(m_render_scene));
        }
    };
}