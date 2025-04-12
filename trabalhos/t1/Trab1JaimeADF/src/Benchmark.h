#pragma once

#include <chrono>

namespace yap
{
    class Benchmark
    {
    private:
        bool m_Active = false;

        std::chrono::high_resolution_clock::time_point m_StartTimepoint;

        int32_t m_Samples = 0;
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

            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(endTimepoint - m_StartTimepoint);
            double seconds = duration.count() / 1e9;
            
            m_Samples++;
            m_TotalTime += seconds;
        }

        void Reset()
        {
            m_TotalTime = 0;
            m_Samples = 0;
        }

        int32_t GetSamples() const
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