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
		ID()
		{
			m_id = ++s_previous_id;
		}

		bool operator==(const ID& other) const
		{
			return m_id == other.m_id;
		}
		bool operator!=(const ID& other) const
		{
			return m_id != other.m_id;
		}
	};

	template<typename UNIQUE_TYPE, typename ID_TYPE>
	ID_TYPE ID<UNIQUE_TYPE, ID_TYPE>::s_previous_id = 0;

	//Provides a way to ID types
	//Each different type that we access get_type_id with will cause a new id to be created which is unique to that type

	using TYPE_ID = ID<int>;

	template<typename TYPE>
	TYPE_ID get_type_id()
	{
		static TYPE_ID id;
		return id;
	}

	//Check if two types are the same

	template<typename T1, typename T2>
	bool same_type()
	{
		return get_type_id<T1>() == get_type_id<T2>();
	}
}