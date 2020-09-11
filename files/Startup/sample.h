#pragma once

#include "Graphics/graphics_services.h"
#include "ECSystem/scene.h"
#include "Multimedia/multimedia_services.h"
#include "Types/vector.h"

#include <random>
#include <vector>

//Services

//Components

struct TRANSFORM
{
    SWIFT::VECTOR2F position;
};

struct VELOCITY
{
    SWIFT::VECTOR2F velocity;
};

struct RENDER_COMPONENT
{

};

//Entities

using BLANK = SWIFT::EC::ENTITY<TRANSFORM, VELOCITY, RENDER_COMPONENT>;

//Systems

class MOVEMENT : public SWIFT::EC::SYSTEM<MOVEMENT, TRANSFORM, VELOCITY>
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
    void update_per_entity(SCENE& scene, SWIFT::EC::ENTITY_BASE& entity, TRANSFORM& transform, VELOCITY& velocity)
    {
        transform.position += velocity.velocity;

        if (transform.position.x < -10.f)
            transform.position.x += bounds.x + 10.f;
        else if (transform.position.x > bounds.x)
            transform.position.x -= bounds.x + 10.f;
        if (transform.position.y < -10.f)
            transform.position.y += bounds.y + 10.f;
        else if (transform.position.y > bounds.y)
            transform.position.y -= bounds.y + 10.f;


        if (pressed or static_cast<float>(std::rand()) / RAND_MAX < 1.0f / 300.f)  //1 in 300 chance per frame = every 5 seconds or so
        {
            auto x_vel = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
            auto y_vel = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
            velocity.velocity = SWIFT::VECTOR2F(x_vel, y_vel).normalize() * 0.5f;

            BLANK b;
            b.component<TRANSFORM>() = transform;
            b.component<VELOCITY>().velocity = -velocity.velocity;
            scene.instantiate(std::move(b));
        }

        if (static_cast<float>(std::rand()) / RAND_MAX < (1.0f / 900000.f * scene.entity_count<void>()))    //1 in 900,000 chance multiplied by number of entities
        {
            scene.destroy(entity.id());
        }
    }
};

class RENDERER : public SWIFT::EC::SYSTEM<RENDERER, TRANSFORM, RENDER_COMPONENT>
{
public:
    template<typename SCENE>
    void start(SCENE&)
    {

    }
    template<typename SCENE>
    void early_update(SCENE& scene)
    {
        scene.template service<SWIFT::RENDER_SERVICE>().clear_and_reserve(count());
    }
    template<typename SCENE>
    void late_update(SCENE&)
    {

    }
    template<typename SCENE>
    void update_per_entity(SCENE& scene, SWIFT::EC::ENTITY_BASE&, TRANSFORM& transform, RENDER_COMPONENT&)
    {
        auto rect = std::unique_ptr<SWIFT::RENDER_OBJECT>(new SWIFT::RECT(transform.position, SWIFT::VECTOR2F(10.f,10.f)));
        scene.template service<SWIFT::RENDER_SERVICE>().add_object(std::move(rect));
    }
};

//Containers

using ENTITIES = SWIFT::EC::ENTITY_HOLDER<
    BLANK>;
using SYSTEMS = SWIFT::EC::SYSTEM_HOLDER<
    MOVEMENT,
    RENDERER>;
using SERVICES = SWIFT::EC::SERVICE_HOLDER<
    SWIFT::WINDOW_SERVICE,
    SWIFT::RENDER_SERVICE>;

//Scene

class SAMPLE_SCENE : public SWIFT::EC::SCENE<ENTITIES, SYSTEMS, SERVICES>
{
public:
    SAMPLE_SCENE()
    {
        BLANK b;
        b.component<TRANSFORM>().position = SWIFT::VECTOR2F(500.f, 500.f);
        instantiate(std::move(b));
    }
};