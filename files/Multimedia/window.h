#pragma once

namespace SWIFT
{

	class WINDOW
	{
		class IMPL;

	public:
		WINDOW(const char* = "Title");
		~WINDOW();

		void title(const char*);
		void create_window(int, int);
		void create_fullscreen(int monitor_index = -1);
		void close_window();

		const char* title() const;
		bool is_open() const;

	private:
		IMPL* m_window = nullptr;
		const char* m_title = "";
	};

}