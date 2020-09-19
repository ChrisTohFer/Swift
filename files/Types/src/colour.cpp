#include "colour.h"

SWIFT::COLOUR::COLOUR()
    : r(255), g(255), b(255), a(255)
{}
SWIFT::COLOUR::COLOUR(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
    : r(red), g(green), b(blue), a(alpha)
{}
