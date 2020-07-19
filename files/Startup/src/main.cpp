#include "Multimedia/window.h"

#include <iostream>
#include <string>
#include <sstream>

#include "console.h"

static bool& running()
{
    static bool r = true;
    return r;
}
static void stop_running()
{
    running() = false;
}

class LISTENER : public SWIFT::INPUT::KEY_LISTENER
{
public:
    LISTENER(SWIFT::CONSOLE& console)
        : m_console(console)
    {}
    virtual void notify(SWIFT::KEY key, SWIFT::INPUT::KEY_STATUS const& status) override
    {
        auto str = std::to_wstring((int)key);
        str += L", ";
        str += status.held     ? L"held " : L"";
        str += status.pressed  ? L"pressed " : L"";
        str += status.released ? L"released " : L"";
                               
        str += L", ";
        str += status.alt      ? L"alt " : L"";
        str += status.control  ? L"control " : L"";
        str += status.shift    ? L"shift " : L"";
        str += status.system   ? L"system " : L"";

        m_console.output(str);
        m_console.output(L"\n");
    }
private:
    SWIFT::CONSOLE& m_console;
};

class LISTENER2 : public SWIFT::INPUT::MOUSE_LISTENER
{
public:
    LISTENER2(SWIFT::CONSOLE& console)
        : m_console(console)
    {}
    virtual void notify(SWIFT::MOUSE_EVENT event)
    {
        auto str = std::to_wstring((int)event.button);
        str += L", ";
        str += std::to_wstring(event.posX);
        str += L", ";
        str += std::to_wstring(event.posY);
        str += L", ";
        str += event.pressed ? L"Pressed" : L"Released";

        m_console.output(str);
        m_console.output(L"\n");
    }
private:
    SWIFT::CONSOLE& m_console;
};

class RECT_LISTENER : public SWIFT::INPUT::MOUSE_LISTENER
{
public:
    RECT_LISTENER(SWIFT::RENDER_SCENE& scene)
        : m_scene(scene)
    {}

    virtual void notify(SWIFT::MOUSE_EVENT event)
    {
        if (event.button != SWIFT::MOUSE_BUTTON::LEFT)
            return;

        if(event.pressed)
        { 
            m_held = true;
            m_pos1 = SWIFT::VECTOR2F(event.posX, event.posY);
        }

        if (!event.pressed && m_held)
        {
            m_held = false;
            SWIFT::RECT rect(m_pos1, SWIFT::VECTOR2F(event.posX, event.posY) - m_pos1);
            m_scene.addRectangle(rect);
        }
    }

private:
    SWIFT::RENDER_SCENE& m_scene;
    SWIFT::VECTOR2F m_pos1;
    bool m_held;
};

int main()
{
    SWIFT::CONSOLE console(std::wcin, std::wcout);
    SWIFT::WINDOW window;
    window.title(L"This is not the title");
    window.create_window(200, 200);

    console.add_console_command(L"exit", L"Exit the application.", &stop_running);
    console.add_console_command(L"fullscreen", L"Make window fullscreen.", window, &SWIFT::WINDOW::create_fullscreen);
    console.add_console_command(L"window", L"Takes width and height as integers and creates a window of that size.", window, &SWIFT::WINDOW::create_window);

    LISTENER listener(console);
    LISTENER2 listener2(console);
    window.input().listen_for_keys(listener);
    window.input().listen_for_mouse(listener2);

    SWIFT::RENDER_SCENE scene;

    RECT_LISTENER rl(scene);
    window.input().listen_for_mouse(rl);

    std::wstring input;
    while (running())
    {
        console.invoke_commands();
        window.update(scene);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }

    return 0;
}