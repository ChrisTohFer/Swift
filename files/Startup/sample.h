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
    template<typename SCENE>
    void early_update(SCENE&)
    {

    }
    template<typename SCENE>
    void late_update(SCENE&)
    {

    }
    template<typename SCENE>
    void update_per_entity(SCENE& scene, TRANSFORM& transform)
    {
        auto x_change = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
        auto y_change = static_cast<float>(std::rand()) / RAND_MAX - 0.5f;
        SWIFT::VECTOR2F position_change(x_change, y_change);
        position_change *= 10.f;
        transform.position += position_change;

        if (static_cast<float>(std::rand()) / RAND_MAX < 1.0f / 300.f)  //1 in 300 chance per frame = doubles every 5 seconds or so
        {
            BLANK b;
            b.component<TRANSFORM>() = transform;
            scene.instantiate(std::move(b));
        }
    }
};

class RENDERER : public SWIFT::EC::SYSTEM<RENDERER, TRANSFORM, RENDER_COMPONENT>
{
public:
    SWIFT::RENDER_SCENE* render_scene;

    template<typename SCENE>
    void early_update(SCENE&)
    {
        render_scene->clear_and_reserve(count());
    }
    template<typename SCENE>
    void late_update(SCENE&)
    {

    }
    template<typename SCENE>
    void update_per_entity(SCENE&, TRANSFORM& transform, RENDER_COMPONENT&)
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