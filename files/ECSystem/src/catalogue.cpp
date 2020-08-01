#include "GlobalHeaders/macros.h"
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
	if (map().find(component->name()) != map().end())
	{
		CONSOLE_WARNING(L"Same name added to component catalogue twice, check that all names are unique!");	//TODO make this a popup/fatal error
	}
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
	auto component_iterator = map().find(name);
	if (component_iterator != map().end())
		return component_iterator->second->create_new();
	else
		return nullptr;
}