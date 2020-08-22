#include "Multimedia/window.h"

#include "Types/event.h"
#include "Console/console.h"
#include "ECSystem/entity.h"
#include "ECSystem/entity_holder.h"
#include "ECSystem/system.h"
#include "ECSystem/system_holder.h"
#include "ECSystem/scene.h"

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

int g_id = 0;

static bool& running()
{
    static bool r = true;
    return r;
}
static void stop_running()
{
    running() = false;
}

class RECT_LISTENER : public SWIFT::INPUT::MOUSE_LISTENER
{
public:
    RECT_LISTENER(SWIFT::RENDER_SCENE& scene)
        : m_scene(scene)
        , m_pos1()
        , m_held(false)
    {}

    virtual void notify(SWIFT::MOUSE_EVENT event)
    {
        if (event.button != SWIFT::MOUSE_BUTTON::LEFT)
            return;

        if(event.pressed)
        { 
            m_held = true;
            m_pos1 = event.pos();
        }

        if (!event.pressed && m_held)
        {
            m_held = false;
            m_vector.push_back(std::make_unique<SWIFT::RECT>(m_pos1, event.pos() - m_pos1));
            m_scene.add_object(++g_id, *m_vector.back());
        }
    }

private:
    SWIFT::RENDER_SCENE& m_scene;
    SWIFT::VECTOR2F m_pos1;
    bool m_held = false;

    std::vector<std::unique_ptr<SWIFT::RECT>> m_vector;
};

class CIRCLE_LISTENER : public SWIFT::INPUT::MOUSE_LISTENER
{
public:
    CIRCLE_LISTENER(SWIFT::RENDER_SCENE& scene)
        : m_scene(scene)
        , m_pos1()
        , m_held(false)
    {}

    virtual void notify(SWIFT::MOUSE_EVENT event)
    {
        if (event.button != SWIFT::MOUSE_BUTTON::RIGHT)
            return;

        if(event.pressed)
        { 
            m_held = true;
            m_pos1 = event.pos();
        }

        if (!event.pressed && m_held)
        {
            m_held = false;

            auto radius = (event.pos() - m_pos1).abs();
            auto position = m_pos1 - SWIFT::VECTOR2F(radius, radius);
            m_vector.push_back(std::make_unique<SWIFT::CIRCLE>(position, radius));
            m_vector.back()->priority = 1;
            m_scene.add_object(++g_id, *m_vector.back());
        }
    }

private:
    SWIFT::RENDER_SCENE& m_scene;
    SWIFT::VECTOR2F m_pos1;
    bool m_held = false;

    std::vector<std::unique_ptr<SWIFT::CIRCLE>> m_vector;
};

struct type1
{
    int a = 1;
};
struct type2
{
    int a = 2;
};
struct type3
{
    int a = 3;
};
struct type4
{
    int a = 4;
};

class SYSTEM1 : public SWIFT::EC::SYSTEM<SYSTEM1, type1>
{
public:
    virtual void update_per_entity(type1& t1)
    {
        std::cout << t1.a << "\n";
    }
};
class SYSTEM2 : public SWIFT::EC::SYSTEM<SYSTEM2, type2>
{
public:
    void update_per_entity(type2& t2)
    {
        std::cout << t2.a << "\n";
    }
};

using ENTITY1 = SWIFT::EC::ENTITY<type1>;
using ENTITY2 = SWIFT::EC::ENTITY<type1, type2>;
using ENTITY3 = SWIFT::EC::ENTITY<type1, type3, type4>;
using HOLDER = SWIFT::EC::ENTITY_HOLDER<ENTITY1, ENTITY2, ENTITY3>;
using SYSTEM_HOLDER = SWIFT::EC::SYSTEM_HOLDER<SYSTEM1, SYSTEM2>;
using SCENE = SWIFT::EC::SCENE<HOLDER, SYSTEM_HOLDER>;

int main()
{
    HOLDER holder;
    SYSTEM_HOLDER sholder;
    SCENE s;
    
    
    s.add_entity(ENTITY1());
    s.add_entity(ENTITY2());
    s.add_entity(ENTITY1());
    s.add_entity(ENTITY2());
    s.add_entity(ENTITY1());
    s.add_entity(ENTITY3());

    s.update();

    using SWIFT::console;

    SWIFT::WINDOW window;
    window.title(L"This is not the title");
    window.create_window(200, 200);

    console().add_console_command(L"exit", L"Exit the application.", &stop_running);
    console().add_console_command(L"fullscreen", L"Make window fullscreen.", window, &SWIFT::WINDOW::create_fullscreen);
    console().add_console_command(L"window", L"Takes width and height as integers and creates a window of that size.", window, &SWIFT::WINDOW::create_window);

    SWIFT::RENDER_SCENE scene;

    RECT_LISTENER rl(scene);
    CIRCLE_LISTENER cl(scene);
    window.input().listen_for_mouse(rl);
    window.input().listen_for_mouse(cl);

    std::wstring input;
    while (running())
    {
        console().invoke_commands();
        window.update(scene);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }
    window.close_window();

    return 0;
}