#pragma once

#include "physics_components.h"

#include "ECSystem/system.h"
#include "Types/transform.h"

namespace SWIFT
{
    class MOVEMENT : public EC::SYSTEM<MOVEMENT, TRANSFORM, MOMENTUM> 
    {
    public:
        template<typename SCENE>
        void start(SCENE&)
        {
        }
        template<typename SCENE>
        void early_update(SCENE&)
        {
        }
        template<typename SCENE>
        void late_update(SCENE&)
        {
        }
        template<typename SCENE>
        void update_per_entity(SCENE& scene, EC::ENTITY_BASE&, TRANSFORM* transform, MOMENTUM* momentum)
        {
            float timestep = scene.timestep();
            transform->position += timestep * momentum->velocity;
            transform->rotation += timestep * momentum->angular_velocity;
        }
    };
}