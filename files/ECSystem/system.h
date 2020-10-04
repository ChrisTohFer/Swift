#pragma once

#include "entity_holder.h"

#include <vector>
#include <type_traits>

namespace SWIFT::EC
{
    template<typename COMPONENT>
    struct OPTIONAL
    {
        using TYPE = COMPONENT;
    };

    template<typename T>
    struct IS_OPTIONAL;

    template<typename T>
    struct IS_OPTIONAL<OPTIONAL<T>>
    {
        static constexpr bool value = true;
    };
    template<typename T>
    struct IS_OPTIONAL
    {
        static constexpr bool value = false;
    };

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
        template<typename ENTITY_TYPE>
        static constexpr bool entity_has_required_components()
        {
            return ((IS_OPTIONAL<TARGET_COMPONENTS>::value                      //Optional component...
                || ENTITY_TYPE::template has_component<TARGET_COMPONENTS>())    //... or available
                && ...);                                                        //Must be true for every component
        }

        template<typename ENTITY_TYPE, typename COMPONENT_TYPE, typename std::enable_if_t<IS_OPTIONAL<COMPONENT_TYPE>::value, int> = 0>
        typename COMPONENT_TYPE::TYPE* get_component_if_available(ENTITY_TYPE& entity)
        {
            //Component is optional so we must check if it exists

            using COMPONENT = typename COMPONENT_TYPE::TYPE;
            if constexpr (ENTITY_TYPE::template has_component<COMPONENT>())
                return &(entity.template component<COMPONENT>());
            else
                return nullptr;
        }
        template<typename ENTITY_TYPE, typename COMPONENT_TYPE, typename std::enable_if_t<!IS_OPTIONAL<COMPONENT_TYPE>::value, int> = 0>
        COMPONENT_TYPE* get_component_if_available(ENTITY_TYPE& entity)
        {
            return &(entity.template component<COMPONENT_TYPE>());
        }


        template<typename SCENE, typename ENTITY_TYPE>
        void update([[maybe_unused]] SCENE& scene, [[maybe_unused]] ENTITY_MAP<ENTITY_TYPE>& map)
        {
            if constexpr (entity_has_required_components<ENTITY_TYPE>())
            {
                //We know that this entity type has the components we need - iterate
                for (auto it = map.begin(), it_end = map.end(); it != it_end; ++it)
                {
                    static_cast<DERIVED_SYSTEM*>(this)->update_per_entity(
                        scene,
                        it->second,
                        get_component_if_available<ENTITY_TYPE, TARGET_COMPONENTS>(it->second)...
                    );
                }
            }
        }

    public:
        template<typename SCENE, typename ...ENTITY_TYPES>
        void update(SCENE& scene, ENTITY_HOLDER<ENTITY_TYPES...>& holder)
        {
            m_count = 
                (((entity_has_required_components<ENTITY_TYPES>())              //Applicable?
                * holder.entity_map<ENTITY_TYPES>().size())                     //Size
                + ...);  //Fold

            static_cast<DERIVED_SYSTEM*>(this)->early_update(scene);
            (update(scene, holder.template entity_map<ENTITY_TYPES>()), ...);   //Call update for every entity type
            static_cast<DERIVED_SYSTEM*>(this)->late_update(scene);
        }
    };

}