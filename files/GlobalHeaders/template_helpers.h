#pragma once

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