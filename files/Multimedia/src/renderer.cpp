#include "renderer.h"
#include "internal_types.h"

#include "GlobalHeaders/timing.h"

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
#include "SFML/Graphics/Font.hpp"
void SWIFT::RENDERER::IMPL::render_loop()
{
	//temporary
	sf::Font font;
	font.loadFromFile("../../resources/fonts/arial.ttf");
	sf::Text cycle_time_text;
	sf::CircleShape circle;
	cycle_time_text.setFont(font);
	cycle_time_text.setFillColor(sf::Color::White);
	cycle_time_text.setCharacterSize(20);
	circle.setFillColor(sf::Color::Green);
	circle.setRadius(50.f);
	auto start_time = std::chrono::system_clock::now();
	//~temporary

	CYCLE_TIMER timer(100);
	while (m_running)
	{
		timer.cycle();

		//temporary
		auto temp = std::to_string(timer.average_cycle_time());
		cycle_time_text.setString(temp);
		auto now = std::chrono::system_clock::now();
		auto time = now - start_time;
		auto time_millis = std::chrono::duration_cast<std::chrono::milliseconds>(time);
		circle.setRadius(50.f * abs(std::cos(time_millis.count() * 0.001f)));
		//~temporary

		m_window.clear(sf::Color::Black);
		m_window.draw(circle);
		m_window.draw(cycle_time_text);
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


