#pragma once

#include "GlobalHeaders/template_helpers.h"
#include <tuple>

namespace SWIFT::EC
{

    template<typename ...SERVICES>
    class SERVICE_HOLDER
    {
        std::tuple<SERVICES...> m_services;

    public:
        template<typename SERVICE>
        SERVICE& service()
        {
            static_assert((std::is_same<SERVICES, SERVICE>::value || ...)); //Will fail if we don't contain the service that is asked for

            auto constexpr index = VARIADIC_INDEX<SERVICE, SERVICES...>::index;
            return std::get<index>(m_services);
        }

        void update()
        {
            (service<SERVICES>().update(), ...);
        }
    };

}