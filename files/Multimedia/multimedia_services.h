#pragma once

#include "window.h"

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
        ~WINDOW_SERVICE()
        {
            console().remove_console_command(L"fullscreen");
            console().remove_console_command(L"window");
        }

        template<typename SCENE>
        void start(SCENE&)
        {

        }
        
        template<typename SCENE>
        void update(SCENE&)
        {
            m_window.update();
        }

        //Actions
        void add_renderer(RENDER_INTERFACE& renderer)
        {
            m_window.add_renderer(renderer);
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
}