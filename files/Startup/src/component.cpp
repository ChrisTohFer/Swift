#include "component.h"

bool SWIFT::ENGINE::COMPONENT::is_type(TYPE_ID other_type_id)
{
	return other_type_id == type_id;
}
