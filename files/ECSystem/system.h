#pragma once

#include "entity_holder.h"

#include <vector>

namespace SWIFT::EC
{

    template<typename DERIVED_SYSTEM, typename ...TARGET_COMPONENTS>
    class SYSTEM
    {
    public:
        template<typename ENTITY_TYPE>
        void update([[maybe_unused]] ENTITY_MAP<ENTITY_TYPE>& map)
        {
            if constexpr (ENTITY_TYPE::template has_components<TARGET_COMPONENTS...>())
            {
                //We know that this entity type has the components we need - iterate
                for (auto it = map.begin(), it_end = map.end(); it != it_end; ++it)
                {
                    static_cast<DERIVED_SYSTEM*>(this)->update_per_entity(it->second.template component<TARGET_COMPONENTS>()...);
                }
            }
        }

        template<typename ...ENTITY_TYPES>
        void update(ENTITY_HOLDER<ENTITY_TYPES...>& holder)
        {
            //Call update for every entity type
            (update(holder.template entity_map<ENTITY_TYPES>()), ...);
        }
    };

}

//#include "system.inl"