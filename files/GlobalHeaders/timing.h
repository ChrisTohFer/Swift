#pragma once

#include <chrono>

namespace SWIFT
{
    class CYCLE_TIMER
    {
        using TIME_POINT = std::chrono::system_clock::time_point;
        using DURATION = std::chrono::system_clock::duration;

    public:
        CYCLE_TIMER(int samples)
            : m_durations_size(samples)
            , m_index(0)
        {
            m_durations = new DURATION[samples]();
            m_previous_time = std::chrono::system_clock::now();
        }
        ~CYCLE_TIMER()
        {
            delete[] m_durations;
        }

        int samples()
        {
            return m_durations_size;
        }

        void cycle()
        {
            auto time = std::chrono::system_clock::now();
            m_index = (m_index + 1) % m_durations_size;
            m_durations[m_index] = time - m_previous_time;
            m_previous_time = time;
        }

        void skip()
        {
            m_previous_time = std::chrono::system_clock::now();
        }

        template<typename TIME_UNIT = std::chrono::microseconds>
        long long average_cycle_time(int samples = 0)
        {
            if (samples <= 0 || samples > m_durations_size)
                samples = m_durations_size;

            long long total = 0;
            for (auto i = 0; i < m_durations_size; ++i)
            {
                //We're stepping back one cycle with each iteration and loop round if we reach the start of the array
                auto index = (m_index - i) + (i > m_index) * m_durations_size;
                total += std::chrono::duration_cast<TIME_UNIT>(m_durations[index]).count();
            }

            return total / m_durations_size;
        }
        template<typename TIME_UNIT = std::chrono::microseconds>
        long long previous_cycle_time()
        {
            return std::chrono::duration_cast<TIME_UNIT>(m_durations[m_index]).count();
        }

    private:
        DURATION* m_durations;
        int       m_durations_size;
        int       m_index;

        TIME_POINT m_previous_time;
    };
}