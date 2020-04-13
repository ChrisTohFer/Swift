#pragma once

#include "component.h"
#include <vector>

namespace SWIFT::EC::CATALOGUE
{
	//CATALOGUE is used to keep track of all different component types and provide a way to instantiate them
	void					add_component(UNIQUE_COMPONENT);
	std::vector<NAME_TYPE>	names();
	UNIQUE_COMPONENT		create_new(const NAME_TYPE&);
}