#pragma once

#include <string>
#include <unordered_map>
#include <cstdint>
#include <variant>
#include <exception>
#include <vector>

namespace pcat
{

    /**
     * @brief Parses the configuration file
     */
    class parse
    {
    public:
        /**
         * @brief Represents file location
         */
        struct loc
        {
            /**
             * @brief Construct a default initialized instance with both line
             * and column equal to 1
             */
            loc() :
                l(1),
                c(1)
            {
            }

            /**
             * @brief Construct an instance with specified line and column
             * @param l Line
             * @param c Column
            */
            loc(size_t l, size_t c) :
                l(l),
                c(c)
            {
            }

            size_t l;
            size_t c;
        };

        /**
         * @brief Represents parsing error
        */
        class err : public std::exception
        {
        public:
            /**
            */
            err(const std::string& message) noexcept;

            err(loc l, const std::string& message) noexcept;

            bool has_loc() const noexcept;

            loc get_loc() const noexcept;

            const char* what() const noexcept override;

        private:
            bool m_has_loc;
            loc m_loc;
            std::string m_message;
        };

        using data_value = std::variant<std::string, int64_t, bool>;

        parse() noexcept;

        std::vector<err> load(const std::string& path) noexcept;

        std::string get_string(const std::string& key) const;

        int64_t get_int(const std::string& key) const;

        bool get_bool(const std::string& key) const;

        std::unordered_map<std::string, data_value> values() const;

    private:
        std::unordered_map<std::string, data_value> m_values;
    };

}
