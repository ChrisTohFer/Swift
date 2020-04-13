#include "entity.h"
#include "catalogue.h"
#include "GlobalHeaders/macros.h"

void SWIFT::EC::ENTITY::update()
{
	auto begin = m_components.begin();
	auto end = m_components.end();
	for (auto it = begin; it != end; ++it)
	{
		it->get()->update();
	}
}

//Add a component onto this entity by name
void SWIFT::EC::ENTITY::add_component(const NAME_TYPE& name)
{
	auto ptr = CATALOGUE::create_new(name);
	if (ptr != nullptr)
	{
		move_component(std::move(ptr));
	}
	else
	{
		CONSOLE_WARNING("Attempted to add a non-existing component");
	}
}

//Moves an existing component onto this entity
void SWIFT::EC::ENTITY::move_component(UNIQUE_COMPONENT&& component)
{
	if (component.get() == nullptr)
		return;

	m_components.push_back(std::move(component));
}