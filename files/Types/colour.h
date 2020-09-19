#pragma once

#include <cstdint>

namespace SWIFT
{

    struct COLOUR
    {
        std::uint8_t r;
        std::uint8_t g;
        std::uint8_t b;
        std::uint8_t a;

        COLOUR();
        COLOUR(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
    };

    namespace COLOURS
    {
        const inline COLOUR white = COLOUR(255, 255, 255, 255);
        const inline COLOUR red   = COLOUR(255, 0, 0, 255);
        const inline COLOUR blue  = COLOUR(0, 255, 0, 255);
        const inline COLOUR green = COLOUR(0, 0, 255, 255);
    }
}