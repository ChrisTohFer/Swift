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

void wstring_test(std::wstring msg)
{
    std::wcout << msg << "\n";
}

int main()
{
    SWIFT::CONSOLE console(std::wcin, std::wcout);

    SWIFT::WINDOW window;
    window.title(L"This is the title");
    window.create_window(200, 200);

    console.add_console_command(L"exit", L"", &stop_running);
    console.add_console_command(L"fullscreen", L"", window, &SWIFT::WINDOW::create_fullscreen);
    console.add_console_command(L"window", L"", window, &SWIFT::WINDOW::create_window);
    console.add_console_command(L"wstringtest", L"", &wstring_test);

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