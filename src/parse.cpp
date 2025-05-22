#include "parse.h"

#include <format>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <charconv>
#include <system_error>
#include <variant>

static size_t _ltrim(std::string& s)
{
    size_t pos = 0;
    for (size_t i = 0; i < s.length(); i++)
    {
        if (!std::isblank(s[i]))
        {
            pos = i;
            break;
        }
    }
    if (pos != 0)
    {
        s.erase(0, pos);
    }
    return pos;
}

static void _rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
        s.end());
}

static size_t _trim(std::string& s)
{
    _rtrim(s);
    return _ltrim(s);
}

static pcat::parse::data_value _parse_value(
    const std::string& s, pcat::parse::loc start_loc)
{
    if (s.empty())
    {
        throw pcat::parse::err(start_loc, "Empty value");
    }

    if (s[0] == '"')
    {
        if (s.length() < 2)
        {
            throw pcat::parse::err(start_loc, "Unterminated string");
        }
        if (s[s.length() - 1] == '"')
        {
            return std::string(s.begin() + 1, s.end() - 1);
        }
        else
        {
            throw pcat::parse::err(
                start_loc, "Last string character is not `\"`");
        }
    }
    else if (s == "true")
    {
        return true;
    }
    else if (s == "false")
    {
        return false;
    }
    else
    {
        for (size_t i = 0; i < s.length(); i++)
        {
            if (!std::isdigit(s[i]))
            {
                pcat::parse::loc l(start_loc.l, start_loc.c + i);
                throw pcat::parse::err(l, "Unexpected character");
            }
        }

        int64_t result = 0;

        auto [ptr, ec] =
            std::from_chars(s.data(), s.data() + s.length(), result);

        if (ec == std::errc::invalid_argument)
        {
            throw pcat::parse::err(start_loc, "Not an integer");
        }
        else if (ec == std::errc::result_out_of_range)
        {
            throw pcat::parse::err(start_loc, "Integer out of range");
        }
        else if (ec != std::errc())
        {
            throw pcat::parse::err(start_loc, "Error while parsing integer");
        }

        return result;
    }
}

namespace pcat
{

    parse::err::err(const std::string& message) noexcept :
        m_has_loc(false),
        m_loc(),
        m_message(message)
    {
    }

    parse::err::err(loc l, const std::string& message) noexcept :
        m_has_loc(true),
        m_loc(l),
        m_message(message)
    {
    }

    bool parse::err::has_loc() const noexcept { return m_has_loc; }

    parse::loc parse::err::get_loc() const noexcept { return m_loc; }

    const char* parse::err::what() const noexcept { return m_message.c_str(); }

    parse::no_key_err::no_key_err(const std::string& key) noexcept :
        m_message(std::format("No key `{}` found", key))
    {
    }

    const char* parse::no_key_err::what() const noexcept
    {
        return m_message.c_str();
    }

    parse::type_err::type_err(const std::string& key,
        const std::string& expected, const std::string& actual) noexcept :
        m_message(std::format(
            "Key `{}` is expected to be of type {}, but the actual type is {}",
            key, expected, actual))
    {
    }

    const char* parse::type_err::what() const noexcept
    {
        return m_message.c_str();
    }

    parse::parse() noexcept :
        m_values()
    {
    }

    std::vector<parse::err> parse::load(const std::string& path) noexcept
    {
        m_values.clear();

        std::vector<err> errors;
        std::ifstream file;

        file.open(path);
        if (file.fail())
        {
            errors.push_back(err(std::format("Failed to open `{}`", path)));
            return errors;
        }

        std::string line;
        loc l(0, 1);
        while (std::getline(file, line))
        {
            l.l++;
            if (file.fail() || file.bad())
            {
                errors.push_back(err(std::format("Failed to read `{}`", path)));
                return errors;
            }

            l.c = 1;
            size_t pos = _trim(line);
            l.c += pos;
            if (!line.empty())
            {
                size_t pos = line.find("=");
                if (pos == std::string::npos)
                {
                    errors.push_back(err(l, "Line does not have `=`"));
                    continue;
                }
                std::string key = line.substr(0, pos);
                _trim(key);
                if (key.empty())
                {
                    errors.push_back(err(l, "Key is empty"));
                    continue;
                }
                if (m_values.contains(key))
                {
                    errors.push_back(
                        err(l, std::format("Duplicate key `{}`", key)));
                }
                std::string value = line.substr(pos + 1, line.length() + 1);
                size_t val_trim_pos = _trim(value);
                l.c += pos + val_trim_pos + 1;
                try
                {
                    data_value val = _parse_value(value, l);
                    m_values[key] = val;
                }
                catch (const err& e)
                {
                    errors.push_back(e);
                }
            }
        }

        return errors;
    }

    std::string parse::get_string(const std::string& key) const
    {
        data_value value = get_value(key);
        if (std::holds_alternative<std::string>(value))
        {
            return std::get<std::string>(value);
        }
        else if (std::holds_alternative<int64_t>(value))
        {
            throw type_err(key, "string", "integer");
        }
        else if (std::holds_alternative<bool>(value))
        {
            throw type_err(key, "string", "boolean");
        }
        else
        {
            return "";
        }
    }

    int64_t parse::get_int(const std::string& key) const
    {
        data_value value = get_value(key);
        if (std::holds_alternative<std::string>(value))
        {
            throw type_err(key, "integer", "string");
        }
        else if (std::holds_alternative<int64_t>(value))
        {
            return std::get<int64_t>(value);
        }
        else if (std::holds_alternative<bool>(value))
        {
            throw type_err(key, "integer", "boolean");
        }
        else
        {
            return 0;
        }
    }

    bool parse::get_bool(const std::string& key) const
    {
        data_value value = get_value(key);
        if (std::holds_alternative<std::string>(value))
        {
            throw type_err(key, "boolean", "string");
        }
        else if (std::holds_alternative<int64_t>(value))
        {
            throw type_err(key, "boolean", "integer");
        }
        else if (std::holds_alternative<bool>(value))
        {
            return std::get<bool>(value);
        }
        else
        {
            return false;
        }
    }

    parse::data_value parse::get_value(const std::string& key) const
    {
        if (!has_key(key))
        {
            throw no_key_err(key);
        }
        return m_values.at(key);
    }

    bool parse::has_key(const std::string& key) const
    {
        return m_values.contains(key);
    }

    std::unordered_map<std::string, parse::data_value> parse::values() const
    {
        return m_values;
    }
}
