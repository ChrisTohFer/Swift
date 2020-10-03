#pragma once

#include "render_object.h"
#include "camera.h"
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

        const CAMERA* camera() const;
        void camera(CAMERA const&);

    private:
        const CAMERA* m_main_camera;
        RENDER_VECTOR m_objects;
        bool          m_drawn = false;
    };

}