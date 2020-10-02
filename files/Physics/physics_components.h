#pragma once

#include "Types/vector.h"

namespace SWIFT
{

    struct MOMENTUM
    {
        float mass = 1.f;
        VECTOR2F velocity = VECTOR2F::zero();
        float    angular_velocity = 0.f;
    };

}