#pragma once

#include <chrono>

namespace SWIFT
{
    class CYCLE_TIMER
    {
        using TIME_POINT = std::chrono::system_clock::time_point;

    public:
        CYCLE_TIMER(int samples)
            : m_durations_size(samples)
            , m_index(0)
        {
            m_durations = new int[samples]();
            m_previous_time = std::chrono::system_clock::now();
        }
        ~CYCLE_TIMER()
        {
            delete[] m_durations;
        }

        void cycle()
        {
            auto time = std::chrono::system_clock::now();
            m_index = (m_index + 1) % m_durations_size;
            m_durations[m_index] = int(std::chrono::duration_cast<std::chrono::microseconds>(time - m_previous_time).count());
            m_previous_time = time;
        }

        void skip()
        {
            m_previous_time = std::chrono::system_clock::now();
        }

        int average_cycle_time()
        {
            int total = 0;
            for (auto i = 0; i < m_durations_size; ++i)
                total += m_durations[i];

            return total / m_durations_size;
        }

    private:
        int* m_durations;
        int  m_durations_size;
        int  m_index;

        TIME_POINT m_previous_time;
    };
}