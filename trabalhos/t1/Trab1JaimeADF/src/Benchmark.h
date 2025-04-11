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
        bool m_Active = false; ///< Indicates whether the benchmark is currently active.
        std::chrono::high_resolution_clock::time_point m_StartTimepoint; ///< The start time of the current benchmark session.
        int32_t m_Samples = 0; ///< The number of recorded samples.
        double m_TotalTime = 0; ///< The total accumulated time in seconds.

    public:
        /**
         * @brief Starts the benchmark timer.
         * 
         * Marks the benchmark as active and records the current time as the start time.
         */
        void Start()
        {
            m_Active = true;
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        /**
         * @brief Stops the benchmark timer and records the elapsed time.
         * 
         * If the benchmark is active, calculates the elapsed time since the 
         * last call to `Start()`, adds it to the total time, and increments 
         * the sample count.
         */
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

        /**
         * @brief Resets the benchmark statistics.
         * 
         * Clears the total time and sample count, effectively resetting the benchmark.
         */
        void Reset()
        {
            m_TotalTime = 0;
            m_Samples = 0;
        }

        /**
         * @brief Retrieves the number of recorded samples.
         * 
         * @return The number of samples recorded by the benchmark.
         */
        int32_t GetSamples() const
        {
            return m_Samples;
        }

        /**
         * @brief Retrieves the average time per sample.
         * 
         * @return The average time per sample in seconds. Returns 0.0 if no samples have been recorded.
         */
        double GetAverageTime() const
        {
            return m_Samples > 0 ? m_TotalTime / m_Samples : 0.0;
        }

        /**
         * @brief Retrieves the total accumulated time.
         * 
         * @return The total time recorded by the benchmark in seconds.
         */
        double GetTotalTime() const
        {
            return m_TotalTime;
        }
    };
}