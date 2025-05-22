#pragma once

#include <string>
#include <cstdint>
#include <exception>
#include <vector>

#include "parse.h"

namespace pcat
{

    /**
     * @brief Loads and parses the config file
     */
    class conf
    {
    public:
        /**
         * @brief Thrown on type errors
         */
        class fmt_err : public std::exception
        {
        public:
            fmt_err(const std::string& message) noexcept;

            const char* what() const noexcept;

        private:
            std::string m_message;
        };

        /**
         * @brief Error collection
         */
        class load_errs
        {
        public:
            /**
             * @brief Indicates whether config has at least one error
             * @return true - if has, false - otherwise
             */
            bool any() const;

            std::vector<parse::err> parse_errs;
            std::vector<parse::type_err> type_errs;
            std::vector<parse::no_key_err> no_key_errs;
            std::vector<fmt_err> fmt_errs;
        };

        /**
         * @brief Constructs an instance with associated config path
         * @param config_path Path to config file
         */
        conf(const std::string& config_path) noexcept;

        /**
         * @brief Loads and parses the config file
         * @return Loading errors
         */
        load_errs load();

        /**
         * @brief Returns the FRAMES_KEY value from config
         */
        std::string frames() const noexcept;

        /**
         * @brief Returns the HIGH_RATE_KEY value from config
         */
        uint8_t high_rate() const noexcept;

        /**
         * @brief Returns the LOW_RATE_KEY value from config
         */
        uint8_t low_rate() const noexcept;

        /**
         * @brief Returns the POLL_PERIOD_KEY value from config
         */
        uint64_t poll_period() const noexcept;

        /**
         * @brief Returns the SMOOTHING_ENABLED_KEY value from config
         */
        bool smoothing_enabled() const noexcept;

        /**
         * @brief Returns the SMOOTHING_VALUE_KEY value from config
         */
        uint64_t smoothing_value() const noexcept;

        /**
         * @brief Returns the SLEEPING_ENABLED_KEY value from config
         */
        bool sleeping_enabled() const noexcept;

        /**
         * @brief Returns the SLEEPING_THRESHOLD_KEY value from config
         */
        uint8_t sleeping_threshold() const noexcept;

        /**
         * @brief Returns the WAKEUP_THRESHOLD_KEY value from config
         */
        uint8_t wakeup_threshold() const noexcept;

        /**
         * @brief Returns the SLEEPING_FRAMES_KEY value from config
         */
        std::string sleeping_frames() const noexcept;

        /**
         * @brief Returns the SLEEPING_RATE_KEY value from config
         */
        uint8_t sleeping_rate() const noexcept;

        /**
         * @brief Returns the FORMAT_ENABLED_KEY value from config
         */
        bool format_enabled() const noexcept;

        /**
         * @brief Returns the FORMAT_KEY value from config
         */
        std::string format() const noexcept;

    private:
        std::string m_path;

        std::string m_frames;
        uint8_t m_low_rate;
        uint8_t m_high_rate;
        uint64_t m_poll_period;
        bool m_smoothing_enabled;
        uint64_t m_smoothing_value;
        bool m_sleeping_enabled;
        uint8_t m_sleeping_threshold;
        uint8_t m_wakeup_threshold;
        std::string m_sleeping_frames;
        uint8_t m_sleeping_rate;
        bool m_format_enabled;
        std::string m_format;
    };

}
