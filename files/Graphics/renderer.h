#pragma once

#include "render_scene.h"

#include "Multimedia/render_interface.h"

namespace SWIFT
{
	class RENDERER : public RENDER_INTERFACE
	{
		class IMPL;

	public:
		~RENDERER();

		//overrides for abstract functions
		void add_parent_window(sf::RenderWindow&) override;
		void remove_parent_window()               override;

		void update_scene(RENDER_SCENE&&);

	private:
		IMPL* m_impl = nullptr;
	};
}