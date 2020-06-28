#pragma once

#include "renderer.h"

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

		const wchar_t* title() const;
		bool is_open() const;

	private:
		void create_impl(const wchar_t*, bool, int, int, int, int);

		IMPL*          m_window = nullptr;
		const wchar_t* m_title = L"";
		RENDERER       m_renderer;
		bool		   m_ready_to_close = false;
	};

}