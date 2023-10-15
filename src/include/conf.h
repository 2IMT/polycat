#pragma once

#include <string>
#include <cstdint>
#include <exception>

namespace pcat
{

    class conf
    {
    public:
        static const std::string FRAMES_DEFAULT;

        static const uint8_t HIGH_RATE_DEFAULT;

        static const uint8_t LOW_RATE_DEFAULT;

        static const uint64_t POLL_PERIOD_DEFAULT;

        static const std::string FRAMES_KEY;

        static const std::string HIGH_RATE_KEY;

        static const std::string LOW_RATE_KEY;

        static const std::string POLL_PERIOD_KEY;

        class open_err : public std::exception
        {
        public:
            open_err(const std::string& message) noexcept;

            const char* what() const noexcept;

        private:
            std::string m_message;
        };

        class read_err : public std::exception
        {
        public:
            read_err(const std::string& message) noexcept;

            const char* what() const noexcept;

        private:
            std::string m_message;
        };

        class parse_err : public std::exception
        {
        public:
            parse_err(const std::string& message) noexcept;

            const char* what() const noexcept;

        private:
            std::string m_message;
        };

        class fmt_err : public std::exception
        {
        public:
            fmt_err(const std::string& message) noexcept;

            const char* what() const noexcept;

        private:
            std::string m_message;
        };

        conf(const std::string& config_path) noexcept;

        void load();

        std::string frames() const noexcept;

        uint8_t high_rate() const noexcept;

        uint8_t low_rate() const noexcept;

        uint64_t poll_period() const noexcept;

    private:
        std::string m_path;

        std::string m_frames;
        uint8_t m_low_rate;
        uint8_t m_high_rate;
        uint64_t m_poll_period;
    };

}
