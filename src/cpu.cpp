#include "cpu.h"

#include <sstream>
#include <numeric>

void throw_fmt_err(const std::string& stat_path)
{
    std::stringstream message;
    message << "Stat file \"" << stat_path
        << "\" has incorrect format.";
    throw pcat::cpu::fmt_err(message.str());
}

void throw_io_err(const std::string& stat_path)
{
    std::stringstream message;
    message << "An error occurred while reading file \"" << stat_path
        << "\".";
    throw pcat::cpu::io_err(message.str());
}

namespace pcat
{

    cpu::io_err::io_err(const std::string& message) noexcept :
        m_message(message)
    {
    }

    const char* cpu::io_err::what() const noexcept
    {
        return m_message.c_str();
    }

    cpu::fmt_err::fmt_err(const std::string& message) noexcept :
        m_message(message)
    {
    }

    const char* cpu::fmt_err::what() const noexcept
    {
        return m_message.c_str();
    }

    cpu::cpu(const std::string& stat_path) noexcept :
        m_stat_path(stat_path),
        m_state_prev({ 0 })
    {
    }

    float cpu::poll()
    {
        state state_curr = get_state();

        uint64_t work_d = state_curr.work - m_state_prev.work;
        uint64_t total_d = state_curr.total - m_state_prev.total;

        m_state_prev = state_curr;

        return (float)work_d / (float)total_d;
    }

    cpu::state cpu::get_state()
    {
        std::ifstream ifstream;

        ifstream.exceptions(std::ios::badbit | std::ios::failbit);

        try
        {
            ifstream.open(m_stat_path);
        }
        catch (std::ios::failure&)
        {
            std::stringstream message;
            message << "Failed to open \"" << m_stat_path << "\".";
            throw io_err(message.str());
        }

        std::string line;

        try
        {
            std::getline(ifstream, line);
        }
        catch (std::ios::failure&)
        {
            throw_io_err(m_stat_path);
        }

        std::stringstream line_stream;
        line_stream << line;

        if (!line_stream.eof())
        {
            std::string cpu_token;
            line_stream >> cpu_token;

            if (cpu_token != std::string("cpu"))
            {
                throw_fmt_err(m_stat_path);
            }
        }
        else
        {
            throw_fmt_err(m_stat_path);
        }

        std::vector<uint64_t> jiffies;
        jiffies.reserve(16);

        while (!line_stream.eof())
        {
            std::string num;
            line_stream >> num;

            try
            {
                jiffies.push_back(std::stoull(num));
            }
            catch (...)
            {
                throw_fmt_err(m_stat_path);
            }
        }

        if (jiffies.size() < 4)
        {
            throw_fmt_err(m_stat_path);
        }

        state result = { 0 };
        result.total = std::accumulate(jiffies.begin(), jiffies.end(), 0);
        result.work = std::accumulate(jiffies.begin(), jiffies.begin() + 3, 0);

        return result;
    }

}
