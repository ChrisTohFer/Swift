#include "component.h"

SWIFT::EC::COMPONENT_BASE::COMPONENT_BASE(TYPE_ID id_arg)
	: type_id(id_arg)
{}

void SWIFT::EC::COMPONENT_BASE::serialise(IO::SERIALISER& serialiser)
{
	auto simple = make_serialisable();
	serialiser.serialise(*simple);
}

void SWIFT::EC::COMPONENT_BASE::deserialise(IO::DESERIALISER& deserialiser)
{
	auto simple = make_serialisable();
	deserialiser.deserialise(*simple);
}

bool SWIFT::EC::COMPONENT_BASE::is_type(TYPE_ID other_type_id)
{
	return other_type_id == type_id;
}