#pragma once

#include "entity_holder.h"
#include "system_holder.h"
#include "service_holder.h"

namespace SWIFT::EC
{

    template<typename ENTITIES, typename SYSTEMS, typename SERVICES>
    class SCENE
    {
    protected:
        ENTITIES m_entity_holder;   //of type ENTITY_HOLDER
        SYSTEMS  m_system_holder;   //of type SYSTEM_HOLDER
        SERVICES m_service_holder;  //of type SERVICE_HOLDER

    public:
        SCENE()
        {
            m_service_holder.start(*this);
            m_system_holder.start(*this);
        }

        //Systems + services
        void update()
        {
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

    };

}