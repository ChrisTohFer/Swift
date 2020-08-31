#pragma once

#include "window.h"
#include "render_scene.h"

#include "Console/console.h"

namespace SWIFT
{

    class WINDOW_SERVICE
    {
        SWIFT::WINDOW m_window;

    public:
        WINDOW_SERVICE()
        {
            using SWIFT::console;
            
            m_window.title(L"This is not the title");
            m_window.create_fullscreen();
            console().add_console_command(L"fullscreen", L"Make window fullscreen.", m_window, &SWIFT::WINDOW::create_fullscreen);
            console().add_console_command(L"window", L"Takes width and height as integers and creates a window of that size.", m_window, &SWIFT::WINDOW::create_window);
        }

        void update_window(SWIFT::RENDER_SCENE&& rscene)
        {
            m_window.update(std::move(rscene));
        }

        template<typename SCENE>
        void start(SCENE&)
        {

        }
        
        template<typename SCENE>
        void update(SCENE&)
        {

        }

        //Queries
        VECTOR2F size()
        {
            return m_window.size();
        }
        INPUT& input()
        {
            return m_window.input();
        }
    };

    class RENDER_SERVICE
    {
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
        void start(SCENE&)
        {

        }

        template<typename SCENE>
        void update(SCENE& scene)
        {
            scene.template service<WINDOW_SERVICE>().update_window(std::move(m_render_scene));
        }
    };
}