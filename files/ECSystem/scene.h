#pragma once

#include "entity_holder.h"
#include "system_holder.h"

namespace SWIFT::EC
{

    template<typename ENTITIES, typename SYSTEMS>
    class SCENE
    {
        ENTITIES m_entity_holder;
        SYSTEMS  m_system_holder;

    public:
        //
        void update()
        {
            m_system_holder.update(m_entity_holder);
        }

        //Entities
        template<typename ENTITY_TYPE>
        ENTITY_TYPE& add_entity(ENTITY_TYPE&& entity)
        {
            static_assert(ENTITIES::template has_entity<ENTITY_TYPE>());

            return m_entity_holder.insert(std::move(entity));
        }
    };

}