#include "component.h"

SWIFT::EC::COMPONENT_BASE::COMPONENT_BASE(TYPE_ID id_arg)
	: type_id(id_arg)
{}

bool SWIFT::EC::COMPONENT_BASE::is_type(TYPE_ID other_type_id)
{
	return other_type_id == type_id;
}