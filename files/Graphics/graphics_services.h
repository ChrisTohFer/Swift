#pragma once

#include "render_scene.h"
#include "renderer.h"
#include "camera.h"
#include "graphics_components.h"

#include "ECSystem/system.h"
#include "Multimedia/multimedia_services.h"

namespace SWIFT
{
    class MAIN_CAMERA
    {
        CAMERA m_camera;
        WINDOW_SERVICE* window_service = nullptr;

    public:
        CAMERA& camera();

        template<typename SCENE>
        void start(SCENE& scene)
        {
            window_service = &(scene.template service<WINDOW_SERVICE>());
            auto window_size = window_service->size();
            m_camera.position(window_size / 2.f);
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
        }
    };

    class RENDER_SYSTEM : public SWIFT::EC::SYSTEM<RENDER_SYSTEM, TRANSFORM, RENDER_COMPONENT>
    {
        SWIFT::RENDERER     m_renderer;
        SWIFT::RENDER_SCENE m_render_scene;

        void clear_and_reserve(size_t capacity);
        void add_object(UP_RENDER_OBJECT&& obj);

    public:
        template<typename SCENE>
        void start(SCENE& scene)
        {
            WINDOW_SERVICE& window = scene.template service<WINDOW_SERVICE>();
            window.add_renderer(m_renderer);

            MAIN_CAMERA& camera = scene.template service<MAIN_CAMERA>();
            m_render_scene.camera(camera.camera());
        }
        template<typename SCENE>
        void early_update(SCENE&)
        {
            clear_and_reserve(count());
        }
        template<typename SCENE>
        void late_update(SCENE&)
        {
            m_renderer.update_scene(std::move(m_render_scene));
        }
        template<typename SCENE>
        void update_per_entity(SCENE&, SWIFT::EC::ENTITY_BASE&, SWIFT::TRANSFORM* transform, RENDER_COMPONENT* sprite)
        {
            add_object(sprite->create_object(*transform));
        }
    };
}