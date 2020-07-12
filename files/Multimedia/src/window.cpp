#include "window.h"
#include "internal_types.h"

#include "Platform/devices.h"

#include "SFML/Graphics.hpp"

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

namespace
{
	SWIFT::KEY convert_key(sf::Keyboard::Key key)
	{
		return SWIFT::KEY(static_cast<int>(key));
	}
}

namespace SWIFT
{
	class WINDOW::IMPL
	{
	public:
		IMPL(WINDOW&, const wchar_t*, bool, int, int, int, int);
		~IMPL();

		void title(const wchar_t*);

		bool is_open() const;
		const sf::WindowHandle handle() const;
		BACKEND_WINDOW& window();

		void lock_input_mutex();
		void unlock_input_mutex();

	private:
		void wait_for_initialization() const;
		void window_loop(const wchar_t*, bool, int, int, int, int);
		 
		WINDOW&	         m_parent;
		BACKEND_WINDOW   m_window;
		std::thread      m_thread;
		std::mutex       m_input_mutex;
		bool             m_running = false;
	};
}

// IMPL //

SWIFT::WINDOW::IMPL::IMPL(WINDOW& parent, const wchar_t* title, bool borderless, int x, int y, int width, int height)
	: m_parent(parent)
{
	m_thread = std::thread(&IMPL::window_loop, this, title, borderless, x, y, width, height);
	wait_for_initialization();	//Don't allow main thread to continue until window is properly set up
}

SWIFT::WINDOW::IMPL::~IMPL()
{
	m_running = false;
	m_thread.join();
}

void SWIFT::WINDOW::IMPL::title(const wchar_t* titleArg)
{
	m_window.setTitle(titleArg);
}

bool SWIFT::WINDOW::IMPL::is_open() const
{
	return m_window.isOpen();
}

const sf::WindowHandle SWIFT::WINDOW::IMPL::handle() const
{
	return m_window.getSystemHandle();
}

SWIFT::BACKEND_WINDOW& SWIFT::WINDOW::IMPL::window()
{
	return m_window;
}

void SWIFT::WINDOW::IMPL::lock_input_mutex()
{
	m_input_mutex.lock();
}

void SWIFT::WINDOW::IMPL::unlock_input_mutex()
{
	m_input_mutex.unlock();
}

void SWIFT::WINDOW::IMPL::wait_for_initialization() const
{
	using namespace std::chrono_literals;
	while (!m_running)
		std::this_thread::sleep_for(1ms);
}

void SWIFT::WINDOW::IMPL::window_loop(const wchar_t* title, bool borderless, int x, int y, int width, int height)
{
	m_window.create(sf::VideoMode(width, height), title, borderless ? sf::Style::None : sf::Style::Default);

	//Only offset if we're in borderless fullscreen
	if (borderless)
		m_window.setPosition(sf::Vector2i(x, y));

	//We don't render on this thread
	m_window.setActive(false);

	m_running = true;
	while (m_running && m_window.isOpen())
	{
		sf::Event event;

		while (m_window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				m_parent.m_ready_to_close = true;
				break;
			case sf::Event::Resized:
				{
					auto newWidth = static_cast<float>(event.size.width);
					auto newHeight = static_cast<float>(event.size.height);

					m_window.setView(sf::View(sf::FloatRect(0, 0, newWidth, newHeight)));
				}
				break;
			case sf::Event::KeyPressed:
				m_input_mutex.lock();
				
				{
					auto key = convert_key(event.key.code);
					auto& key_update = m_parent.m_key_updates[static_cast<int>(key)];

					key_update.changed_since_last_frame = !key_update.held;
					key_update.held = true;
					key_update.alt = event.key.alt;
					key_update.control = event.key.control;
					key_update.shift = event.key.shift;
					key_update.system = event.key.system;
				}

				m_input_mutex.unlock();
				break;
			case sf::Event::KeyReleased:
				m_input_mutex.lock();
				
				{
					auto key = convert_key(event.key.code);
					auto& key_update = m_parent.m_key_updates[static_cast<int>(key)];

					key_update.changed_since_last_frame = key_update.held;
					key_update.held = false;
				}

				m_input_mutex.unlock();
				break;
			}
		}

		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}

	m_running = false;
	m_window.close();
}

// WINDOW //

SWIFT::WINDOW::WINDOW(const wchar_t* titleArg)
{
	title(titleArg);
}

SWIFT::WINDOW::~WINDOW()
{
	close_window();
}

void SWIFT::WINDOW::title(const wchar_t* titleArg)
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
	create_impl(m_title, false, 0, 0, size_x, size_y);
}

void SWIFT::WINDOW::create_fullscreen()
{
	SWIFT::PLAT::MONITOR_DIMENSIONS dimensions;
	
	if (m_window)
	{
		dimensions = SWIFT::PLAT::current_monitor_dimensions(m_window->handle());
		close_window();
	}
	else
	{
		dimensions = SWIFT::PLAT::main_monitor_dimensions();
	}

	auto video_mode = sf::VideoMode(dimensions.width, dimensions.height);
	create_impl(m_title, true, dimensions.posX, dimensions.posY, dimensions.width, dimensions.height);
}

void SWIFT::WINDOW::close_window()
{
	m_renderer.stop();	//Must stop rendering before we destroy the window

	delete m_window;
	m_window = nullptr;

	m_ready_to_close = false;
}

void SWIFT::WINDOW::update(INPUT& input)
{
	if (m_window)
	{
		m_window->lock_input_mutex();
		input.update(m_key_updates);
		m_window->unlock_input_mutex();
	}

	if (m_ready_to_close)
		close_window();
}

const wchar_t* SWIFT::WINDOW::title() const
{
	return m_title;
}

bool SWIFT::WINDOW::is_open() const
{
	return m_window && m_window->is_open();
}

void SWIFT::WINDOW::create_impl(const wchar_t* title, bool borderless, int x, int y, int width, int height)
{
	m_window = new IMPL(*this , title, borderless, x, y, width, height);
	m_renderer.begin(m_window->window());
}
