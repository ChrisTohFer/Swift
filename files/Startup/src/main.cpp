#include "sample.h"
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
    //Add basic console commands
    SWIFT::console().add_console_command(L"exit", L"Exit the application.", &stop_running);

    //Create scene
    SAMPLE_SCENE ss;

    while (running())
    {
        SWIFT::console().invoke_commands();
        ss.update();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(15ms);
    }

    return 0;
}