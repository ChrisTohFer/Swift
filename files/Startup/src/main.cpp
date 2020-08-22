#include "Multimedia/window.h"
#include "Console/console.h"

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
    using SWIFT::console;

    //Create window
    SWIFT::WINDOW window;
    window.title(L"This is not the title");
    window.create_fullscreen();

    //Add basic console commands
    console().add_console_command(L"exit", L"Exit the application.", &stop_running);
    console().add_console_command(L"fullscreen", L"Make window fullscreen.", window, &SWIFT::WINDOW::create_fullscreen);
    console().add_console_command(L"window", L"Takes width and height as integers and creates a window of that size.", window, &SWIFT::WINDOW::create_window);

    SWIFT::RENDER_SCENE scene;

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