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
        using namespace std::chrono_literals;
        auto next_update_time = std::chrono::system_clock::now() + 1000000us/60;

        SWIFT::console().invoke_commands();
        ss.update();

        std::this_thread::sleep_until(next_update_time);
    }

    return 0;
}