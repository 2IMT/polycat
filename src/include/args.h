#pragma once

#include <string>
#include <exception>

namespace pcat
{

    /**
     * @brief Command line arguments parser
     */
    class args
    {
    public:
        static const std::string PROGRAM_NAME;

        static const std::string PROGRAM_VERSION;

        static const std::string STAT_PATH_ARG;

        static const std::string STAT_PATH_ARG_LONG;

        static const std::string STAT_PATH_ARG_HELP;

        static const std::string STAT_PATH_ARG_META;

        static const std::string CONF_PATH_ARG;

        static const std::string CONF_PATH_ARG_LONG;

        static const std::string CONF_PATH_ARG_HELP;

        static const std::string CONF_PATH_ARG_META;

        static const std::string STAT_PATH_DEFAULT;

        static const std::string CONF_PATH_DEFAULT;

        /**
         * Thrown on parsing errors
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
         * @brief Constructs an instance of parser
         * @param argc Argument count including exec name (argv[0])
         * @param argv Arguments including exec name (argv[0])
         */
        args(int argc, char** argv) noexcept;

        /**
         * @brief Parses given arguments and sets corresponding values
         * @exception pcat::args::parse_err
         */
        void parse();

        /**
         * @brief Tells stat file location
         * @return Stat file path
         */
        std::string stat_path() const noexcept;

        /**
         * @brief Tells config file location
         * @return Config file path
         */
        std::string conf_path() const noexcept;

    private:
        int m_argc;
        char** m_argv;
        std::string m_stat_path;
        std::string m_conf_path;
    };

}
