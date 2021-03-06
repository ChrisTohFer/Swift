#pragma once

#include "render_interface.h"
#include "input.h"

#include "Types/vector.h"

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
		void update();

		void add_renderer(RENDER_INTERFACE&);
		void remove_renderer();

		const wchar_t* title() const;
		bool is_open() const;
		SWIFT::VECTOR2F size() const;
		INPUT& input();

	private:
		void create_impl(const wchar_t*, bool, int, int, int, int);

		RENDER_INTERFACE*     m_renderer = nullptr;
		INPUT                 m_input;
		KEY_ARRAY<KEY_UPDATE> m_key_updates;
		MOUSE_UPDATE	      m_mouse_updates;

		IMPL*                 m_window = nullptr;
		const wchar_t*        m_title = L"";
		bool		          m_ready_to_close = false;
	};

}