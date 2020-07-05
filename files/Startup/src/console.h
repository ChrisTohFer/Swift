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
        //Inner types

        struct COMMAND_DEFINITION
        {
            virtual ~COMMAND_DEFINITION() = default;
            virtual bool register_command(CONSOLE&, std::istream&) = 0;

            std::string description;
        };

        template<typename ...ARGS>
        struct COMMAND_DEFINITION_IMPL : public COMMAND_DEFINITION
        {
            COMMAND_DEFINITION_IMPL(std::string description, void(*func)(ARGS...));
            bool register_command(CONSOLE&, std::istream&) override;

        private:
            void(*fn)(ARGS...);                 //Function pointer
        };

        struct COMMAND
        {
            virtual ~COMMAND() = default;
            virtual void invoke() = 0;
        };

        template<typename ...ARGS>
        struct COMMAND_IMPL : public COMMAND
        {
            COMMAND_IMPL(void(*func)(ARGS...));

            void invoke() override;
            template<int INDEX, int COUNT>
            bool read_arguments_from_stream_recursive(std::istream&);

            void(*fn)(ARGS...);                 //Function pointer
            std::tuple<ARGS...> arguments;
        };

        using COMMAND_DEFINITION_MAP = std::map<std::string, std::unique_ptr<COMMAND_DEFINITION>>;
        using COMMANDS = std::vector<std::unique_ptr<COMMAND>>;
        using OUTPUTS = std::vector<std::string>;

    public:
        //Functions

        CONSOLE(std::istream&, std::ostream&);
        ~CONSOLE();

        template<typename ...ARGS>
        bool add_console_command(std::string command, std::string description, void(*func)(ARGS...)); //May need to allow member functions in future
        void remove_console_command(std::string command);

        void invoke_commands();
        void output(std::string const& msg);

    private:
        void console_loop();
        void check_input();

        std::thread             m_thread;
        std::recursive_mutex    m_mutex;                    //We use a recursive mutex, as invoking a console command may involve printing to console (and thus acquires the mutex twice)
        bool                    m_running = true;
        COMMAND_DEFINITION_MAP  m_command_definitions;
        COMMANDS                m_commands;
        std::istream&           m_istream;
        std::ostream&           m_ostream;
    };

    // CONSOLE //
    
    template<typename ...ARGS>
    inline bool CONSOLE::add_console_command(std::string command, std::string description, void(*func)(ARGS...))
    {
        m_mutex.lock();
        
        bool success;
        
        auto test_itr = m_command_definitions.find(command);
        if (test_itr != m_command_definitions.end())
            success = false;
        else
        {
            success = true;
            m_command_definitions.emplace(command, std::make_unique<COMMAND_DEFINITION_IMPL<ARGS...>>(description, func));
        }

        m_mutex.unlock();
        return success;
    }

    // COMMAND_DEFINITION //

    template<typename ...ARGS>
    inline CONSOLE::COMMAND_DEFINITION_IMPL<ARGS...>::COMMAND_DEFINITION_IMPL(std::string description, void(*func)(ARGS...))
    {
        description = description;
        fn = func;
    }

    template<typename ...ARGS>
    inline bool CONSOLE::COMMAND_DEFINITION_IMPL<ARGS...>::register_command(CONSOLE& console, [[maybe_unused]] std::istream& command_stream)
    {
        auto command = std::make_unique<COMMAND_IMPL<ARGS...>>(fn);
        if constexpr (sizeof...(ARGS) != 0)
        {
            if (!command->read_arguments_from_stream_recursive<0, sizeof...(ARGS)>(command_stream))
                return false;
        }

        console.m_commands.push_back(std::move(command));
        return true;
    }

    // COMMAND //

    template<typename ...ARGS>
    inline CONSOLE::COMMAND_IMPL<ARGS...>::COMMAND_IMPL(void(*func)(ARGS...))
        : fn(func)
    {}

    template<typename ...ARGS>
    inline void CONSOLE::COMMAND_IMPL<ARGS...>::invoke()
    {
        std::apply(fn, arguments);
    }

    template<typename ...ARGS>
    template<int INDEX, int COUNT>
    inline bool CONSOLE::COMMAND_IMPL<ARGS...>::read_arguments_from_stream_recursive(std::istream& command_stream)
    {
        command_stream >> std::get<INDEX>(arguments);

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