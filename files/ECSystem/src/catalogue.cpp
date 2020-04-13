#include "catalogue.h"
#include <map>

namespace {
	using MAP = std::map<SWIFT::EC::NAME_TYPE, SWIFT::EC::UNIQUE_COMPONENT>;
	MAP& map()
	{
		static MAP map;
		return map;
	}
}

void SWIFT::EC::CATALOGUE::add_component(UNIQUE_COMPONENT component)
{
	map().emplace(component->name(), std::move(component));
}

std::vector<SWIFT::EC::NAME_TYPE> SWIFT::EC::CATALOGUE::names()
{
	std::vector<SWIFT::EC::NAME_TYPE> namelist;

	for (auto it = map().begin(); it != map().end(); ++it)
	{
		namelist.push_back(it->first);
	}

	return namelist;
}

SWIFT::EC::UNIQUE_COMPONENT SWIFT::EC::CATALOGUE::create_new(const NAME_TYPE& name)
{
	if (map().find(name) != map().end())
		return map().operator[](name)->create_new();
	else
		return nullptr;
}