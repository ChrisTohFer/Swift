#pragma once
#include <type_traits>

//Create an array with a value of type VALUE for each entry in an enum class
//ENUM_KEY must have a member called ENUM_SIZE whose underlying value is the number of values in the enum

template<typename ENUM_KEY, typename VALUE, std::enable_if_t<std::is_enum<ENUM_KEY>::value, int> = 0>
class STATIC_MAP {
public:
	VALUE& operator[](ENUM_KEY key)
	{
		if (key == ENUM_KEY::ENUM_SIZE)
			abort();		//Attempting to access a value past the end of the array, will cause a crashes or memory corruption

		return m_values[int(key)];
	}
	const VALUE& operator[](ENUM_KEY key) const
	{
		if (key == ENUM_KEY::ENUM_SIZE)
			abort();		//Attempting to access a value past the end of the array, will cause a crashes or memory corruption

		return m_values[int(key)];
	}

private:
	
	VALUE m_values[int(ENUM_KEY::ENUM_SIZE)];	//Requires that the enum class provides its own size via a member
};