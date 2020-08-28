#pragma once

#include "system.h"
#include "GlobalHeaders/template_helpers.h"

#include <tuple>

namespace SWIFT::EC
{

    template<typename ...SYSTEMS>
    class SYSTEM_HOLDER
    {
        std::tuple<SYSTEMS...> m_systems;

    public:
        template<typename SYSTEM>
        SYSTEM& system()
        {
            static_assert((std::is_same<SYSTEMS, SYSTEM>::value || ...)); //Will fail if we don't contain the system that is asked for

            auto constexpr index = VARIADIC_INDEX<SYSTEM, SYSTEMS...>::index;
            return std::get<index>(m_systems);
        }
        
        template<typename ...ENTITY_TYPES>
        void update(ENTITY_HOLDER<ENTITY_TYPES...>& entities)
        {
            (system<SYSTEMS>().update(entities), ...);
        }
    };

}