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
	//Each different type that we access TYPE_ID with will cause a new id to be created (during static initialization) which is unique to that type

	using TYPE_ID = ID<int>;

	template<typename TYPE>
	struct GET_TYPE_ID final
	{
		const static ID<TYPE_ID> id;

	private:
		GET_TYPE_ID();	//Private to prevent construction
	};

	template<typename TYPE>
	const ID<TYPE_ID> GET_TYPE_ID<TYPE>::id;

	//Check if two types are the same

	template<typename T1, typename T2>
	bool same_type()
	{
		return GET_TYPE_ID<T1>::id == GET_TYPE_ID<T2>::id;
	}
}