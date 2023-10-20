#include "conf.h"

#include <fstream>
#include <sstream>
#include <limits>

#include <nlohmann/json.hpp>

namespace pcat
{

const std::string conf::FRAMES_DEFAULT = "";

const uint8_t conf::LOW_RATE_DEFAULT = 3;

const uint8_t conf::HIGH_RATE_DEFAULT = 90;

const uint64_t conf::POLL_PERIOD_DEFAULT = 500;

const bool conf::SMOOTHING_ENABLED_DEFAULT = true;

const uint64_t conf::SMOOTHING_VALUE_DEFAULT = 1200;

const std::string conf::FRAMES_KEY = "frames";

const std::string conf::HIGH_RATE_KEY = "highRate";

const std::string conf::LOW_RATE_KEY = "lowRate";

const std::string conf::POLL_PERIOD_KEY = "pollingPeriod";

const std::string conf::SMOOTHING_ENABLED_KEY = "smoothEnabled";

const std::string conf::SMOOTHING_VALUE_KEY = "smoothValue";

conf::open_err::open_err(const std::string& message) noexcept :
    m_message(message)
{
}

const char* conf::open_err::what() const noexcept { return m_message.c_str(); }

conf::read_err::read_err(const std::string& message) noexcept :
    m_message(message)
{
}

const char* conf::read_err::what() const noexcept { return m_message.c_str(); }

conf::parse_err::parse_err(const std::string& message) noexcept :
    m_message(message)
{
}

const char* conf::parse_err::what() const noexcept { return m_message.c_str(); }

conf::fmt_err::fmt_err(const std::string& message) noexcept :
    m_message(message)
{
}

const char* conf::fmt_err::what() const noexcept { return m_message.c_str(); }

conf::conf(const std::string& config_path) noexcept :
    m_path(config_path),
    m_frames(FRAMES_DEFAULT),
    m_low_rate(LOW_RATE_DEFAULT),
    m_high_rate(HIGH_RATE_DEFAULT),
    m_poll_period(POLL_PERIOD_DEFAULT),
    m_smoothing_enabled(SMOOTHING_ENABLED_DEFAULT),
    m_smoothing_value(SMOOTHING_VALUE_DEFAULT)
{
}

void conf::load()
{
    std::ifstream conf_exists;
    conf_exists.open(m_path);

    if (!conf_exists)
    {
        try
        {
            std::ofstream conf_empty;
            conf_empty.exceptions(std::ios::badbit | std::ios::failbit);

            conf_empty.open(m_path);
            conf_empty << "{\n}" << std::endl;
            conf_empty.close();
        }
        catch (std::ios::failure&)
        {
            throw open_err("Failed to create an empty config file.");
        }
    }

    conf_exists.close();

    std::ifstream ifstream;

    ifstream.exceptions(std::ios::badbit | std::ios::failbit);

    try
    {
        ifstream.open(m_path);
    }
    catch (std::ios::failure&)
    {
        throw open_err("Failed to open the file.");
    }

    std::string frames = "";
    uint64_t low_rate = 0;
    uint64_t high_rate = 0;
    uint64_t poll_period = 0;
    bool smoothing_enabled = false;
    uint64_t smoothing_value = 0;

    std::string last_checked;
    try
    {
        nlohmann::json json = nlohmann::json::parse(ifstream);

        last_checked = FRAMES_KEY;
        frames = json.value(FRAMES_KEY, FRAMES_DEFAULT);

        last_checked = LOW_RATE_KEY;
        low_rate = json.value(LOW_RATE_KEY, (uint64_t)LOW_RATE_DEFAULT);

        last_checked = HIGH_RATE_KEY;
        high_rate = json.value(HIGH_RATE_KEY, (uint64_t)HIGH_RATE_DEFAULT);

        last_checked = POLL_PERIOD_KEY;
        poll_period = json.value(POLL_PERIOD_KEY, POLL_PERIOD_DEFAULT);

        last_checked = SMOOTHING_ENABLED_KEY;
        smoothing_enabled =
            json.value(SMOOTHING_ENABLED_KEY, SMOOTHING_ENABLED_DEFAULT);

        last_checked = SMOOTHING_VALUE_KEY;
        smoothing_value =
            json.value(SMOOTHING_VALUE_KEY, SMOOTHING_VALUE_DEFAULT);
    }
    catch (std::ios::failure&)
    {
        throw read_err("Failed to read the file.");
    }
    catch (nlohmann::json::parse_error&)
    {
        throw read_err("Failed to parse the file.");
    }
    catch (nlohmann::json::type_error&)
    {
        std::stringstream message;
        message << "Invalid type for key \"" << last_checked << "\".";
        throw fmt_err(message.str());
    }
    catch (nlohmann::json::out_of_range&)
    {
        std::stringstream message;
        message << "Out of range error for key \"" << last_checked << "\".";
        throw fmt_err(message.str());
    }

    std::stringstream message;

    bool err = false;

    if (frames.length() < 1)
    {
        message << "Key \"" << FRAMES_KEY << "\" cannot be an empty string.";
        err = true;
    }

    if (low_rate < 1 || low_rate > std::numeric_limits<uint8_t>().max())
    {
        if (err)
        {
            message << "\n";
        }
        message << "Key \"" << LOW_RATE_KEY
                << "\" should be a number in range [1-255] inclusive.";
        err = true;
    }

    if (high_rate < 1 || high_rate > std::numeric_limits<uint8_t>().max())
    {
        if (err)
        {
            message << "\n";
        }
        message << "Key \"" << HIGH_RATE_KEY
                << "\" should be a number in range [1-255] inclusive.";
        err = true;
    }

    if (poll_period < 1)
    {
        if (err)
        {
            message << "\n";
        }
        message << "Key \"" << POLL_PERIOD_KEY
                << "\" should be a number greater than 1.";
        err = true;
    }

    if (smoothing_value < 1 || smoothing_value > 10'000)
    {
        if (err)
        {
            message << "\n";
        }
        message << "Key \"" << SMOOTHING_VALUE_KEY
                << "\" should be a number in range [1-10000] inclusive";
        err = true;
    }

    if (err)
    {
        throw fmt_err(message.str());
    }

    m_frames = frames;
    m_low_rate = low_rate;
    m_high_rate = high_rate;
    m_poll_period = poll_period;
    m_smoothing_enabled = smoothing_enabled;
    m_smoothing_value = smoothing_value;
}

std::string conf::frames() const noexcept { return m_frames; }

uint8_t conf::high_rate() const noexcept { return m_high_rate; }

uint8_t conf::low_rate() const noexcept { return m_low_rate; }

uint64_t conf::poll_period() const noexcept { return m_poll_period; }

bool conf::smoothing_enabled() const noexcept { return m_smoothing_enabled; }

uint64_t conf::smoothing_value() const noexcept { return m_smoothing_value; }

}
