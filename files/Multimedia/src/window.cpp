#include "window.h"
#include "SFML/Graphics.hpp"
#include <thread>

namespace SWIFT
{
	class WINDOW::IMPL
	{
	public:
		IMPL(sf::VideoMode const&, const char*, bool borderless = false);
		~IMPL();

		void title(const char*);

	private:
		void window_loop(sf::VideoMode const&, const char*, bool);

		sf::RenderWindow m_window;
		std::thread m_thread;
		bool m_running = true;
	};
}

// WINDOW //

SWIFT::WINDOW::WINDOW()
{
}

SWIFT::WINDOW::WINDOW(const char* titleArg)
{
	title(titleArg);
	create_fullscreen();
}

SWIFT::WINDOW::WINDOW(int size_x, int size_y, const char* titleArg)
{
	title(titleArg);
	create_window(size_x, size_y);
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
	m_window = new IMPL(video_mode, m_title);
}

void SWIFT::WINDOW::create_fullscreen()
{
	if (m_window)
		close_window();
	
	auto video_mode = sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	m_window = new IMPL(video_mode, m_title, true);
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

// IMPL //

SWIFT::WINDOW::IMPL::IMPL(sf::VideoMode const& video_mode, const char* titleArg, bool borderless)
	: m_thread(&IMPL::window_loop, this, video_mode, titleArg, borderless)
{}

SWIFT::WINDOW::IMPL::~IMPL()
{
	m_running = false;
	m_thread.join();
}

void SWIFT::WINDOW::IMPL::title(const char* titleArg)
{
	m_window.setTitle(titleArg);
}

void SWIFT::WINDOW::IMPL::window_loop(sf::VideoMode const& video_mode, const char* titleArg, bool borderless)
{
	m_window.create(video_mode, titleArg, borderless ? sf::Style::None : sf::Style::Default);
	
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
				auto visibleArea = sf::FloatRect(0, 0, event.size.width, event.size.height);
				auto view = sf::View(visibleArea);
				m_window.setView(view);
				break;
			}
		}

		m_window.clear(sf::Color::Black);
		m_window.display();
	}

	m_window.close();
}