#include "entity.h"

//ENTITY inline functions

template<typename ... COMPONENTS>
template<typename REQUIRED_COMPONENT>
constexpr bool SWIFT::EC::ENTITY<COMPONENTS...>::has_component() const
{
    return std::is_base_of<REQUIRED_COMPONENT, ENTITY>::value;
}

template<typename ... COMPONENTS>
template<typename ... REQUIRED_COMPONENTS>
constexpr bool SWIFT::EC::ENTITY<COMPONENTS...>::has_components() const
{
    return (std::is_base_of<REQUIRED_COMPONENTS, ENTITY>::value && ...);
}

template<typename ... COMPONENTS>
template<typename COMPONENT>
COMPONENT& SWIFT::EC::ENTITY<COMPONENTS...>::component()
{
    return *static_cast<COMPONENT*>(this);
}

template<typename ... COMPONENTS>
template<typename COMPONENT>
const COMPONENT& SWIFT::EC::ENTITY<COMPONENTS...>::component() const
{
    return *static_cast<const COMPONENT*>(this);
}