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
	
	SWIFT::MOUSE_BUTTON convert_button(sf::Mouse::Button button)
	{
		return SWIFT::MOUSE_BUTTON(button);
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
		sf::WindowHandle handle() const;
		SWIFT::VECTOR2F size() const;
		BACKEND_WINDOW& window();

		void lock_input_mutex();
		void unlock_input_mutex();

	private:
		void wait_for_initialization() const;
		void window_loop(const wchar_t*, bool, int, int, int, int);
		 
		void handle_closed();
		void handle_resized(sf::Event const&);

		void handle_key_pressed(sf::Event const&);
		void handle_key_released(sf::Event const&);

		void handle_mouse_button_pressed(sf::Event const&);
		void handle_mouse_button_released(sf::Event const&);
		void handle_mouse_wheel_scrolled(sf::Event const&);
		void handle_mouse_moved(sf::Event const&);
		void handle_mouse_entered();
		void handle_mouse_left();

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

sf::WindowHandle SWIFT::WINDOW::IMPL::handle() const
{
	return m_window.getSystemHandle();
}

SWIFT::VECTOR2F SWIFT::WINDOW::IMPL::size() const
{
	auto s = m_window.getSize();
	return SWIFT::VECTOR2F(float(s.x), float(s.y));
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

	//View always 1.f by 1.f centered on origin
	m_window.setView(sf::View(sf::FloatRect(-0.5f, -0.5f, 1.f, 1.f)));

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
				handle_closed();
				break;
			case sf::Event::Resized:
				handle_resized(event);
				break;
			case sf::Event::KeyPressed:
				handle_key_pressed(event);
				break;
			case sf::Event::KeyReleased:
				handle_key_released(event);
				break;
			case sf::Event::MouseButtonPressed:
				handle_mouse_button_pressed(event);
				break;
			case sf::Event::MouseButtonReleased:
				handle_mouse_button_released(event);
				break;
			case sf::Event::MouseWheelScrolled:
				handle_mouse_wheel_scrolled(event);
				break;
			case sf::Event::MouseMoved:
				handle_mouse_moved(event);
				break;
			case sf::Event::MouseEntered:
				handle_mouse_entered();
				break;
			case sf::Event::MouseLeft:
				handle_mouse_left();
				break;
			default:
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

void SWIFT::WINDOW::IMPL::handle_closed()
{
	m_parent.m_ready_to_close = true;
}

void SWIFT::WINDOW::IMPL::handle_resized(sf::Event const& event)
{
}

void SWIFT::WINDOW::IMPL::handle_key_pressed(sf::Event const& event)
{
 	m_input_mutex.lock();

	auto key = convert_key(event.key.code);
	if (key == SWIFT::KEY::Unknown)
	{
		m_input_mutex.unlock();
		return;
	}
	auto& key_update = m_parent.m_key_updates[static_cast<int>(key)];

	key_update.changed_since_last_frame = !key_update.held;
	key_update.held = true;
	key_update.alt = event.key.alt;
	key_update.control = event.key.control;
	key_update.shift = event.key.shift;
	key_update.system = event.key.system;

	m_input_mutex.unlock();
}

void SWIFT::WINDOW::IMPL::handle_key_released(sf::Event const& event)
{
	m_input_mutex.lock();

	auto key = convert_key(event.key.code); 
	if (key == SWIFT::KEY::Unknown)
	{
		m_input_mutex.unlock();
		return;
	}

	auto& key_update = m_parent.m_key_updates[static_cast<int>(key)];

	key_update.changed_since_last_frame = key_update.held;
	key_update.held = false;

	m_input_mutex.unlock();
}

void SWIFT::WINDOW::IMPL::handle_mouse_button_pressed(sf::Event const& event)
{
	m_input_mutex.lock();

	m_parent.m_mouse_updates.events.push_back(
		{
			convert_button(event.mouseButton.button),
			event.mouseButton.x,
			event.mouseButton.y,
			true
		}
	);

	m_input_mutex.unlock();
}

void SWIFT::WINDOW::IMPL::handle_mouse_button_released(sf::Event const& event)
{
	m_input_mutex.lock();

	m_parent.m_mouse_updates.events.push_back(
		{
			convert_button(event.mouseButton.button),
			event.mouseButton.x,
			event.mouseButton.y,
			false
		}
	);

	m_input_mutex.unlock();
}

void SWIFT::WINDOW::IMPL::handle_mouse_wheel_scrolled(sf::Event const& event)
{
	m_input_mutex.lock();

	m_parent.m_mouse_updates.wheelScroll += event.mouseWheelScroll.y;

	m_input_mutex.unlock();
}

void SWIFT::WINDOW::IMPL::handle_mouse_moved(sf::Event const& event)
{
	m_input_mutex.lock();

	m_parent.m_mouse_updates.posX = event.mouseMove.x;
	m_parent.m_mouse_updates.posY = event.mouseMove.y;

	m_input_mutex.unlock();
}

void SWIFT::WINDOW::IMPL::handle_mouse_entered()
{
	m_input_mutex.lock();

	m_parent.m_mouse_updates.in_window = true;

	m_input_mutex.unlock();
}

void SWIFT::WINDOW::IMPL::handle_mouse_left()
{
	m_input_mutex.lock();

	m_parent.m_mouse_updates.in_window = false;

	m_input_mutex.unlock();
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
	if (m_renderer)
		m_renderer->remove_parent_window(); //Must stop rendering before we destroy the window

	delete m_window;
	m_window = nullptr;

	m_ready_to_close = false;
}

void SWIFT::WINDOW::update()
{
	if (m_window)
	{
		m_window->lock_input_mutex();
		m_input.update(m_mouse_updates, m_key_updates);
		m_window->unlock_input_mutex();
	}

	if (m_ready_to_close)
		close_window();
}

void SWIFT::WINDOW::add_renderer(RENDER_INTERFACE& renderer)
{
	if (m_renderer)
		m_renderer->remove_parent_window();

	if(m_window)
		renderer.add_parent_window(m_window->window());

	m_renderer = &renderer;
}

void SWIFT::WINDOW::remove_renderer()
{
	if (m_renderer)
		m_renderer->remove_parent_window();

	m_renderer = nullptr;
}

const wchar_t* SWIFT::WINDOW::title() const
{
	return m_title;
}

bool SWIFT::WINDOW::is_open() const
{
	return m_window && m_window->is_open();
}

SWIFT::VECTOR2F SWIFT::WINDOW::size() const
{
	if (m_window)
		return m_window->size();
	else
		return SWIFT::VECTOR2F::zero();
}

SWIFT::INPUT& SWIFT::WINDOW::input() 
{
	return m_input;
}

void SWIFT::WINDOW::create_impl(const wchar_t* title, bool borderless, int x, int y, int width, int height)
{
	m_window = new IMPL(*this , title, borderless, x, y, width, height);

	if (m_renderer)
		m_renderer->add_parent_window(m_window->window());
}
