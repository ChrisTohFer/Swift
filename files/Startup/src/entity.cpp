#include "entity.h"

namespace SWIFT::ENGINE
{

	void ENTITY::update()
	{
		auto begin = m_components.begin();
		auto end = m_components.end();
		for (auto it = begin; it != end; ++it)
		{
			it->get()->update();
		}
	}
}