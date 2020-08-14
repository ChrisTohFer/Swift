#include "Multimedia/window.h"

#include "Types/event.h"
#include "Console/console.h"
#include "ECSystem/entity.h"

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
int main()
{
    SWIFT::EC::ENTITY<type1, type2, type3> ent;
    
    if(ent.has_components<type2, type1>())
        std::cout << "Has 1 and 2\n";
    if(ent.has_components<type1, type2, type3>())
        std::cout << "1 2 and 3\n";
    if(ent.has_components<type3, type4>())
        std::cout << "Has 3 and 4\n";
    if(ent.has_component<type3>())
        std::cout << "Has 3\n";

    std::cout << ent.component<type1>().a << "\n";


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