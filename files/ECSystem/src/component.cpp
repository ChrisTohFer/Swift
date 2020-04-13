#include "component.h"

SWIFT::EC::COMPONENT_BASE::COMPONENT_MAP& SWIFT::EC::COMPONENT_BASE::component_type_map()
{
	static COMPONENT_MAP map;
	return map;
}

SWIFT::EC::COMPONENT_BASE::COMPONENT_BASE(TYPE_ID id_arg)
	: type_id(id_arg)
{}

bool SWIFT::EC::COMPONENT_BASE::is_type(TYPE_ID other_type_id)
{
	return other_type_id == type_id;
}

SWIFT::EC::UNIQUE_COMPONENT SWIFT::EC::COMPONENT_BASE::create_from_name(const char* component_name)
{
	if (component_type_map().find(component_name) == component_type_map().end())
		return nullptr;

	return component_type_map()[component_name]->create_new();
}
