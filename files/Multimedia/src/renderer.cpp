#include "renderer.h"
#include "internal_types.h"

#include "SFML/Graphics.hpp"

#include <thread>

class SWIFT::RENDERER::IMPL
{
public:
	IMPL(BACKEND_WINDOW& window);
	~IMPL();

private:
	void render_loop();

	std::thread       m_thread;
	bool              m_running = true;
	BACKEND_WINDOW&   m_window;
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

#include <cmath>
#include <chrono>
void SWIFT::RENDERER::IMPL::render_loop()
{
	sf::CircleShape circle;
	circle.setFillColor(sf::Color::Green);
	circle.setRadius(50.f);

	auto start_time = std::chrono::system_clock::now();
	while (m_running)
	{
		auto now = std::chrono::system_clock::now();
		auto time = now - start_time;
		auto time_millis = std::chrono::duration_cast<std::chrono::milliseconds>(time);
		circle.setRadius(50.f * abs(std::cos(time_millis.count() * 0.001f)));

		m_window.clear(sf::Color::Black);
		m_window.draw(circle);
		m_window.display();

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1ms);
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


