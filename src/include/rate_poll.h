#pragma once

#include <atomic>
#include <chrono>
#include <string>
#include <chrono>

#include "cpu.h"

namespace pcat
{

    /**
     * @brief Poll CPU load at a constant rate
    */
    class rate_poll
    {
    public:
        /**
         * @param period Period of polling
        */
        rate_poll(uint64_t period,
            const std::string& stat_path) noexcept;

        /**
         * @brief CPU polling routine (should be run in separate thread)
        */
        void run() noexcept;

        /**
         * @brief Stops polling the CPU
        */
        void stop() noexcept;

        /**
         * @brief Tells if an IO error has happened during CPU polling
         * @return true - on error, false - otherwise
        */
        bool io_err() noexcept;

        /**
         * @brief Tells if stat file was malformed during CPU polling
         * @return true - on error, false - otherwise
        */
        bool fmt_err() noexcept;

        /**
         * @brief Tells the current CPU load
         * @return Value in range [0-1]
        */
        float poll() noexcept;

    private:
        cpu m_cpu;
        std::chrono::milliseconds m_period;
        bool m_done;
        bool m_io_err;
        bool m_fmt_err;
        float m_cpu_load;
        std::mutex m_done_mut;
        std::mutex m_io_err_mut;
        std::mutex m_fmt_err_mut;
        std::mutex m_cpu_load_mut;
    };

}
