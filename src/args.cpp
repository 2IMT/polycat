#include "args.h"

#include <filesystem>
#include <format>
#include <stdexcept>
#include <cstdlib>
#include <unordered_set>

#include <argparse/argparse.hpp>

namespace pcat
{

    std::pair<std::string, std::string> parse_overwrite(
        const std::string& overwrite)
    {
        args::parse_err error(std::format(
            "Overwrite string \"{}\" has invalid format", overwrite));

        if (overwrite.empty())
        {
            throw error;
        }

        std::pair<std::string, std::string> result;

        size_t equal_count = 0;
        size_t key_end = 0;
        size_t value_start = 0;
        for (std::size_t i = 0; i < overwrite.length(); i++)
        {
            char ch = overwrite[i];

            if (i == 0 || i == overwrite.length() - 1)
            {
                if (ch == '=')
                {
                    throw error;
                }
            }

            if (ch == '=')
            {
                equal_count++;
                if (equal_count > 1)
                {
                    throw error;
                }

                key_end = i;
                value_start = i + 1;
            }
        }

        if (equal_count == 0)
        {
            throw error;
        }

        result.first =
            std::string(overwrite.begin(), overwrite.begin() + key_end);
        result.second =
            std::string(overwrite.begin() + value_start, overwrite.end());

        return result;
    }

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

        program.add_argument(OVERWRITE_ARG, OVERWRITE_ARG_LONG)
            .required()
            .metavar(OVERWRITE_ARG_META)
            .help(OVERWRITE_ARG_HELP)
            .default_value<std::vector<std::string>>({})
            .append();

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

        std::vector<std::string> overwrites =
            program.get<std::vector<std::string>>(OVERWRITE_ARG_LONG);

        m_overwrites.reserve(overwrites.size());

        std::unordered_set<std::string> key_set;
        key_set.reserve(overwrites.size());
        for (const auto& overwrite : overwrites)
        {
            std::pair<std::string, std::string> overwrite_pair =
                parse_overwrite(overwrite);

            key_set.insert(overwrite_pair.first);
            m_overwrites.push_back(overwrite_pair);
        }

        if (key_set.size() < overwrites.size())
        {
            throw parse_err("Overwrite list has duplicate keys");
        }
    }

    std::string args::stat_path() const noexcept { return m_stat_path; }

    std::string args::conf_path() const noexcept { return m_conf_path; }

    std::vector<std::pair<std::string, std::string>>
    args::overwrites() const noexcept
    {
        return m_overwrites;
    }

}
