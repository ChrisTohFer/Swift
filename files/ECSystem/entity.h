#pragma once

#include "Types/id.h"
#include "GlobalHeaders/template_helpers.h"

#include <type_traits>
#include <tuple>

namespace SWIFT::EC
{
    using ENTITY_ID = ID<struct ENTITY_T>;

    class ENTITY_BASE
    {
    protected:
        ENTITY_ID m_id;
    public:
        ENTITY_ID id() { return m_id; }
    };

    //ENTITY is a collection of components bundled together with an id
    template<typename ... COMPONENTS>
    class ENTITY : public ENTITY_BASE
    {
        std::tuple<COMPONENTS...> m_components;

    public:
        template<typename REQUIRED_COMPONENT>
        static constexpr bool has_component();
        template<typename ... REQUIRED_COMPONENTS>
        static constexpr bool has_components();

        template<typename COMPONENT>
        COMPONENT& component();
        template<typename COMPONENT>
        const COMPONENT& component() const;

        ENTITY copy() const;
    };
}

#include "entity.inl"