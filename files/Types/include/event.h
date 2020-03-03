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

		void add_listener(LISTENER*);
		void remove_listener(LISTENER*);
		void remove_all_listeners();

		void invoke(PARAMS...) const;
	private:
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
		void stop_listening();
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

	//When moved, remove listeners from old event and add to this one
	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::EVENT(EVENT&& other) noexcept
	{
		auto begin = other.m_listeners.begin();
		auto end = other.m_listeners.end();

		for (auto listener_iter = begin; listener_iter != end; ++listener_iter)
		{
			auto& listener = *listener_iter;
			add_listener(listener);
			listener->m_listenees.erase(&other);
		}
		other.m_listeners.erase(begin, end);
	}

	//Inform listeners we are no longer reporting
	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::~EVENT()
	{
		remove_all_listeners();
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::add_listener(LISTENER* listener)
	{
		m_listeners.emplace(listener);
		listener->m_listenees.emplace(this);
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::remove_listener(LISTENER* listener)
	{
		m_listeners.erase(listener);
		listener->m_listenees.erase(this);
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::remove_all_listeners()
	{
		auto begin = m_listeners.begin();
		auto end = m_listeners.end();

		for (auto listener_iter = begin; listener_iter != end; ++listener_iter)
		{
			auto& listener = *listener_iter;
			listener->m_listenees.erase(this);
		}
		m_listeners.erase(begin, end);
	}

	template<typename ... PARAMS>
	inline void EVENT<PARAMS...>::invoke(PARAMS... params) const
	{
		auto begin = m_listeners.begin();
		auto end = m_listeners.end();

		for (auto listener_iter = begin; listener_iter != end; ++listener_iter)
		{
			auto& listener = *listener_iter;
			listener->notify(params...);
		}
	}

	//Listener

	//When copied, listen to the same events
	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::LISTENER::LISTENER(const LISTENER& other)
	{
		auto begin = other.m_listenees.begin();
		auto end = other.m_listenees.end();

		for (auto listenee_iter = begin; listenee_iter != end; ++listenee_iter)
		{
			auto& listenee = *listenee_iter;
			listen(listenee);
		}
	}

	//When moved, stop old version from listening and start listening
	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::LISTENER::LISTENER(LISTENER&& other) noexcept
	{
		auto begin = other.m_listenees.begin();
		auto end = other.m_listenees.end();

		for (auto listenee_iter = begin; listenee_iter != end; ++listenee_iter)
		{
			auto& listenee = *listenee_iter;
			listen(listenee);
			listenee->m_listeners.erase(&other);
		}
		other.m_listenees.erase(begin, end);
	}

	template<typename ...PARAMS>
	inline EVENT<PARAMS...>::LISTENER::~LISTENER()
	{
		stop_listening();
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::LISTENER::listen(EVENT* evt)
	{
		evt->m_listeners.emplace(this);
		m_listenees.emplace(evt);
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::LISTENER::stop_listening(EVENT* evt)
	{
		evt->m_listeners.erase(this);
		m_listenees.erase(evt);
	}

	template<typename ...PARAMS>
	inline void EVENT<PARAMS...>::LISTENER::stop_listening()
	{
		auto begin = m_listenees.begin();
		auto end = m_listenees.end();

		for (auto listenee_iter = begin; listenee_iter != end; ++listenee_iter)
		{
			auto& listenee = *listenee_iter;
			listenee->m_listeners.erase(this);
		}
		m_listenees.erase(begin, end);
	}

}

