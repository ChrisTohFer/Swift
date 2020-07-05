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

int main()
{
    SWIFT::CONSOLE console(std::wcin, std::wcout);

    SWIFT::WINDOW window;
    window.title(L"This is the title");
    window.create_window(200, 200);

    console.add_console_command(L"exit", L"Exit the application.", &stop_running);
    console.add_console_command(L"fullscreen", L"Make window fullscreen.", window, &SWIFT::WINDOW::create_fullscreen);
    console.add_console_command(L"window", L"Takes width and height as integers and creates a window of that size.", window, &SWIFT::WINDOW::create_window);

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