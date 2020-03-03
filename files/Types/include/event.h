#pragma once

#include <set>

namespace st
{

	//Event class can be invoked to call the notify() function on all listeners that are listening to it
	
	//Add/remove listeners by calling LISTENER::listen() and stop_listening()

	//LISTENER can listen to any number of different events

	template<typename ... PARAMS>
	class EVENT {
	public:
		class LISTENER;

		EVENT() = default;
		EVENT(const EVENT&);
		EVENT(EVENT&&) noexcept;
		~EVENT();

		void invoke(PARAMS...) const;
	private:
		void add_listener(LISTENER* listener);
		void remove_listener(LISTENER* listener);

		std::set<LISTENER*> m_listeners;
	};

	template<typename ... PARAMS>
	class EVENT<PARAMS...>::LISTENER {
		friend EVENT;
	public:
		LISTENER() = default;
		LISTENER(const LISTENER&);
		LISTENER(LISTENER&&) noexcept;
		virtual ~LISTENER();

		void listen(EVENT*);
		void stop_listening(EVENT*);
	protected:
		virtual void notify(PARAMS...) = 0;

	private:
		std::set<EVENT*> m_listenees;
	};

	//Functions

	//Event

	//When copied, do not copy listeners
	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::EVENT(const EVENT& other)
	{
	}

	//When moved, update the listeners to reflect this
	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::EVENT(EVENT&& other) noexcept
	{
		for (auto listener_iter = other.m_listeners.begin(); listener_iter != other.m_listeners.end(); ++listener_iter)
		{
			auto& listener = *listener_iter;
			listener->stop_listening(&other);
			listener->listen(this);
		}
	}

	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::~EVENT()
	{
		for (auto listener_iter = m_listeners.begin(); listener_iter != m_listeners.end(); ++listener_iter)
		{
			auto& listener = *listener_iter;
			listener->stop_listening(this);
		}
	}

	template<typename ... PARAMS>
	void EVENT<PARAMS...>::invoke(PARAMS... params) const
	{
		for (auto listener_iter = m_listeners.begin(); listener_iter != m_listeners.end(); ++listener_iter)
		{
			(*listener_iter)->notify(params...);
		}
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::add_listener(LISTENER* listener)
	{
		m_listeners.emplace(listener);
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::remove_listener(LISTENER* listener)
	{
		m_listeners.erase(listener);
	}

	//Listener

	//When copied, listen to the same events
	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::LISTENER::LISTENER(const LISTENER& other)
	{
		for (auto listenee_iter = other.m_listenees.begin(); listenee_iter != other.m_listenees.end(); ++listenee_iter)
		{
			auto& listenee = *listenee_iter;
			listenee->add_listener(this);
		}
	}

	//When moved, stop old version from listening and start listening
	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::LISTENER::LISTENER(LISTENER&& other) noexcept
	{
		for (auto listenee_iter = other.m_listenees.begin(); listenee_iter != other.m_listenees.end(); ++listenee_iter)
		{
			auto& listenee = *listenee_iter;
			listenee->remove_listener(&other);
			listenee->add_listener(this);
		}
	}

	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::LISTENER::~LISTENER()
	{
		for (auto listenee_iter = m_listenees.begin(); listenee_iter != m_listenees.end(); ++listenee_iter)
		{
			auto& listenee = *listenee_iter;
			listenee->remove_listener(this);
		}
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::LISTENER::listen(EVENT* evt)
	{
		evt->add_listener(this);
		m_listenees.emplace(evt);
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::LISTENER::stop_listening(EVENT* evt)
	{
		evt->remove_listener(this);
		m_listenees.erase(evt);
	}

}

