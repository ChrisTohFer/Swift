#include "window.h"
#include "SFML/Graphics.hpp"
#include <thread>

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
		IMPL(sf::VideoMode const&, const char*, bool borderless = false);
		~IMPL();

		void title(const char*);
		
		bool is_open() const;

	private:
		void window_loop(sf::VideoMode const&, const char*, bool);

		sf::RenderWindow m_window;
		std::thread m_windowThread;
		bool m_running = true;
		RENDERER m_renderer;
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

void SWIFT::WINDOW::IMPL::RENDERER::render_loop()
{
	auto& window = m_window_impl.m_window;

	while (m_window_impl.m_running)
	{
		window.clear(sf::Color::Black);
		window.display();

		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}

	window.setActive(false);
}

// IMPL //

SWIFT::WINDOW::IMPL::IMPL(sf::VideoMode const& video_mode, const char* titleArg, bool borderless)
	: m_renderer(*this)
{
	m_windowThread = std::thread(&IMPL::window_loop, this, video_mode, titleArg, borderless);
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

void SWIFT::WINDOW::IMPL::window_loop(sf::VideoMode const& video_mode, const char* titleArg, bool borderless)
{
	m_window.create(video_mode, titleArg, borderless ? sf::Style::None : sf::Style::Default);

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