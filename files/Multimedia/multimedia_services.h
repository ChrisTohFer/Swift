#pragma once

#include "window.h"

#include "Console/console.h"

namespace SWIFT
{

    class WINDOW_SERVICE
    {
        SWIFT::WINDOW m_window;

    public:
        WINDOW_SERVICE();
        ~WINDOW_SERVICE();

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
        VECTOR2F size();
        INPUT& input();
    };
}