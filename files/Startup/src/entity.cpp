#include "entity.h"

void SWIFT::ENGINE::ENTITY::update()
{
	auto begin = m_components.begin();
	auto end = m_components.end();
	for (auto it = begin; it != end; ++it)
	{
		it->get()->update();
	}
}

//Moves an existing component onto this entity
void SWIFT::ENGINE::ENTITY::move_component(UNIQUE_COMPONENT&& component)
{
	if (component.get() == nullptr)
		return;

	m_components.push_back(std::move(component));
}