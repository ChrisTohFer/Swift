#pragma once

#include "entity.h"
#include "entity_holder.h"

namespace SWIFT::EC
{
    
    template<typename HOLDER, typename ENTITY_TYPE>
    class ENTITY_ACCESSOR
    {
    public:
        ENTITY_ACCESSOR(HOLDER& holder, ENTITY_ID id)
        {
            m_entity_holder = &holder;
            m_id = id;
        }
        ENTITY_ACCESSOR(HOLDER& holder, ENTITY_BASE const& entity)
        {
            m_entity_holder = &holder;
            m_id = entity.id();
        }

        ENTITY_TYPE* access()
        {
            auto& map           = m_entity_holder->template entity_map<ENTITY_TYPE>();
            auto  entity_iter   = map.find(m_id);

            if (entity_iter != map.end())
                return &(entity_iter->second);

            //We didn't find entity - it might not have been instantiated yet
            auto& instantiation_map = m_entity_holder->template instantiaton_entity_map<ENTITY_TYPE>();
            entity_iter = instantiation_map.find(m_id);

            if (entity_iter != instantiation_map.end())
                return &(entity_iter->second);

            //Entity doesn't exist
            return nullptr;
        }

    private:
        HOLDER*     m_entity_holder;
        ENTITY_ID   m_id;
    };
}