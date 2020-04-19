#pragma once

#include "Types/include/id.h"
#include "FileIO/include/serialisation.h"

#include <type_traits>
#include <memory>
#include <map>
#include <string>

namespace SWIFT::EC
{
	//Types
	class COMPONENT_BASE;
	using COMPONENT_ID = ID<COMPONENT_BASE>;
	using UNIQUE_COMPONENT = std::unique_ptr<COMPONENT_BASE>;
	using NAME_TYPE = std::string;

	//Forward declarations
	namespace CATALOGUE
	{
		void add_component(UNIQUE_COMPONENT);
	}

	//Base class for component, provides access to generic functions when the component type is not known
	class COMPONENT_BASE : public IO::SERIALISABLE
	{
	protected:
		const TYPE_ID type_id;
		const COMPONENT_ID id;

	public:
		COMPONENT_BASE(TYPE_ID);
		virtual ~COMPONENT_BASE() = default;

		virtual UNIQUE_COMPONENT create_new() = 0;
		virtual NAME_TYPE name() const = 0;
		virtual void update() {};
		
		virtual IO::UNIQUE_SERIALISABLE make_serialisable() = 0;
		virtual void serialise(IO::SERIALISER&) final;
		virtual void deserialise(IO::DESERIALISER&) final;

		bool is_type(TYPE_ID);
	};

	//crtp base class for components
	template<typename DERIVED>
	class COMPONENT : public COMPONENT_BASE
	{
		static bool static_initialize();
		static inline bool dummy_var = static_initialize();	//Dummy var allows us to call static_initialize for every derived type
	
	public:
		COMPONENT();
		virtual UNIQUE_COMPONENT create_new();
	};
}

//DEFINITIONS

template<typename DERIVED>
bool SWIFT::EC::COMPONENT<DERIVED>::static_initialize()
{
	CATALOGUE::add_component(UNIQUE_COMPONENT(new DERIVED()));
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
