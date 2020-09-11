#pragma once

//Forward declarations
namespace sf
{
    class RenderWindow;
}

namespace SWIFT
{

    class RENDER_INTERFACE
    {
    public:
        virtual void add_parent_window(sf::RenderWindow&)    = 0;
        virtual void remove_parent_window()                  = 0;
    };

}