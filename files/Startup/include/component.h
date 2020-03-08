#pragma once

#include "Types/include/id.h"
#include <type_traits>

namespace SWIFT::ENGINE
{
	class COMPONENT;
	using COMPONENT_ID = ID<COMPONENT>;

	template<typename COMPONENT_TYPE>
	using IS_COMPONENT_TYPE = std::enable_if_t<std::is_base_of<COMPONENT, COMPONENT_TYPE>::value, int>;

	class COMPONENT
	{
		friend class ENTITY;
		TYPE_ID type_id;

	public:
		const COMPONENT_ID id;

		virtual void update() = 0;

		bool is_type(TYPE_ID);
	};

}