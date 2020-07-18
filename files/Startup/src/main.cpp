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
    window.input().listen_for_keys(listener);

    std::wstring input;
    while (running())
    {
        console.invoke_commands();
        window.update();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }

    return 0;
}