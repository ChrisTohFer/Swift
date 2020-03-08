#pragma once

#include "Types/include/id.h"

namespace SWIFT::ENGINE
{
	class COMPONENT;
	using COMPONENT_ID = ID<COMPONENT>;

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