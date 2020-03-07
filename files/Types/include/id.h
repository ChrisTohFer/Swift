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
}