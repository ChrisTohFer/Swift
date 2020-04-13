#pragma once

#include "Types/include/id.h"

#include <type_traits>
#include <memory>
#include <map>

namespace SWIFT::EC
{
	//Types
	class COMPONENT_BASE;
	using COMPONENT_ID = ID<COMPONENT_BASE>;
	using UNIQUE_COMPONENT = std::unique_ptr<COMPONENT_BASE>;

	//Base class for component, provides access to functions
	class COMPONENT_BASE
	{
	protected:
		using COMPONENT_MAP = std::map<const char*, UNIQUE_COMPONENT>;
		static COMPONENT_MAP& component_type_map();

		const TYPE_ID type_id;
		const COMPONENT_ID id;

	public:
		COMPONENT_BASE(TYPE_ID);
		virtual ~COMPONENT_BASE() = default;

		static UNIQUE_COMPONENT create_from_name(const char*);

		virtual UNIQUE_COMPONENT create_new() = 0;
		virtual const char* name() const = 0;
		virtual void update() {};

		bool is_type(TYPE_ID);
	};


	//crtp base class for components
	template<typename DERIVED>
	class COMPONENT : public COMPONENT_BASE
	{
		static bool static_initialize();
		static inline bool dummy_var = static_initialize();	//Initialized during static initialization, allowing us to call static_initialize for every component type
	
	public:
		COMPONENT();
		virtual UNIQUE_COMPONENT create_new();
	};
}

//DEFINITIONS

template<typename DERIVED>
bool SWIFT::EC::COMPONENT<DERIVED>::static_initialize()
{
	auto ptr = UNIQUE_COMPONENT(new DERIVED());
	auto name = ptr->name();
	component_type_map().emplace(std::make_pair(name, std::move(ptr)));
	return true;
}

template<typename DERIVED>
SWIFT::EC::COMPONENT<DERIVED>::COMPONENT()
	: COMPONENT_BASE(get_type_id<DERIVED>())
{}

template<typename DERIVED>
SWIFT::EC::UNIQUE_COMPONENT SWIFT::EC::COMPONENT<DERIVED>::create_new()
{
	return UNIQUE_COMPONENT(new DERIVED());
}
