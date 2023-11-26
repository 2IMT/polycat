#pragma once

#include <string>
#include <cstdint>
#include <exception>

namespace pcat
{

    /**
     * @brief Loads and parses the config file
     */
    class conf
    {
    public:
        static inline const std::string FRAMES_DEFAULT = "";

        static inline const uint8_t HIGH_RATE_DEFAULT = 2;

        static inline const uint8_t LOW_RATE_DEFAULT = 30;

        static inline const uint64_t POLL_PERIOD_DEFAULT = 1000;

        static inline const bool SMOOTHING_ENABLED_DEFAULT = true;

        static inline const uint64_t SMOOTHING_VALUE_DEFAULT = 2000;

        static inline const bool SLEEPING_ENABLED_DEFAULT = true;

        static inline const uint64_t SLEEPING_THRESHOLD_DEFAULT = 8;

        static inline const uint64_t WAKEUP_THRESHOLD_DEFAULT = 12;

        static inline const std::string SLEEPING_FRAMES_DEFAULT = "";

        static inline const uint64_t SLEEPING_RATE_DEFAULT = 4;

        static inline const bool FORMAT_ENABLED_DEFAULT = false;

        static inline const std::string FORMAT_DEFAULT = "$frame $lcpu";

        static inline const std::string FRAMES_KEY = "frames";

        static inline const std::string HIGH_RATE_KEY = "highRate";

        static inline const std::string LOW_RATE_KEY = "lowRate";

        static inline const std::string POLL_PERIOD_KEY = "pollingPeriod";

        static inline const std::string SMOOTHING_ENABLED_KEY = "smoothEnabled";

        static inline const std::string SMOOTHING_VALUE_KEY = "smoothValue";

        static inline const std::string SLEEPING_ENABLED_KEY = "sleepEnabled";

        static inline const std::string SLEEPING_THRESHOLD_KEY =
            "sleepThreshold";

        static inline const std::string WAKEUP_THRESHOLD_KEY =
            "wakeupThreshold";

        static inline const std::string SLEEPING_FRAMES_KEY = "sleepFrames";

        static inline const std::string SLEEPING_RATE_KEY = "sleepRate";

        static inline const std::string FORMAT_ENABLED_KEY = "formatEnabled";

        static inline const std::string FORMAT_KEY = "format";

        /**
         * @brief Thrown on IO errors while opening
         */
        class open_err : public std::exception
        {
        public:
            open_err(const std::string& message) noexcept;

            const char* what() const noexcept;

        private:
            std::string m_message;
        };

        /**
         * @brief Thrown on IO errors while reading
         */
        class read_err : public std::exception
        {
        public:
            read_err(const std::string& message) noexcept;

            const char* what() const noexcept;

        private:
            std::string m_message;
        };

        /**
         * @brief Thrown on JSON parsing errors
         */
        class parse_err : public std::exception
        {
        public:
            parse_err(const std::string& message) noexcept;

            const char* what() const noexcept;

        private:
            std::string m_message;
        };

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
         * @brief Constructs an instance with associated config path
         * @param config_path Path to config file
         */
        conf(const std::string& config_path) noexcept;

        /**
         * @brief Loads and parses the config file
         */
        void load();

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
