#pragma once

namespace SWIFT
{
	//Type safe way to create id values which can be compared
	//All newly default constructed SWIFT::ID objects have their own id

	template<typename UNIQUE_TYPE, typename ID_TYPE = long long>
	class ID
	{
		static ID_TYPE s_previous_id;
		ID_TYPE m_id;

	public:
		ID();

		bool operator==(const ID& other) const;
		bool operator!=(const ID& other) const;
		bool operator<(const ID& other) const;
	};

	//Provides a way to ID types
	//Each different type that we access get_type_id with will cause a new id to be created which is unique to that type

	using TYPE_ID = ID<struct TYPE_ID_T>;

	template<typename TYPE>
	TYPE_ID get_type_id();

	//Check if two types are the same

	template<typename T1, typename T2>
	bool same_type();
}

//FUNCTIONS

//ID

template<typename UNIQUE_TYPE, typename ID_TYPE>
ID_TYPE SWIFT::ID<UNIQUE_TYPE, ID_TYPE>::s_previous_id = 0;

template<typename UNIQUE_TYPE, typename ID_TYPE>
SWIFT::ID<UNIQUE_TYPE, ID_TYPE>::ID()
{
	m_id = ++s_previous_id;
}

template<typename UNIQUE_TYPE, typename ID_TYPE>
bool SWIFT::ID<UNIQUE_TYPE, ID_TYPE>::operator==(const ID& other) const
{
	return m_id == other.m_id;
}

template<typename UNIQUE_TYPE, typename ID_TYPE>
bool SWIFT::ID<UNIQUE_TYPE, ID_TYPE>::operator!=(const ID& other) const
{
	return m_id != other.m_id;
}

template<typename UNIQUE_TYPE, typename ID_TYPE>
inline bool SWIFT::ID<UNIQUE_TYPE, ID_TYPE>::operator<(const ID& other) const
{
	return m_id < other.m_id;
}

//TYPE_ID

template<typename TYPE>
SWIFT::TYPE_ID SWIFT::get_type_id()
{
	static TYPE_ID id;
	return id;
}

template<typename T1, typename T2>
bool SWIFT::same_type()
{
	return get_type_id<T1>() == get_type_id<T2>();
}