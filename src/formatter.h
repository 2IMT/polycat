#pragma once

#include <string>
#include <cstdint>
#include <exception>

namespace pcat
{

    /**
     * @brief Formats the output
     */
    class formatter
    {
    public:
        static const std::string FORMAT_PREFIX;

        static const std::string L_CPU_LOAD_KEY;

        static const std::string R_CPU_LOAD_KEY;

        static const std::string FRAME_KEY;

        static const std::string PREFIX_KEY;

        /**
         * @brief Thrown on format errors
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
         * @brief Constructs an empty instance
         */
        formatter() noexcept;

        /**
         * @brief Sets current format,
         * Example format: "$frame $lcpu",
         * Available keys: $frame, $lcpu, $rcpu, $$
         * @param format Format string
         * @exception pcat::formatter::fmt_err
         */
        void set(const std::string& format);

        /**
         * @brief Formats the string using current format
         * @param frame Animation frame
         * @param load CPU load
         * @return Formatted string
         */
        std::string format(
            const std::string& frame, uint8_t load) const noexcept;

    private:
        std::string m_format;
        std::string m_rcpu_fmt;
        std::string m_lcpu_fmt;
        std::string m_frame_fmt;
        std::string m_prefix_fmt;
    };

}
