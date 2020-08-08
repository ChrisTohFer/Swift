#pragma once

#include "Types/id.h"

#include <type_traits>

namespace SWIFT::EC
{
    using ENTITY_ID = ID<struct ENTITY_T>;

    struct ENTITY_BASE
    {
        ENTITY_ID id;
    };

    //ENTITY is a collection of components bundled together with an id
    template<typename ... COMPONENTS>
    struct ENTITY : public ENTITY_BASE, protected COMPONENTS ...
    {
        template<typename REQUIRED_COMPONENT>
        constexpr bool has_component() const;
        template<typename ... REQUIRED_COMPONENTS>
        constexpr bool has_components() const;

        template<typename COMPONENT>
        COMPONENT& component();
        template<typename COMPONENT>
        const COMPONENT& component() const;
    };
}

#include "entity.inl"