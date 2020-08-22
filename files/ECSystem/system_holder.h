#pragma once

#include "system.h"

#include <tuple>

namespace SWIFT::EC
{

    template<typename ...SYSTEMS>
    class SYSTEM_HOLDER
    {
        std::tuple<SYSTEMS...> m_systems;

    public:
        template<typename ...ENTITY_TYPES>
        void update(ENTITY_HOLDER<ENTITY_TYPES...>& entities)
        {
            update_systems_recursive<0>(entities);
        }

        template<int INDEX, typename ...ENTITY_TYPES>
        void update_systems_recursive(ENTITY_HOLDER<ENTITY_TYPES...>& entities);

    };

    template<typename ...SYSTEMS>
    template<int INDEX, typename ...ENTITY_TYPES>
    void SYSTEM_HOLDER<SYSTEMS...>::update_systems_recursive([[maybe_unused]] ENTITY_HOLDER<ENTITY_TYPES...>& entities)
    {
        if constexpr (INDEX < sizeof...(SYSTEMS))
        {
            std::get<INDEX>(m_systems).update(entities);
            update_systems_recursive<INDEX + 1>(entities);
        }
    }

}