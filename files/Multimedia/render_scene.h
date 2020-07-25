#pragma once

#include "render_object.h"
#include <map>

namespace SWIFT
{
    struct BACKEND_WINDOW;
    
    class RENDER_SCENE
    {
        using RENDER_MAP = std::map<int, RENDER_OBJECT*>;

    public:
        void add_object(int, RENDER_OBJECT&);
        void remove_object(int);

        void draw(BACKEND_WINDOW&);
        bool drawn() const;

    private:
        RENDER_MAP m_objects;
        bool       m_drawn = false;
    };

}