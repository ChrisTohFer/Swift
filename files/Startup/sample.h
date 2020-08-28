#pragma once

#include "ECSystem/scene.h"
#include "Multimedia/render_scene.h"
#include "Types/vector.h"

#include <random>
#include <vector>

//Services

//Components

struct TRANSFORM
{
    SWIFT::VECTOR2F position;
};

struct RENDER_COMPONENT
{

};

//Entities

using BLANK = SWIFT::EC::ENTITY<TRANSFORM, RENDER_COMPONENT>;

//Systems

class MOVEMENT : public SWIFT::EC::SYSTEM<MOVEMENT, TRANSFORM>
{
public:
    void early_update()
    {

    }
    void late_update()
    {

    }
    void update_per_entity(TRANSFORM& transform)
    {
        auto x_change = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
        auto y_change = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
        SWIFT::VECTOR2F position_change(x_change, y_change);
        position_change *= 10.f;
        transform.position += position_change;
    }
};

class RENDERER : public SWIFT::EC::SYSTEM<RENDERER, TRANSFORM, RENDER_COMPONENT>
{
public:
    SWIFT::RENDER_SCENE* render_scene;

    void early_update()
    {
        render_scene->clear_and_reserve(count());
    }
    void late_update()
    {

    }
    void update_per_entity(TRANSFORM& transform, RENDER_COMPONENT&)
    {
        auto circle = std::unique_ptr<SWIFT::RENDER_OBJECT>(new SWIFT::CIRCLE(transform.position, 5.f));
        render_scene->add_object(std::move(circle));
    }
};

//Containers

using ENTITIES = SWIFT::EC::ENTITY_HOLDER<BLANK>;
using SYSTEMS = SWIFT::EC::SYSTEM_HOLDER<MOVEMENT, RENDERER>;
using SERVICES = SWIFT::EC::SERVICE_HOLDER<>;

//Scene

class SAMPLE_SCENE : public SWIFT::EC::SCENE<ENTITIES, SYSTEMS, SERVICES>
{
public:
    SAMPLE_SCENE(SWIFT::RENDER_SCENE& rscene)
    {
        m_system_holder.system<RENDERER>().render_scene = &rscene;

        for (auto i = 0u; i < 5u; ++i)
        {
            BLANK b;
            b.component<TRANSFORM>().position = SWIFT::VECTOR2F(500.f, 500.f);
            add_entity(std::move(b));
        }
    }
};