#include "renderer.h"

#include "GlobalHeaders/timing.h"

#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Font.hpp"

#include <thread>
#include <mutex>

class SWIFT::RENDERER::IMPL
{
public:
	IMPL(sf::RenderWindow& window);
	~IMPL();

	void update_scene(RENDER_SCENE&&);

private:
	void render_loop();

	std::thread       m_thread;
	std::mutex        m_mutex;
	bool              m_running = true;
	sf::RenderWindow& m_window;
	RENDER_SCENE	  m_scene;
};

// RENDERER::IMPL //

SWIFT::RENDERER::IMPL::IMPL(sf::RenderWindow& window)
	: m_window(window)
{
	m_thread = std::thread(&RENDERER::IMPL::render_loop, this);
}

SWIFT::RENDERER::IMPL::~IMPL()
{
	m_running = false;
	m_thread.join();
}

void SWIFT::RENDERER::IMPL::update_scene(RENDER_SCENE&& scene)
{
	m_mutex.lock();
	m_scene = std::move(scene);
	m_mutex.unlock();
}

void SWIFT::RENDERER::IMPL::render_loop()
{
	CYCLE_TIMER timer(100);
	while (m_running)
	{
		timer.cycle();

		m_window.clear(sf::Color::Black);
		{
			m_mutex.lock();
			m_scene.draw(m_window);	//Scene is protected by mutex as it is provided by main thread
			m_mutex.unlock();
		}
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
	remove_parent_window();
}

void SWIFT::RENDERER::add_parent_window(sf::RenderWindow& window)
{
	m_impl = new IMPL(window);
}

void SWIFT::RENDERER::remove_parent_window()
{
	delete m_impl;
	m_impl = nullptr;
}

void SWIFT::RENDERER::update_scene(RENDER_SCENE&& scene)
{
	if(m_impl)
		m_impl->update_scene(std::move(scene));
}


