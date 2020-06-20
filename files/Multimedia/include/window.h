#pragma once

namespace SWIFT
{

	class WINDOW
	{
		class IMPL;

	public:
		WINDOW();
		WINDOW(const char*);
		WINDOW(int, int, const char*);
		~WINDOW();

		void title(const char*);
		void create_window(int, int);
		void create_fullscreen();
		void close_window();

		const char* title() const;

	private:
		IMPL* m_window = nullptr;
		const char* m_title = "";
	};

}