#include "args.h"

#include <filesystem>
#include <cstdlib>
#include <sstream>

#include <argparse/argparse.hpp>

namespace pcat
{

    const std::string args::PROGRAM_NAME = "polycat";

    const std::string args::PROGRAM_VERSION = "1.0.0";

    const std::string args::STAT_PATH_DEFAULT = "/proc/stat";

    const std::string args::STAT_PATH_ARG = "-s";

    const std::string args::STAT_PATH_ARG_LONG = "--stat-path";

    const std::string args::STAT_PATH_ARG_HELP =
        "sets the path for stat file used to poll the CPU";

    const std::string args::STAT_PATH_ARG_META = "<path>";

    const std::string args::CONF_NAME = "polycat-config.json";

    const std::string args::CONF_PATH_ARG = "-c";

    const std::string args::CONF_PATH_ARG_LONG = "--config-path";

    const std::string args::CONF_PATH_ARG_HELP =
        "sets the path for configuration file";

    const std::string args::CONF_PATH_ARG_META = "<path>";

    std::string create_conf_path() noexcept
    {
        namespace fs = std::filesystem;

        char* home_path_c_str = std::getenv("HOME");

        if (home_path_c_str == NULL)
        {
            return args::CONF_NAME;
        }

        std::string home_path = home_path_c_str;

        if (home_path.empty())
        {
            return args::CONF_NAME;
        }
        if (home_path.back() != '/')
        {
            home_path += "/";
        }

        std::string conf_dir = home_path + ".config";

        if (!fs::exists(conf_dir))
        {
            try
            {
                fs::create_directory(conf_dir);
            }
            catch (fs::filesystem_error& e)
            {
                return args::CONF_NAME;
            }
        }
        else if (!fs::is_directory(conf_dir))
        {
            return args::CONF_NAME;
        }

        return conf_dir + "/" + args::CONF_NAME;
    }

    args::parse_err::parse_err(const std::string& message) noexcept :
        m_message(message)
    {
    }

    const char* args::parse_err::what() const noexcept
    {
        return m_message.c_str();
    }

    args::args(int argc, char** argv) noexcept :
        m_argc(argc),
        m_argv(argv),
        m_stat_path(STAT_PATH_DEFAULT),
        m_conf_path(create_conf_path())
    {
    }

    void args::parse()
    {
        argparse::ArgumentParser program(PROGRAM_NAME, PROGRAM_VERSION);

        program.add_argument(STAT_PATH_ARG, STAT_PATH_ARG_LONG)
            .required()
            .metavar(STAT_PATH_ARG_META)
            .help(STAT_PATH_ARG_HELP)
            .default_value(STAT_PATH_DEFAULT);

        program.add_argument(CONF_PATH_ARG, CONF_PATH_ARG_LONG)
            .required()
            .metavar(CONF_PATH_ARG_META)
            .help(CONF_PATH_ARG_HELP)
            .default_value(CONF_NAME);

        try
        {
            program.parse_args(m_argc, m_argv);
        }
        catch (std::runtime_error& e)
        {
            throw parse_err(e.what());
        }

        m_stat_path = program.get(STAT_PATH_ARG_LONG);

        if (program.is_used(CONF_PATH_ARG_LONG))
        {
            m_conf_path = program.get(CONF_PATH_ARG_LONG);
        }
    }

    std::string args::stat_path() const noexcept { return m_stat_path; }

    std::string args::conf_path() const noexcept { return m_conf_path; }

}
