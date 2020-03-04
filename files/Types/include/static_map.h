#pragma once
#include <type_traits>

namespace st {

	//Create an array with a value of type VALUE for each entry in an enum class
	//ENUM_KEY must have a member called ENUM_SIZE whose underlying value is the number of values in the enum

	template<typename ENUM_KEY, typename VALUE, std::enable_if_t<std::is_enum<ENUM_KEY>::value, int> = 0>
	class STATIC_MAP {
		static const int SIZE = int(ENUM_KEY::ENUM_SIZE);	//Requires that the enum class provides its own size via a member

	public:
		STATIC_MAP() {
			if constexpr (std::is_pointer<VALUE>::value)
			{
				set_all(nullptr);
			}
		}
		STATIC_MAP(const VALUE& value)
		{
			set_all(value);
		}

		VALUE& operator[](ENUM_KEY key)
		{
			if (key == ENUM_KEY::ENUM_SIZE)		//Attempting to access a value past the end of the array, would cause crashes or memory corruption
				abort();

			return m_values[int(key)];
		}
		const VALUE& operator[](ENUM_KEY key) const
		{
			if (key == ENUM_KEY::ENUM_SIZE)		//Attempting to access a value past the end of the array, would cause crashes or memory corruption
				abort();

			return m_values[int(key)];
		}

		void set_all(const VALUE& value)
		{
			for (int i = 0; i < SIZE; ++i)
				m_values[i] = value;
		}

	private:
		VALUE m_values[int(SIZE)];
	};

}