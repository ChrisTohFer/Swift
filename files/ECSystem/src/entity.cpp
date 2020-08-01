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
		CONSOLE_WARNING(L"Attempted to add a non-existing component");
	}
}

//Moves an existing component onto this entity
void SWIFT::EC::ENTITY::move_component(UNIQUE_COMPONENT&& component)
{
	if (component.get() == nullptr)
		return;

	m_components.push_back(std::move(component));
}

void SWIFT::EC::ENTITY::serialise(IO::SERIALISER& serialiser)
{
	size_t size = m_components.size();

	serialiser.serialise(size);
	for (size_t i = 0; i < size; ++i)
	{
		serialiser.serialise(std::string(m_components[i]->name()));
		serialiser.serialise(*m_components[i]);
		serialiser.add_separator();
	}
}

void SWIFT::EC::ENTITY::deserialise(IO::DESERIALISER& deserialiser)
{
	ASSERT(m_components.size() == 0);	//We don't want to read data into an entity that already has components

	size_t size;

	deserialiser.deserialise(size);
	for (size_t i = 0; i < size; ++i)
	{
		{
			std::string name;
			deserialiser.deserialise(name);
			m_components.push_back(CATALOGUE::create_new(name));
		}

		deserialiser.deserialise(*m_components[i]);
		deserialiser.next_separator();
	}
}
