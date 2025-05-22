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
        static inline const std::string CONF_NAME = "polycat-config";

        static inline const std::string STAT_PATH_DEFAULT = "/proc/stat";

        static inline const std::string HELP_TEXT =
            R"(Usage: polycat [--help] [--version] --stat-path <path> --config-path <path>

Optional arguments:
    -h, --help                shows help message and exits
    -v, --version             prints version information and exits
    -s, --stat-path <path>    sets the path for stat file used to poll the CPU
        default: "/proc/stat"
    -c, --config-path <path>  sets the path for configuration file
        default: `$HOME/.config/polycat-config` if exists, `)" POLYCAT_PREFIX
            R"(/share/polycat/polycat-config` otherwise)";

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
         * @brief Tells if help was requested
         * @return true - if help was requested, false - otherwise
         */
        bool help() const noexcept;

        /**
         * @brief Tells if version was requested
         * @return true - if version was requested, false - otherwise
         */
        bool version() const noexcept;

    private:
        int m_argc;
        char** m_argv;
        std::string m_stat_path;
        std::string m_conf_path;
        bool m_help;
        bool m_version;
    };

}
