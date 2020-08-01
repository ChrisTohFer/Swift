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


    //FUNCTIONS

    CONSOLE& console();

}

#include "console.inl"