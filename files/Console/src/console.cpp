#include "console.h"

#include <sstream>
#include <iomanip>
#include <iostream>

SWIFT::CONSOLE::CONSOLE(std::wistream& istream, std::wostream& ostream)
    : m_istream(istream), m_ostream(ostream)
{
    m_thread = std::thread(&CONSOLE::console_loop, this);
}

SWIFT::CONSOLE::~CONSOLE()
{
    m_running = false;
    //m_thread.join(); We don't join here as the thread may be waiting for cin, just allow the thread to terminate forcefully on destruction
}

void SWIFT::CONSOLE::remove_console_command(std::wstring command)
{
    m_mutex.lock();

    auto command_itr = m_command_definitions.find(command);
    if (command_itr != m_command_definitions.end())
        m_command_definitions.erase(command_itr);

    m_mutex.unlock();
}

void SWIFT::CONSOLE::invoke_commands()
{
    m_mutex.lock();

    for (auto& command : m_commands)
    {
        command->invoke();
    }
    m_commands.clear();

    m_mutex.unlock();
}

void SWIFT::CONSOLE::output(std::wstring const& msg)
{
    static std::mutex output_mutex; //There is no need to use the same mutex for the output/input sides of the console
    output_mutex.lock();
    m_ostream << msg;
    output_mutex.unlock();
}

void SWIFT::CONSOLE::console_loop()
{
    add_console_command(L"help", L"Bring up the full list of console commands.", *this, &CONSOLE::output_command_map);

    while (m_running)
    {
        check_input();
    }
}

void SWIFT::CONSOLE::check_input()
{
    std::wstring command_line;
    std::getline(m_istream, command_line);  //We block here while waiting for input - make sure we haven't locked the mutex at this point

    if (command_line == L"")
    {
        return;
    }

    std::wistringstream command_stream(command_line);
    std::wstring command;
    command_stream >> command;

    //Parse command and add to vector
    m_mutex.lock();

    auto command_itr = m_command_definitions.find(command);
    if (command_itr == m_command_definitions.end())
    {
        m_ostream << "Command was not recognised.\n";
    }
    else if (!command_itr->second->register_command(*this, command_stream))
    {
        m_ostream << "Incorrect arguments supplied.\n";
    }

    m_mutex.unlock();
}

void SWIFT::CONSOLE::output_command_map()
{
    m_ostream << "\n";
    for (auto& command : m_command_definitions)
    {
        m_ostream << std::left << std::setw(20) << command.first << " - " << command.second->description << "\n";
    }
}

SWIFT::CONSOLE& SWIFT::console()
{
    static CONSOLE c(std::wcin, std::wcout);
    return c;
}
