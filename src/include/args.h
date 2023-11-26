#pragma once

#include <utility>
#include <string>
#include <vector>
#include <exception>

namespace pcat
{

    /**
     * @brief Command line arguments parser
     */
    class args
    {
    public:
        static inline const std::string PROGRAM_NAME = "polycat";

        static inline const std::string PROGRAM_VERSION = "1.3.0";

        static inline const std::string STAT_PATH_DEFAULT = "/proc/stat";

        static inline const std::string STAT_PATH_ARG = "-s";

        static inline const std::string STAT_PATH_ARG_LONG = "--stat-path";

        static inline const std::string STAT_PATH_ARG_HELP =
            "sets the path for stat file used to poll the CPU";

        static inline const std::string STAT_PATH_ARG_META = "<path>";

        static inline const std::string CONF_NAME = "polycat-config.json";

        static inline const std::string CONF_PATH_ARG = "-c";

        static inline const std::string CONF_PATH_ARG_LONG = "--config-path";

        static inline const std::string CONF_PATH_ARG_HELP =
            "sets the path for configuration file";

        static inline const std::string CONF_PATH_ARG_META = "<path>";

        static inline const std::string OVERWRITE_ARG = "-o";

        static inline const std::string OVERWRITE_ARG_LONG = "--overwrite";

        static inline const std::string OVERWRITE_ARG_HELP =
            "overwrites the config value (can be specified several "
            "times)";

        static inline const std::string OVERWRITE_ARG_META = "<key>=<value>";

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

        /**
         * @brief Tells config overwrites
         * @return Config overwrites in form of key-value pair list
         */
        std::vector<std::pair<std::string, std::string>>
        overwrites() const noexcept;

    private:
        int m_argc;
        char** m_argv;
        std::string m_stat_path;
        std::string m_conf_path;
        std::vector<std::pair<std::string, std::string>> m_overwrites;
    };

}
