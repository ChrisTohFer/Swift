#include "multimedia_services.h"

SWIFT::WINDOW_SERVICE::WINDOW_SERVICE()
{
    using SWIFT::console;

    m_window.title(L"This is not the title");
    m_window.create_fullscreen();
    console().add_console_command(L"fullscreen", L"Make window fullscreen.", m_window, &SWIFT::WINDOW::create_fullscreen);
    console().add_console_command(L"window", L"Takes width and height as integers and creates a window of that size.", m_window, &SWIFT::WINDOW::create_window);
}

SWIFT::WINDOW_SERVICE::~WINDOW_SERVICE()
{
    console().remove_console_command(L"fullscreen");
    console().remove_console_command(L"window");
}

SWIFT::VECTOR2F SWIFT::WINDOW_SERVICE::size()
{
    return m_window.size();
}

SWIFT::INPUT& SWIFT::WINDOW_SERVICE::input()
{
    return m_window.input();
}