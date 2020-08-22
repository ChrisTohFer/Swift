#pragma once

#include "render_scene.h"

namespace SWIFT
{
	class RENDERER
	{
		class IMPL;

	public:
		~RENDERER();

		void begin(struct BACKEND_WINDOW&);
		void stop();

		void update_scene(RENDER_SCENE&&);

	private:
		IMPL* m_impl = nullptr;
	};
}