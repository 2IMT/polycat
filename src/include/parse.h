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
         * @brief Thrown on parsing errors
         */
        class err : public std::exception
        {
        public:
            /**
             * @brief Construct an instance without location
             * @param message Message
             */
            err(const std::string& message) noexcept;

            /**
             * @brief Construct an instance with specified location
             * @param l Location
             * @param message Message
             */
            err(loc l, const std::string& message) noexcept;

            /**
             * @brief Indicates whether whether or not the error has location
             * @return true - if has, false - otherwise
             */
            bool has_loc() const noexcept;

            /**
             * @brief Gets the error location
             * @return Location
             */
            loc get_loc() const noexcept;

            const char* what() const noexcept override;

        private:
            bool m_has_loc;
            loc m_loc;
            std::string m_message;
        };

        /**
         * @brief Thrown when no key found
         */
        class no_key_err : public std::exception
        {
        public:
            no_key_err(const std::string& key) noexcept;

            const char* what() const noexcept override;

        private:
            std::string m_message;
        };

        /**
         * @brief Thrown on type errors
         */
        class type_err : public std::exception
        {
        public:
            type_err(const std::string& key, const std::string& expected,
                const std::string& actual) noexcept;

            const char* what() const noexcept override;

        private:
            std::string m_message;
        };

        using data_value = std::variant<std::string, int64_t, bool>;

        parse() noexcept;

        /**
         * @brief Loads the config from file
         * @param path Path to file
         * @return List of parsing errors occured
         */
        std::vector<err> load(const std::string& path) noexcept;

        /**
         * @brief Gets the string value
         * @param key Key
         * @return String value
         * @exception pcat::parse::no_key_err
         * @exception pcat::parse::type_err
         */
        std::string get_string(const std::string& key) const;

        /**
         * @brief Gets the integer value
         * @param key Key
         * @return Integer value
         * @exception pcat::parse::no_key_err
         * @exception pcat::parse::type_err
         */
        int64_t get_int(const std::string& key) const;

        /**
         * @brief Gets the boolean value
         * @param key Key
         * @return Boolean value
         * @exception pcat::parse::no_key_err
         * @exception pcat::parse::type_err
         */
        bool get_bool(const std::string& key) const;

        /**
         * @brief Gets the value
         * @param key Key
         * @return Value
         * @exception pcat::parse::no_key_err
         */
        data_value get_value(const std::string& key) const;

        /**
         * @brief Indicated whether the config has specified key
         * @param key Key
         * @return true - if has, false - otherwise
         */
        bool has_key(const std::string& key) const;

        /**
         * @brief Get all key-value pairs
         * @return Unordered map of key-value pairs
         */
        std::unordered_map<std::string, data_value> values() const;

    private:
        std::unordered_map<std::string, data_value> m_values;
    };

}
