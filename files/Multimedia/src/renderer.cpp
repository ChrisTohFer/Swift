#include "renderer.h"
#include "internal_types.h"

#include "GlobalHeaders/timing.h"

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Font.hpp"

#include <thread>
#include <mutex>

class SWIFT::RENDERER::IMPL
{
public:
	IMPL(BACKEND_WINDOW& window);
	~IMPL();

	void update_scene(RENDER_SCENE const&);

private:
	void render_loop();

	std::thread       m_thread;
	std::mutex        m_mutex;
	bool              m_running = true;
	BACKEND_WINDOW&   m_window;
	RENDER_SCENE	  m_scene;
};

// RENDERER::IMPL //

SWIFT::RENDERER::IMPL::IMPL(BACKEND_WINDOW& window)
	: m_window(window)
{
	m_thread = std::thread(&RENDERER::IMPL::render_loop, this);
}

SWIFT::RENDERER::IMPL::~IMPL()
{
	m_running = false;
	m_thread.join();
}

void SWIFT::RENDERER::IMPL::update_scene(RENDER_SCENE const& scene)
{
	m_mutex.lock();
	m_scene = scene;
	m_mutex.unlock();
}

void SWIFT::RENDERER::IMPL::render_loop()
{
	//temporary
	sf::Font font;
	font.loadFromFile("../../resources/fonts/arial.ttf");
	sf::Text cycle_time_text;
	cycle_time_text.setFont(font);
	cycle_time_text.setFillColor(sf::Color::White);
	cycle_time_text.setCharacterSize(20);
	//~temporary

	CYCLE_TIMER timer(100);
	while (m_running)
	{
		timer.cycle();

		//temporary
		auto temp = std::to_string(timer.average_cycle_time());
		cycle_time_text.setString(temp);
		auto now = std::chrono::system_clock::now();
		//~temporary

		m_window.clear(sf::Color::Black);
		{
			m_mutex.lock();
			m_scene.draw(m_window);	//Scene is protected by mutex as it is provided by main thread
			m_mutex.unlock();
		}
		m_window.draw(cycle_time_text);
		m_window.display();

		//If we've drawn the scene already then wait
		while (m_running && m_scene.drawn())
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1ms);
		}
	}

	m_window.setActive(false);
}

// RENDERER //

SWIFT::RENDERER::~RENDERER()
{
	stop();
}

void SWIFT::RENDERER::begin(BACKEND_WINDOW& window)
{
	m_impl = new IMPL(window);
}

void SWIFT::RENDERER::stop()
{
	delete m_impl;
	m_impl = nullptr;
}

void SWIFT::RENDERER::update_scene(RENDER_SCENE const& scene)
{
	if(m_impl)
		m_impl->update_scene(scene);
}


