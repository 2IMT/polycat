#include "formatter.h"

#include <sstream>
#include <regex>

size_t count_occurences(
    const std::string& string, const std::string& substring) noexcept
{
    size_t occurences = 0;
    std::string::size_type pos = 0;
    while ((pos = string.find(substring, pos)) != std::string::npos)
    {
        occurences += 1;
        pos += substring.length();
    }

    return occurences;
}

void replace_all(std::string& string, const std::string& substring_old,
    const std::string& substring_new) noexcept
{
    if (substring_old.empty())
    {
        return;
    }

    std::string::size_type pos = 0;
    while ((pos = string.find(substring_old, pos)) != std::string::npos)
    {
        string.replace(pos, substring_old.length(), substring_new);
        pos += substring_new.length();
    }
}

namespace pcat
{

const std::string formatter::FORMAT_PREFIX = "$";

const std::string formatter::L_CPU_LOAD_KEY = "lcpu";

const std::string formatter::R_CPU_LOAD_KEY = "rcpu";

const std::string formatter::FRAME_KEY = "frame";

const std::string formatter::PREFIX_KEY = "$";

formatter::fmt_err::fmt_err(const std::string& message) noexcept :
    m_message(message)
{
}

const char* formatter::fmt_err::what() const noexcept
{
    return m_message.c_str();
}

formatter::formatter() noexcept :
    m_format(),
    m_rcpu_fmt(),
    m_lcpu_fmt(),
    m_frame_fmt(),
    m_prefix_fmt()
{
}

void formatter::set(const std::string& format)
{
    std::string rcpu_fmt = FORMAT_PREFIX + R_CPU_LOAD_KEY;
    std::string lcpu_fmt = FORMAT_PREFIX + L_CPU_LOAD_KEY;
    std::string frame_fmt = FORMAT_PREFIX + FRAME_KEY;
    std::string prefix_fmt = FORMAT_PREFIX + PREFIX_KEY;

    size_t prefix_occurences = count_occurences(format, FORMAT_PREFIX);
    size_t rcpu_occurences = count_occurences(format, rcpu_fmt);
    size_t lcpu_occurences = count_occurences(format, lcpu_fmt);
    size_t frame_occurences = count_occurences(format, frame_fmt);
    size_t esc_prefix_occurences = count_occurences(format, prefix_fmt);

    prefix_occurences -= esc_prefix_occurences;

    if (prefix_occurences != rcpu_occurences + lcpu_occurences +
                                 frame_occurences + esc_prefix_occurences)
    {
        std::stringstream message;
        message << "String \"" << format << "\" has incorrect format.";
        throw fmt_err(message.str());
    }

    m_format = format;
    m_rcpu_fmt = rcpu_fmt;
    m_lcpu_fmt = lcpu_fmt;
    m_frame_fmt = frame_fmt;
    m_prefix_fmt = prefix_fmt;
}

std::string formatter::format(
    const std::string& frame, uint8_t load) const noexcept
{
    std::string result = m_format;

    std::string r_load_string =
        std::to_string(static_cast<uint32_t>(load)) + std::string("%");
    std::string l_load_string = r_load_string;

    while (l_load_string.length() < 4 && r_load_string.length() < 4)
    {
        r_load_string = " " + r_load_string;
        l_load_string = l_load_string + " ";
    }

    replace_all(result, m_prefix_fmt, FORMAT_PREFIX);
    replace_all(result, m_frame_fmt, frame);
    replace_all(result, m_rcpu_fmt, r_load_string);
    replace_all(result, m_lcpu_fmt, l_load_string);

    return result;
}

}
