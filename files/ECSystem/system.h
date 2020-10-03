#pragma once

#include "entity_holder.h"

#include <vector>

namespace SWIFT::EC
{

    template<typename DERIVED_SYSTEM, typename ...TARGET_COMPONENTS>
    class SYSTEM
    {
        size_t m_count;

    protected:
        size_t count()
        {
            return m_count;
        }

    private:
        template<typename SCENE, typename ENTITY_TYPE>
        void update([[maybe_unused]] SCENE& scene, [[maybe_unused]] ENTITY_MAP<ENTITY_TYPE>& map)
        {
            if constexpr (ENTITY_TYPE::template has_components<TARGET_COMPONENTS...>())
            {
                //We know that this entity type has the components we need - iterate
                for (auto it = map.begin(), it_end = map.end(); it != it_end; ++it)
                {
                    static_cast<DERIVED_SYSTEM*>(this)->update_per_entity(scene, it->second, it->second.template component<TARGET_COMPONENTS>()...);
                }
            }
        }

    public:
        template<typename SCENE, typename ...ENTITY_TYPES>
        void update(SCENE& scene, ENTITY_HOLDER<ENTITY_TYPES...>& holder)
        {
            m_count = 
                (((ENTITY_TYPES::template has_components<TARGET_COMPONENTS...>()) //Applicable?
                * holder.entity_map<ENTITY_TYPES>().size())                       //Size
                + ...);  //Fold

            static_cast<DERIVED_SYSTEM*>(this)->early_update(scene);
            (update(scene, holder.template entity_map<ENTITY_TYPES>()), ...);  //Call update for every entity type
            static_cast<DERIVED_SYSTEM*>(this)->late_update(scene);
        }
    };

}