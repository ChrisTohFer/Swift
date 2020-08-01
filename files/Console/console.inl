#pragma once

#include "console.h"

namespace SWIFT{

    // CONSOLE //

    template<typename ...ARGS>
    inline bool CONSOLE::add_console_command(std::wstring command, std::wstring description, void(*func)(ARGS...))
    {
        m_mutex.lock();

        bool success;

        auto test_itr = m_command_definitions.find(command);
        if (test_itr != m_command_definitions.end())
            success = false;
        else
        {
            success = true;
            m_command_definitions.emplace(command, std::make_unique<COMMAND_DEFINITION_NONMEMBER<ARGS...>>(description, func));
        }

        m_mutex.unlock();
        return success;
    }

    template<typename T, typename ...ARGS>
    inline bool CONSOLE::add_console_command(std::wstring command, std::wstring description, T& target, void(T::* func)(ARGS...))
    {
        m_mutex.lock();

        bool success;

        auto test_itr = m_command_definitions.find(command);
        if (test_itr != m_command_definitions.end())
            success = false;
        else
        {
            success = true;
            m_command_definitions.emplace(command, std::make_unique<COMMAND_DEFINITION_MEMBER<T, ARGS...>>(description, &target, func));
        }

        m_mutex.unlock();
        return success;
    }

    // COMMAND_DEFINITION_NONMEMBER //

    template<typename ...ARGS>
    inline CONSOLE::COMMAND_DEFINITION_NONMEMBER<ARGS...>::COMMAND_DEFINITION_NONMEMBER(std::wstring description, void(*func)(ARGS...))
    {
        this->description = description;
        m_fn = func;
    }

    template<typename ...ARGS>
    inline bool CONSOLE::COMMAND_DEFINITION_NONMEMBER<ARGS...>::register_command(CONSOLE& console, [[maybe_unused]] std::wistream& command_stream)
    {
        auto command = std::make_unique<COMMAND_NONMEMBER<ARGS...>>(m_fn);
        if constexpr (sizeof...(ARGS) != 0)
        {
            if (!command->read_arguments_from_stream_recursive<0, sizeof...(ARGS)>(command_stream))
                return false;
        }

        console.m_commands.push_back(std::move(command));
        return true;
    }

    // COMMAND_DEFINITION_MEMBER //

    template<typename T, typename ...ARGS>
    inline CONSOLE::COMMAND_DEFINITION_MEMBER<T, ARGS...>::COMMAND_DEFINITION_MEMBER(std::wstring description, T* target, void(T::* func)(ARGS...))
    {
        this->description = description;
        m_target = target;
        m_fn = func;
    }

    template<typename T, typename ...ARGS>
    inline bool CONSOLE::COMMAND_DEFINITION_MEMBER<T, ARGS...>::register_command(CONSOLE& console, [[maybe_unused]] std::wistream& command_stream)
    {
        auto command = std::make_unique<COMMAND_MEMBER<T, ARGS...>>(m_target, m_fn);
        if constexpr (sizeof...(ARGS) != 0)
        {
            if (!command->read_arguments_from_stream_recursive<0, sizeof...(ARGS)>(command_stream))
                return false;
        }

        console.m_commands.push_back(std::move(command));
        return true;
    }

    // COMMAND_NONMEMBER //

    template<typename ...ARGS>
    inline CONSOLE::COMMAND_NONMEMBER<ARGS...>::COMMAND_NONMEMBER(void(*func)(ARGS...))
        : m_fn(func)
    {}

    template<typename ...ARGS>
    inline void CONSOLE::COMMAND_NONMEMBER<ARGS...>::invoke()
    {
        std::apply(m_fn, m_arguments);
    }

    template<typename ...ARGS>
    template<int INDEX, int COUNT>
    inline bool CONSOLE::COMMAND_NONMEMBER<ARGS...>::read_arguments_from_stream_recursive(std::wistream& command_stream)
    {
        command_stream >> std::get<INDEX>(m_arguments);

        if (command_stream.fail())
        {
            command_stream.clear();
            return false;
        }
        else if constexpr (INDEX + 1 == COUNT)
        {
            return true;
        }
        else
            return read_arguments_from_stream_recursive<INDEX + 1, COUNT>(command_stream);
    }

    // COMMAND_MEMBER //

    template<typename T, typename ...ARGS>
    inline CONSOLE::COMMAND_MEMBER<T, ARGS...>::COMMAND_MEMBER(T* target, void(T::* func)(ARGS...))
        : m_target(target)
        , m_fn(func)
    {}

    template<typename T, typename ...ARGS>
    inline void CONSOLE::COMMAND_MEMBER<T, ARGS...>::invoke()
    {
        auto lambda = [&](ARGS... args)
        {
            (m_target->*m_fn)(args...);
        };
        std::apply(lambda, m_arguments);
    }

    template<typename T, typename ...ARGS>
    template<int INDEX, int COUNT>
    inline bool CONSOLE::COMMAND_MEMBER<T, ARGS...>::read_arguments_from_stream_recursive(std::wistream& command_stream)
    {
        command_stream >> std::get<INDEX>(m_arguments);

        if (command_stream.fail())
        {
            command_stream.clear();
            return false;
        }
        else if constexpr (INDEX + 1 == COUNT)
        {
            return true;
        }
        else
            return read_arguments_from_stream_recursive<INDEX + 1, COUNT>(command_stream);
    }

}