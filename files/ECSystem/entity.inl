#include "entity.h"

//ENTITY inline functions

template<typename ... COMPONENTS>
template<typename REQUIRED_COMPONENT>
constexpr bool SWIFT::EC::ENTITY<COMPONENTS...>::has_component()
{
    return (std::is_same<REQUIRED_COMPONENT, COMPONENTS>::value || ...);
}

template<typename ... COMPONENTS>
template<typename ... REQUIRED_COMPONENTS>
constexpr bool SWIFT::EC::ENTITY<COMPONENTS...>::has_components()
{
    return (ENTITY::has_component<REQUIRED_COMPONENTS>() && ...);
}

template<typename ... COMPONENTS>
template<typename COMPONENT>
COMPONENT& SWIFT::EC::ENTITY<COMPONENTS...>::component()
{
    static_assert(ENTITY::has_component<COMPONENT>());

    constexpr auto index = VARIADIC_INDEX<COMPONENT, COMPONENTS...>::index;
    return std::get<index>(m_components);
}

template<typename ... COMPONENTS>
template<typename COMPONENT>
const COMPONENT& SWIFT::EC::ENTITY<COMPONENTS...>::component() const
{
    //If this assert fails, we are trying to access a component in an entity that doesn't contain it
    static_assert(ENTITY::has_component<COMPONENT>());

    constexpr auto index = VARIADIC_INDEX<COMPONENT, COMPONENTS...>::index;
    return std::get<index>(m_components);
}

template<typename ... COMPONENTS>
SWIFT::EC::ENTITY<COMPONENTS...> SWIFT::EC::ENTITY<COMPONENTS...>::copy() const
{
    auto other = *this;
    other.m_id = ENTITY_ID();
    return other;
}