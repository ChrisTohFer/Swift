#pragma once

#include "Types/vector.h"
#include <vector>

namespace SWIFT
{
    struct BACKEND_WINDOW;

    class SHAPE
    {
    public:
        SHAPE(VECTOR2F const& position, VECTOR2F const& size);

        virtual void draw(BACKEND_WINDOW&) = 0;

        VECTOR2F position() const;
        VECTOR2F centre() const;
        VECTOR2F size() const;
        //transform
        //colour or texturing information

        void position(VECTOR2F const&);
        void centre(VECTOR2F const&);
        void size(VECTOR2F const&);
        //transform
        //colour or texturing information

    protected:
        VECTOR2F m_position;
        VECTOR2F m_size;
        //transform
        //colour or texturing information
    };

    class RECT : public SHAPE
    {
    public:
        RECT(VECTOR2F const& pos, VECTOR2F const& size);

        void draw(BACKEND_WINDOW&) override;
    };

    class RENDER_SCENE
    {
    public:
        void addRectangle(RECT const&);
        void draw(BACKEND_WINDOW&);

        bool drawn() const;

    private:
        std::vector<RECT> m_rectangles;
        bool              m_drawn = false;
    };

}