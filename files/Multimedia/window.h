#pragma once

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
		void create_fullscreen(int monitor_index = -1);
		void close_window();

		const wchar_t* title() const;
		bool is_open() const;

	private:
		IMPL* m_window = nullptr;
		const wchar_t* m_title = L"";
	};

}