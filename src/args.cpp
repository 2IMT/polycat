#include "args.h"

#include <filesystem>
#include <cstdlib>
#include <cstring>
#include <format>

static const char* _adv_args(int& argc, char**& argv)
{
    if (argc == 0)
    {
        return nullptr;
    }

    const char* arg = argv[0];
    argc--;
    argv++;
    return arg;
}

static bool _streq(const char* l, const char* r) { return strcmp(l, r) == 0; }

namespace pcat
{

    static std::string _get_conf_path() noexcept
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

        std::string conf_path = home_path + ".config/" + args::CONF_NAME;

        if (fs::exists(conf_path))
        {
            return conf_path;
        }
        else
        {
            return "/usr/share/polycat/" + args::CONF_NAME;
        }
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
        m_conf_path(_get_conf_path()),
        m_help(false),
        m_version(false)
    {
    }

    void args::parse()
    {
        _adv_args(m_argc, m_argv);

        const char* arg = nullptr;
        while ((arg = _adv_args(m_argc, m_argv)) != nullptr)
        {
            if (_streq("-h", arg) || _streq("--help", arg))
            {
                m_help = true;
            }
            else if (_streq("-v", arg) || _streq("--version", arg))
            {
                m_version = true;
            }
            else if (_streq("-s", arg) || _streq("--stat-path", arg))
            {
                const char* value = _adv_args(m_argc, m_argv);
                if (value == nullptr)
                {
                    throw parse_err(std::format(
                        "Parameter `{}` expected a value, but got none", arg));
                }
                m_stat_path = value;
            }
            else if (_streq("-c", arg) || _streq("--config-path", arg))
            {
                const char* value = _adv_args(m_argc, m_argv);
                if (value == nullptr)
                {
                    throw parse_err(std::format(
                        "Parameter `{}` expected a value, but got none", arg));
                }
                m_conf_path = value;
            }
            else
            {
                throw parse_err(std::format("Invalid parameter `{}`", arg));
            }
        }
    }

    std::string args::stat_path() const noexcept { return m_stat_path; }

    std::string args::conf_path() const noexcept { return m_conf_path; }

    bool args::help() const noexcept { return m_help; };

    bool args::version() const noexcept { return m_version; };

}
