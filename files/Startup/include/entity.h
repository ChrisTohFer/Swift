#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "Types/include/id.h"
#include "component.h"


namespace SWIFT::ENGINE {

	//Types
	class ENTITY;
	using ENTITY_ID = ID<ENTITY>;

	template<typename COMPONENT_TYPE>
	using IS_COMPONENT_TYPE = std::enable_if_t<std::is_base_of<COMPONENT<COMPONENT_TYPE>, COMPONENT_TYPE>::value, int>;

	//Entity base class
	class ENTITY
	{
		std::vector<UNIQUE_COMPONENT> m_components;

	public:
		const ENTITY_ID id;
		
		virtual void update();

		//Component access and edit

		template<typename COMPONENT_TYPE, IS_COMPONENT_TYPE<COMPONENT_TYPE> = 0>
		COMPONENT_TYPE& add_component();
		template<typename COMPONENT_TYPE, IS_COMPONENT_TYPE<COMPONENT_TYPE> = 0>
		void remove_component();
		template<typename COMPONENT_TYPE, IS_COMPONENT_TYPE<COMPONENT_TYPE> = 0>
		COMPONENT_TYPE* find_component();

		void move_component(UNIQUE_COMPONENT&&);
	};

}

//FUNCTIONS

//Adds component of the specified type and returns a reference
template<typename COMPONENT_TYPE, SWIFT::ENGINE::IS_COMPONENT_TYPE<COMPONENT_TYPE>>
inline COMPONENT_TYPE& SWIFT::ENGINE::ENTITY::add_component()
{
	auto ptr = new COMPONENT_TYPE();
	m_components.push_back(UNIQUE_COMPONENT(ptr));
	return *ptr;
}

//Removes the first encountered component of the specified type
template<typename COMPONENT_TYPE, SWIFT::ENGINE::IS_COMPONENT_TYPE<COMPONENT_TYPE>>
inline void SWIFT::ENGINE::ENTITY::remove_component()
{
	auto begin = m_components.begin();
	auto end = m_components.end();
	for (auto it = begin; it != end; ++it)
	{
		auto& component = *it;
		if (component->is_type(get_type_id<COMPONENT_TYPE>()))
		{
			m_components.erase(it);
			break;
		}
	}
}

//Returns the first encountered component of the specified type
template<typename COMPONENT_TYPE, SWIFT::ENGINE::IS_COMPONENT_TYPE<COMPONENT_TYPE>>
inline COMPONENT_TYPE* SWIFT::ENGINE::ENTITY::find_component()
{
	auto begin = m_components.begin();
	auto end = m_components.end();
	for (auto it = begin; it != end; ++it)
	{
		auto& component = *it;
		if (component->is_type(get_type_id<COMPONENT_TYPE>()))
		{
			return static_cast<COMPONENT_TYPE*>(component.get());
		}
	}

	return nullptr;
}
