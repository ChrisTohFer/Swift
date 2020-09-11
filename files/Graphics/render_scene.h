#pragma once

#include "render_object.h"
#include <memory>
#include <vector>

namespace sf
{
    class RenderWindow;
}

namespace SWIFT
{
    using UP_RENDER_OBJECT = std::unique_ptr<RENDER_OBJECT>;

    class RENDER_SCENE
    {
        using RENDER_VECTOR = std::vector<UP_RENDER_OBJECT>;

    public:
        void add_object(UP_RENDER_OBJECT&&);
        void clear_and_reserve(size_t);

        void draw(sf::RenderWindow&);
        bool drawn() const;

    private:
        RENDER_VECTOR m_objects;
        bool          m_drawn = false;
    };

}