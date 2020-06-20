#include "window.h"

#include "SFML/Graphics.hpp"
#include <Windows.h>
#include <thread>

#include <iostream>
#include <vector>

namespace
{
	BOOL CALLBACK Monitorenumproc(
		HMONITOR monitor,
		HDC,
		LPRECT,
		LPARAM vector
	)
	{
		std::vector<MONITORINFO>& monitors = *reinterpret_cast<std::vector<MONITORINFO>*>(vector);
		monitors.push_back(MONITORINFO());
		monitors.back().cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &monitors.back());
		
		return TRUE;
	}

	MONITORINFO monitor_info(int monitor_index)
	{
		std::vector<MONITORINFO> monitors;

		EnumDisplayMonitors(
			nullptr,
			nullptr,
			Monitorenumproc,
			LPARAM(&monitors)
		);

		if (monitor_index >= 0 && monitors.size() > size_t(monitor_index))
			return monitors[monitor_index];
		else
			return monitors[0];
	}
}

namespace SWIFT
{
	class WINDOW::IMPL
	{
		class RENDERER
		{
		public:
			RENDERER(IMPL& window);

			void begin();
			void wait_for_end();

		private:
			void render_loop();

			IMPL& m_window_impl;
			std::thread m_renderThread;
		};

	public:
		struct PARAMS
		{
			const char* title = "Title";
			sf::VideoMode videoMode = sf::VideoMode(100,100);
			bool borderless = false;
			int offsetX = 0;
			int offsetY = 0;
		};

		IMPL(PARAMS const&);
		~IMPL();

		void title(const char*);
		
		bool is_open() const;
		const HWND handle() const;

	private:
		void window_loop(PARAMS const&);

		sf::RenderWindow m_window;
		std::thread m_windowThread;
		bool m_running = true;
		RENDERER m_renderer;
	};
}

// WINDOW //

SWIFT::WINDOW::WINDOW(const char* titleArg)
{
	title(titleArg);
}

SWIFT::WINDOW::~WINDOW()
{
	close_window();
}

void SWIFT::WINDOW::title(const char* titleArg)
{
	m_title = titleArg;

	if (m_window)
	{
		m_window->title(titleArg);
	}
}

void SWIFT::WINDOW::create_window(int size_x, int size_y)
{
	if (m_window)
		close_window();

	auto video_mode = sf::VideoMode(size_x, size_y);
	m_window = new IMPL({m_title, video_mode});
}

void SWIFT::WINDOW::create_fullscreen(int monitor_index)
{
	//Find the monitor via windows
	HMONITOR monitor;
	MONITORINFO info;
	if (m_window && monitor_index == -1)
	{
		monitor = MonitorFromWindow(m_window->handle(), MONITOR_DEFAULTTONEAREST);
		info.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &info);
	}
	else
	{
		info = monitor_info(monitor_index);
	}

	//Close old window
	if (m_window)
		close_window();

	//Set dimensions of new window
	auto x = info.rcMonitor.left;
	auto y = info.rcMonitor.top;
	auto width = info.rcMonitor.right - info.rcMonitor.left;
	auto height = info.rcMonitor.bottom - info.rcMonitor.top;

	auto video_mode = sf::VideoMode(width, height);
	m_window = new IMPL({ m_title, video_mode, true, x, y });
}

void SWIFT::WINDOW::close_window()
{
	delete m_window;
	m_window = nullptr;
}

const char* SWIFT::WINDOW::title() const
{
	return m_title;
}

bool SWIFT::WINDOW::is_open() const
{
	return m_window && m_window->is_open();
}

// IMPL::RENDERER //

SWIFT::WINDOW::IMPL::RENDERER::RENDERER(IMPL& window)
	: m_window_impl(window)
{}

void SWIFT::WINDOW::IMPL::RENDERER::begin()
{
	m_renderThread = std::thread(&RENDERER::render_loop, this);
}

void SWIFT::WINDOW::IMPL::RENDERER::wait_for_end()
{
	m_renderThread.join();
}

#include <cmath>
#include <chrono>

void SWIFT::WINDOW::IMPL::RENDERER::render_loop()
{
	auto& window = m_window_impl.m_window;

	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Green);
	circle.setRadius(50.f);

	auto start_time = std::chrono::system_clock::now();
	while (m_window_impl.m_running)
	{
		auto now = std::chrono::system_clock::now();
		auto time = now - start_time;
		auto time_millis = std::chrono::duration_cast<std::chrono::milliseconds>(time);

		circle.setRadius(50.f * abs(std::cos(time_millis.count() * 0.001f)));

		window.clear(sf::Color::Black);
		window.draw(circle);
		window.display();

		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}

	window.setActive(false);
}

// IMPL //

SWIFT::WINDOW::IMPL::IMPL(PARAMS const& params)
	: m_renderer(*this)
{
	m_windowThread = std::thread(&IMPL::window_loop, this, params);
}

SWIFT::WINDOW::IMPL::~IMPL()
{
	m_running = false;
	m_windowThread.join();
}

void SWIFT::WINDOW::IMPL::title(const char* titleArg)
{
	m_window.setTitle(titleArg);
}

bool SWIFT::WINDOW::IMPL::is_open() const
{
	return m_window.isOpen();
}

const HWND SWIFT::WINDOW::IMPL::handle() const
{
	return m_window.getSystemHandle();
}

void SWIFT::WINDOW::IMPL::window_loop(PARAMS const& params)
{
	m_window.create(params.videoMode, params.title, params.borderless ? sf::Style::None : sf::Style::Default);
	
	//Only offset if we're in borderless fullscreen
	if (params.borderless)
		m_window.setPosition(sf::Vector2i(params.offsetX, params.offsetY));

	m_window.setActive(false);	//We don't render on this thread
	m_renderer.begin();

	while (m_running)
	{
		sf::Event event;

		while (m_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				m_running = false;
				break;
			case sf::Event::Resized:
				auto width = static_cast<float>(event.size.width);
				auto height = static_cast<float>(event.size.height);

				m_window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
				break;
			}
		}

		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}

	m_renderer.wait_for_end();
	m_window.close();
}