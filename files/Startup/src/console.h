#pragma once

#include <thread>
#include <mutex>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <memory>

namespace SWIFT
{

    class CONSOLE
    {
        //Command definitions represent functions which we've added to the console that the user can call via a string
        struct COMMAND_DEFINITION;
        template<typename ...ARGS>
        struct COMMAND_DEFINITION_NONMEMBER;
        template<typename T, typename ...ARGS>
        struct COMMAND_DEFINITION_MEMBER;

        //Commands represent requests that the user has made which need to be invoked by the main thread
        struct COMMAND;
        template<typename ...ARGS>
        struct COMMAND_NONMEMBER;
        template<typename T, typename ...ARGS>
        struct COMMAND_MEMBER;

        using COMMAND_DEFINITION_MAP = std::map<std::wstring, std::unique_ptr<COMMAND_DEFINITION>>;
        using COMMANDS = std::vector<std::unique_ptr<COMMAND>>;

    public:
        //Functions

        CONSOLE(std::wistream&, std::wostream&);
        ~CONSOLE();

        template<typename ...ARGS>
        bool add_console_command(std::wstring command, std::wstring description, void(*func)(ARGS...));
        template<typename T, typename ...ARGS>
        bool add_console_command(std::wstring command, std::wstring description, T& target, void(T::*func)(ARGS...));
        void remove_console_command(std::wstring command);

        void invoke_commands();
        void output(std::wstring const& msg);

    private:
        void console_loop();
        void check_input();
        void output_command_map();

        std::thread              m_thread;
        std::mutex               m_mutex;
        bool                     m_running = true;
        COMMAND_DEFINITION_MAP   m_command_definitions;
        COMMANDS                 m_commands;
        std::wistream&           m_istream;
        std::wostream&           m_ostream;
    };

    // Command definitions //

    //Base class for storing pointers
    struct CONSOLE::COMMAND_DEFINITION
    {
        virtual ~COMMAND_DEFINITION() = default;
        virtual bool register_command(CONSOLE&, std::wistream&) = 0;

        std::wstring description;
    };

    //Implementation for non member functions
    template<typename ...ARGS>
    struct CONSOLE::COMMAND_DEFINITION_NONMEMBER : public COMMAND_DEFINITION
    {
        COMMAND_DEFINITION_NONMEMBER(std::wstring description, void(*func)(ARGS...));
        bool register_command(CONSOLE&, std::wistream&) override;

    private:
        void(*m_fn)(ARGS...);                 //Function pointer
    };

    //Implementation for member functions
    //Assumes that the target will continue to exist and not be moved in memory from the point that it is added to the console
    template<typename T, typename ...ARGS>
    struct CONSOLE::COMMAND_DEFINITION_MEMBER : public COMMAND_DEFINITION
    {
        COMMAND_DEFINITION_MEMBER(std::wstring description, T* target, void(T::* func)(ARGS...));
        bool register_command(CONSOLE&, std::wistream&) override;

    private:
        T* m_target;
        void(T::* m_fn)(ARGS...);                 //Function pointer
    };

    // Commands //

    struct CONSOLE::COMMAND
    {
        virtual ~COMMAND() = default;
        virtual void invoke() = 0;
    };

    template<typename ...ARGS>
    struct CONSOLE::COMMAND_NONMEMBER : public COMMAND
    {
        COMMAND_NONMEMBER(void(*func)(ARGS...));

        void invoke() override;
        template<int INDEX, int COUNT>
        bool read_arguments_from_stream_recursive(std::wistream&);

        void(*m_fn)(ARGS...);                 //Function pointer
        std::tuple<ARGS...> m_arguments;
    };

    template<typename T, typename ...ARGS>
    struct CONSOLE::COMMAND_MEMBER : public COMMAND
    {
        COMMAND_MEMBER(T* target, void(T::* func)(ARGS...));

        void invoke() override;
        template<int INDEX, int COUNT>
        bool read_arguments_from_stream_recursive(std::wistream&);

    private:
        T* m_target;
        void(T::* m_fn)(ARGS...);                 //Function pointer
        std::tuple<ARGS...> m_arguments;
    };

    //Functions

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