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
    using SWIFT::console;

    //Create window
    SWIFT::WINDOW window;
    window.title(L"This is not the title");
    window.create_fullscreen();


    //Add basic console commands
    console().add_console_command(L"exit", L"Exit the application.", &stop_running);
    console().add_console_command(L"fullscreen", L"Make window fullscreen.", window, &SWIFT::WINDOW::create_fullscreen);
    console().add_console_command(L"window", L"Takes width and height as integers and creates a window of that size.", window, &SWIFT::WINDOW::create_window);

    //Create scene
    SWIFT::RENDER_SCENE rscene;
    SAMPLE_SCENE ss(rscene);

    std::wstring input;
    const int updates_per_spawn = 10;
    int updates = 0;
    while (running())
    {
        if (++updates == updates_per_spawn)
        {
            updates = 0;
            BLANK b;
            b.component<TRANSFORM>().position = SWIFT::VECTOR2F(500.f, 500.f);
            ss.add_entity(std::move(b));
        }
        console().invoke_commands();
        ss.update();
        window.update(std::move(rscene));

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(15ms);
    }
    window.close_window();

    return 0;
}