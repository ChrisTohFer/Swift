#pragma once

#include "window.h"
#include "render_scene.h"

#include "Console/console.h"

namespace SWIFT
{

    class WINDOW_SERVICE
    {
        SWIFT::WINDOW window;

    public:
        WINDOW_SERVICE()
        {
            using SWIFT::console;
            
            window.title(L"This is not the title");
            window.create_fullscreen();
            console().add_console_command(L"fullscreen", L"Make window fullscreen.", window, &SWIFT::WINDOW::create_fullscreen);
            console().add_console_command(L"window", L"Takes width and height as integers and creates a window of that size.", window, &SWIFT::WINDOW::create_window);
        }

        void update_window(SWIFT::RENDER_SCENE&& rscene)
        {
            window.update(std::move(rscene));
        }

        template<typename SCENE>
        void update(SCENE&)
        {

        }

        //Queries
        VECTOR2F size()
        {
            return window.size();
        }
    };

    class RENDER_SERVICE
    {
        SWIFT::RENDER_SCENE render_scene;

    public:
        void clear_and_reserve(size_t capacity)
        {
            render_scene.clear_and_reserve(capacity);
        }
        void add_object(UP_RENDER_OBJECT&& obj)
        {
            render_scene.add_object(std::move(obj));
        }

        template<typename SCENE>
        void update(SCENE& scene)
        {
            scene.template service<WINDOW_SERVICE>().update_window(std::move(render_scene));
        }
    };
}