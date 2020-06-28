#include "Multimedia/window.h"

#include <iostream>
#include <string>
#include <sstream>

int main()
{
    SWIFT::WINDOW window;
    window.title(L"This is the title");
    window.create_window(200,200);

    std::wstring input;
    while (std::getline(std::wcin, input))
    {
        std::wistringstream iss(input);
        std::wstring term;

        iss >> term;

        if (term == L"exit")
        {
            window.close_window();
            break;
        }
        else if (term == L"fullscreen")
        {
            window.create_fullscreen();
        }
        else if (term == L"window")
        {
            int width, height;
            iss >> width >> height;
            window.create_window(width, height);
        }
        else if (term == L"title")
        {
            iss >> term;
            window.title(term.c_str());
        }
        else if (term == L"check")
        {
            std::wcout << (window.is_open() ? L"Open" : L"Closed") << "\n";
        }

        window.update();
    }

    return 0;
}