#pragma once

#include "renderer.h"
#include "input.h"

namespace SWIFT
{
	class WINDOW
	{
		class IMPL;

	public:
		WINDOW(const wchar_t* = L"Title");
		~WINDOW();

		void title(const wchar_t*);
		void create_window(int, int);
		void create_fullscreen();
		void close_window();
		void update(RENDER_SCENE const&);

		const wchar_t* title() const;
		bool is_open() const;
		INPUT& input();

	private:
		void create_impl(const wchar_t*, bool, int, int, int, int);

		RENDERER			  m_renderer;
		INPUT                 m_input;
		KEY_ARRAY<KEY_UPDATE> m_key_updates;
		MOUSE_UPDATE	      m_mouse_updates;

		IMPL*                 m_window = nullptr;
		const wchar_t*        m_title = L"";
		bool		          m_ready_to_close = false;
	};

}