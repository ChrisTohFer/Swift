#pragma once

#include <type_traits>

//VARIADIC_INDEX - used to obtain the index of a particular type in a template parameter pack

template<typename TYPE, typename ...TUPLE_TYPES>
struct VARIADIC_INDEX;

template<typename TYPE, typename ...TUPLE_TYPES>
struct VARIADIC_INDEX<TYPE, TYPE, TUPLE_TYPES...>
{
    static const size_t index = 0;
};

template<typename TYPE, typename OTHER_TYPE, typename ...TUPLE_TYPES>
struct VARIADIC_INDEX<TYPE, OTHER_TYPE, TUPLE_TYPES...>
{
    static const size_t index = VARIADIC_INDEX<TYPE, TUPLE_TYPES...>::index + 1;
};

//VARIADIC_POLYMORPHIC_INDEX - similar to VARIADIC_INDEX but can return the index of a derived type instead of the chosen type

template<typename BASE_TYPE, typename TYPE, typename ...TUPLE_TYPES>
struct VARIADIC_POLYMORPHIC_INDEX
{
private:
    static constexpr size_t recursive_value()
    {
        if constexpr (std::is_base_of<BASE_TYPE, TYPE>::value)
        {
            return 0;
        }
        else
        {
            return VARIADIC_POLYMORPHIC_INDEX<BASE_TYPE, TUPLE_TYPES...>::index + 1;
        }
    }

public:
    static const size_t index = recursive_value();
};