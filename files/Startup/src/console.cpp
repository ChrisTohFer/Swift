#include "console.h"

#include <sstream>
#include <iomanip>

SWIFT::CONSOLE::CONSOLE(std::wistream& istream, std::wostream& ostream)
    : m_istream(istream), m_ostream(ostream)
{
    m_thread = std::thread(&CONSOLE::console_loop, this);
}

SWIFT::CONSOLE::~CONSOLE()
{
    m_running = false;
    //m_thread.join(); We don't join here as the thread may be waiting for cin, just allow the thread to terminate forcefully
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
    m_mutex.lock();
    m_ostream << msg;
    m_mutex.unlock();
}

void SWIFT::CONSOLE::console_loop()
{
    add_console_command(L"help", L"Bring up the full list of console commands.", *this, &CONSOLE::output_command_map);

    while (m_running)
    {
        check_input();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
    }
}

void SWIFT::CONSOLE::check_input()
{
    m_istream.peek();   //I don't know why, but nothing can be entered to the console if we dont do this

    std::wstring command_line;
    std::getline(m_istream, command_line);

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
    for (auto& command : m_command_definitions)
    {
        m_ostream << std::left << std::setw(28) << command.first << "- " << command.second->description << "\n";
    }
}
