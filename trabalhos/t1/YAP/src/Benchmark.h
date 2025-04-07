#pragma once

#include <chrono>

namespace yap
{
    class Benchmark
    {
    private:
        bool m_Active = false;

        std::chrono::high_resolution_clock::time_point m_StartTimepoint;

        int64_t m_Samples = 0;
        double m_TotalTime = 0;
    
    public:
        void Start()
        {
            m_Active = true;
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        void Stop()
        {
            if (!m_Active)
            {
                return;
            }

            m_Active = false;
            auto endTimepoint = std::chrono::high_resolution_clock::now();

            m_Samples++;
            m_TotalTime += (endTimepoint - m_StartTimepoint).count() / 1e9;
        }

        void Reset()
        {
            m_TotalTime = 0;
            m_Samples = 0;
        }

        int64_t GetSamples() const
        {
            return m_Samples;
        }

        double GetAverageTime() const
        {
            return m_Samples > 0 ? m_TotalTime / m_Samples : 0.0;
        }

        double GetTotalTime() const
        {
            return m_TotalTime;
        }
    };
}