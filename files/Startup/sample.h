#pragma once

#include "Graphics/graphics_services.h"
#include "Physics/physics_components.h"
#include "Physics/physics_services.h"
#include "ECSystem/scene.h"
#include "Multimedia/multimedia_services.h"
#include "Types/vector.h"
#include "Types/transform.h"

#include <random>
#include <vector>

inline float random()
{
    return static_cast<float>(std::rand()) / RAND_MAX;
}

//Services

//Components

struct VELOCITY
{
    SWIFT::VECTOR2F velocity;
};

//Entities

using BLANK = SWIFT::EC::ENTITY<SWIFT::TRANSFORM, SWIFT::MOMENTUM, SWIFT::SPRITE_COMPONENT>;

//Systems

class SCRIPT : public SWIFT::EC::SYSTEM<SCRIPT, SWIFT::TRANSFORM, SWIFT::MOMENTUM>
{
    SWIFT::VECTOR2F bounds;
    bool pressed = false;

public:
    template<typename SCENE>
    void start(SCENE&)
    {

    }
    template<typename SCENE>
    void early_update(SCENE& scene)
    {
        SWIFT::INPUT& input = scene.template service<SWIFT::WINDOW_SERVICE>().input();
        pressed = input.key_pressed(SWIFT::KEY::Space);

        bounds = scene.template service<SWIFT::WINDOW_SERVICE>().size();
    }
    template<typename SCENE>
    void late_update(SCENE&)
    {

    }
    template<typename SCENE>
    void update_per_entity(SCENE& scene, SWIFT::EC::ENTITY_BASE& entity, SWIFT::TRANSFORM* transform, SWIFT::MOMENTUM* velocity)
    {
        if (transform->position.x < -10.f)
            transform->position.x += bounds.x + 10.f;
        else if (transform->position.x > bounds.x)
            transform->position.x -= bounds.x + 10.f;
        if (transform->position.y < -10.f)
            transform->position.y += bounds.y + 10.f;
        else if (transform->position.y > bounds.y)
            transform->position.y -= bounds.y + 10.f;


        if (pressed or random() < 1.0f / 300.f)  //1 in 300 chance per frame = every 5 seconds or so
        {
            auto x_vel   = random() - 0.5f;
            auto y_vel   = random() - 0.5f;
            auto rot_vel = (random() - 0.5f) * 2.f;
            velocity->velocity = SWIFT::VECTOR2F(x_vel, y_vel).normalize() * 100;
            velocity->angular_velocity = rot_vel;

            BLANK& b = scene.instantiate<BLANK>();
            auto accessor = scene.create_accessor(b);
            BLANK& c = *accessor.access();
            c.component<SWIFT::TRANSFORM>() = *transform;
            auto& momentum = c.component<SWIFT::MOMENTUM>();
            momentum.velocity = -velocity->velocity;
            momentum.angular_velocity = -rot_vel;

            auto& sprite = c.component<SWIFT::SPRITE_COMPONENT>();
            sprite.size = SWIFT::VECTOR2F(10.f, 10.f);
            auto colour_rand = random();
            if (colour_rand < 0.33f)
            {
                sprite.colour = SWIFT::COLOURS::red;
            }
            else if (colour_rand < 0.66f)
            {
                sprite.colour = SWIFT::COLOURS::green;
            }
            else
            {
                sprite.colour = SWIFT::COLOURS::blue;
            }
        }

        if (random() < (1.0f / 900000.f * scene.entity_count<void>()))    //1 in 900,000 chance multiplied by number of entities
        {
            scene.destroy(entity.id());
        }
    }
};

//Containers

using ENTITIES = SWIFT::EC::ENTITY_HOLDER<
    BLANK>;
using SYSTEMS = SWIFT::EC::SYSTEM_HOLDER<
    SCRIPT,
    SWIFT::MOVEMENT,
    SWIFT::RENDER_SYSTEM>;
using SERVICES = SWIFT::EC::SERVICE_HOLDER<
    SWIFT::WINDOW_SERVICE,
    SWIFT::MAIN_CAMERA>;

//Scene

class SAMPLE_SCENE : public SWIFT::EC::SCENE<ENTITIES, SYSTEMS, SERVICES>
{
public:
    SAMPLE_SCENE()
    {
        BLANK& b = instantiate<BLANK>();
        auto& t = b.component<SWIFT::TRANSFORM>();
        t.position = SWIFT::VECTOR2F(500.f, 500.f);
        t.rotation = 0.2f;

        auto& s = b.component<SWIFT::SPRITE_COMPONENT>();
        s.size = SWIFT::VECTOR2F(10.f, 10.f);
        s.colour = SWIFT::COLOURS::red;
    }
};