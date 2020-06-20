#include "Multimedia/window.h"

#include <iostream>
#include <string>
#include <sstream>

int main()
{
    SWIFT::WINDOW window;
    window.title("This is the title");
    window.create_window(200,200);

    std::string input;
    while (std::getline(std::cin, input))
    {
        std::istringstream iss(input);
        std::string term;

        iss >> term;

        if (term == "exit")
        {
            window.close_window();
            break;
        }
        else if (term == "fullscreen")
        {
            window.create_fullscreen();
        }
        else if (term == "fullscreenx")
        {
            int i;
            iss >> i;
            window.create_fullscreen(i);
        }
        else if (term == "window")
        {
            int width, height;
            iss >> width >> height;
            window.create_window(width, height);
        }
        else if (term == "title")
        {
            iss >> term;
            window.title(term.c_str());
        }
        else if (term == "check")
        {
            std::cout << (window.is_open() ? "Open" : "Closed") << "\n";
        }
    }

    return 0;
}