#pragma once

#include "SFML/Graphics.hpp"

#include <mutex>

namespace SWIFT
{
	struct BACKEND_WINDOW : public sf::RenderWindow
	{};
	struct BACKEND_VERTEX_ARRAY : public sf::VertexArray
	{};
}
