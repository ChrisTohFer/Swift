#pragma once

#include "entity_holder.h"
#include "system_holder.h"
#include "service_holder.h"
#include "GlobalHeaders/timing.h"

namespace SWIFT::EC
{

    template<typename ENTITIES, typename SYSTEMS, typename SERVICES>
    class SCENE
    {
    protected:
        ENTITIES    m_entity_holder;       //of type ENTITY_HOLDER
        SYSTEMS     m_system_holder;       //of type SYSTEM_HOLDER
        SERVICES    m_service_holder;      //of type SERVICE_HOLDER
        
        float       m_timestep      = 1 / 60.f; //intended length of update in seconds
        float       m_last_timestep = 1 / 60.f; //actual length of previous update in seconds
        CYCLE_TIMER m_update_timer;

    public:
        SCENE()
            : m_update_timer(300)
        {
            m_service_holder.start(*this);
            m_system_holder.start(*this);
        }

        //Systems + services
        void update()
        {
            m_update_timer.cycle();
            m_last_timestep = m_update_timer.previous_cycle_time() / 1000000.f; //Convert microseconds to seconds

            m_service_holder.update(*this);
            m_entity_holder.update();
            m_system_holder.update(*this, m_entity_holder);
        }

        template<typename SYSTEM>
        SYSTEM& system()
        {
            return m_system_holder.template system<SYSTEM>();
        }
        template<typename SERVICE>
        SERVICE& service()
        {
            return m_service_holder.template service<SERVICE>();
        }

        //Entities
        template<typename ENTITY_TYPE>
        ENTITY_TYPE& add_entity(ENTITY_TYPE&& entity)
        {
            static_assert(ENTITIES::template has_entity<ENTITY_TYPE>());

            return m_entity_holder.insert(std::move(entity));
        }
        template<typename ENTITY_TYPE>
        ENTITY_TYPE& instantiate(ENTITY_TYPE&& entity)
        {
            static_assert(ENTITIES::template has_entity<ENTITY_TYPE>());

            return m_entity_holder.instantiate(std::move(entity));
        }
        template<typename ENTITY_TYPE>
        ENTITY_TYPE& instantiate(ENTITY_TYPE const& entity)
        {
            static_assert(ENTITIES::template has_entity<ENTITY_TYPE>());

            return m_entity_holder.instantiate(entity.copy());
        }
        void destroy(ENTITY_ID id)
        {
            m_entity_holder.destroy(id);
        }

        template<typename ENTITY_TYPE>
        size_t entity_count()
        {
            return m_entity_holder.count<ENTITY_TYPE>();
        }

        //Timing
        float timestep()
        {
            return m_timestep;
        }
        float timestep(float new_timestep)
        {
            return m_timestep = new_timestep;
        }
        float last_timestep()
        {
            return m_last_timestep;
        }
        float framerate(int samples)
        {
            auto microsecond_average = m_update_timer.average_cycle_time(samples);
            return 1000000.f / microsecond_average;
        }
    };

}