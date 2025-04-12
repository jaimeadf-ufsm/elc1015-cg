#pragma once

#include <chrono>

/**
 * @file Benchmark.h
 * @brief Provides a utility class for benchmarking code execution time.
 * 
 * This file defines the `yap::Benchmark` class, which allows measuring the 
 * execution time of code blocks. It supports starting, stopping, resetting, 
 * and retrieving timing statistics such as total time, average time, and 
 * the number of samples.
 */

namespace yap
{
    /**
     * @class Benchmark
     * @brief A utility class for measuring code execution time.
     * 
     * The `Benchmark` class provides methods to start and stop a timer, 
     * accumulate timing data, and retrieve statistics such as the total 
     * elapsed time, average time per sample, and the number of samples.
     */
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