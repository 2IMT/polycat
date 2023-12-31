#include "conf.h"

#include <filesystem>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <limits>

#include <nlohmann/json.hpp>

namespace pcat
{

    const std::string conf::FRAMES_DEFAULT = "";

    const uint8_t conf::LOW_RATE_DEFAULT = 2;

    const uint8_t conf::HIGH_RATE_DEFAULT = 30;

    const uint64_t conf::POLL_PERIOD_DEFAULT = 1000;

    const bool conf::SMOOTHING_ENABLED_DEFAULT = true;

    const uint64_t conf::SMOOTHING_VALUE_DEFAULT = 2000;

    const bool conf::SLEEPING_ENABLED_DEFAULT = true;

    const uint64_t conf::SLEEPING_THRESHOLD_DEFAULT = 8;

    const uint64_t conf::WAKEUP_THRESHOLD_DEFAULT = 12;

    const std::string conf::SLEEPING_FRAMES_DEFAULT = "";

    const uint64_t conf::SLEEPING_RATE_DEFAULT = 4;

    const bool conf::FORMAT_ENABLED_DEFAULT = false;

    const std::string conf::FORMAT_DEFAULT = "$frame $lcpu";

    const std::string conf::FRAMES_KEY = "frames";

    const std::string conf::HIGH_RATE_KEY = "highRate";

    const std::string conf::LOW_RATE_KEY = "lowRate";

    const std::string conf::POLL_PERIOD_KEY = "pollingPeriod";

    const std::string conf::SMOOTHING_ENABLED_KEY = "smoothEnabled";

    const std::string conf::SMOOTHING_VALUE_KEY = "smoothValue";

    const std::string conf::SLEEPING_ENABLED_KEY = "sleepEnabled";

    const std::string conf::SLEEPING_THRESHOLD_KEY = "sleepThreshold";

    const std::string conf::WAKEUP_THRESHOLD_KEY = "wakeupThreshold";

    const std::string conf::SLEEPING_FRAMES_KEY = "sleepFrames";

    const std::string conf::SLEEPING_RATE_KEY = "sleepRate";

    const std::string conf::FORMAT_ENABLED_KEY = "formatEnabled";

    const std::string conf::FORMAT_KEY = "format";

    void create_default_config(const std::string& path)
    {
        std::ofstream ofstream;
        ofstream.exceptions(std::ios::badbit | std::ios::failbit);
        ofstream.open(path);

        nlohmann::ordered_json config = {
            {            conf::FRAMES_KEY,             conf::FRAMES_DEFAULT},

            {         conf::HIGH_RATE_KEY,          conf::HIGH_RATE_DEFAULT},

            {          conf::LOW_RATE_KEY,           conf::LOW_RATE_DEFAULT},

            {       conf::POLL_PERIOD_KEY,        conf::POLL_PERIOD_DEFAULT},

            { conf::SMOOTHING_ENABLED_KEY,  conf::SMOOTHING_ENABLED_DEFAULT},

            {   conf::SMOOTHING_VALUE_KEY,    conf::SMOOTHING_VALUE_DEFAULT},

            {  conf::SLEEPING_ENABLED_KEY,   conf::SLEEPING_ENABLED_DEFAULT},

            {conf::SLEEPING_THRESHOLD_KEY, conf::SLEEPING_THRESHOLD_DEFAULT},

            {  conf::WAKEUP_THRESHOLD_KEY,   conf::WAKEUP_THRESHOLD_DEFAULT},

            {   conf::SLEEPING_FRAMES_KEY,    conf::SLEEPING_FRAMES_DEFAULT},

            {     conf::SLEEPING_RATE_KEY,      conf::SLEEPING_RATE_DEFAULT},

            {    conf::FORMAT_ENABLED_KEY,     conf::FORMAT_ENABLED_DEFAULT},

            {            conf::FORMAT_KEY,             conf::FORMAT_DEFAULT}
        };

        ofstream << std::setw(4) << config << std::endl;
    }

    conf::open_err::open_err(const std::string& message) noexcept :
        m_message(message)
    {
    }

    const char* conf::open_err::what() const noexcept
    {
        return m_message.c_str();
    }

    conf::read_err::read_err(const std::string& message) noexcept :
        m_message(message)
    {
    }

    const char* conf::read_err::what() const noexcept
    {
        return m_message.c_str();
    }

    conf::parse_err::parse_err(const std::string& message) noexcept :
        m_message(message)
    {
    }

    const char* conf::parse_err::what() const noexcept
    {
        return m_message.c_str();
    }

    conf::fmt_err::fmt_err(const std::string& message) noexcept :
        m_message(message)
    {
    }

    const char* conf::fmt_err::what() const noexcept
    {
        return m_message.c_str();
    }

    conf::conf(const std::string& config_path) noexcept :
        m_path(config_path),
        m_frames(FRAMES_DEFAULT),
        m_low_rate(LOW_RATE_DEFAULT),
        m_high_rate(HIGH_RATE_DEFAULT),
        m_poll_period(POLL_PERIOD_DEFAULT),
        m_smoothing_enabled(SMOOTHING_ENABLED_DEFAULT),
        m_smoothing_value(SMOOTHING_VALUE_DEFAULT),
        m_sleeping_enabled(SLEEPING_ENABLED_DEFAULT),
        m_sleeping_threshold(SLEEPING_THRESHOLD_DEFAULT),
        m_wakeup_threshold(WAKEUP_THRESHOLD_DEFAULT),
        m_sleeping_frames(SLEEPING_FRAMES_DEFAULT),
        m_sleeping_rate(SLEEPING_RATE_DEFAULT),
        m_format_enabled(FORMAT_ENABLED_DEFAULT),
        m_format(FORMAT_DEFAULT)
    {
    }

    void conf::load()
    {
        if (!std::filesystem::exists(m_path))
        {
            try
            {
                create_default_config(m_path);
            }
            catch (std::ios::failure&)
            {
                throw open_err("Failed to create an empty config file.");
            }
        }

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
        bool sleeping_enabled = false;
        uint64_t sleeping_threshold = 0;
        uint64_t wakeup_threshold = 0;
        std::string sleeping_frames = "";
        uint64_t sleeping_rate = 0;
        bool format_enabled = false;
        std::string format = "";

        std::string last_checked;
        try
        {
            nlohmann::json json = nlohmann::json::parse(ifstream);

            last_checked = FRAMES_KEY;
            frames = json.value(FRAMES_KEY, FRAMES_DEFAULT);

            last_checked = LOW_RATE_KEY;
            low_rate = json.value(
                LOW_RATE_KEY, static_cast<uint64_t>(LOW_RATE_DEFAULT));

            last_checked = HIGH_RATE_KEY;
            high_rate = json.value(
                HIGH_RATE_KEY, static_cast<uint64_t>(HIGH_RATE_DEFAULT));

            last_checked = POLL_PERIOD_KEY;
            poll_period = json.value(POLL_PERIOD_KEY, POLL_PERIOD_DEFAULT);

            last_checked = SMOOTHING_ENABLED_KEY;
            smoothing_enabled =
                json.value(SMOOTHING_ENABLED_KEY, SMOOTHING_ENABLED_DEFAULT);

            last_checked = SMOOTHING_VALUE_KEY;
            smoothing_value =
                json.value(SMOOTHING_VALUE_KEY, SMOOTHING_VALUE_DEFAULT);

            last_checked = SLEEPING_ENABLED_KEY;
            sleeping_enabled =
                json.value(SLEEPING_ENABLED_KEY, SLEEPING_ENABLED_DEFAULT);

            last_checked = SLEEPING_THRESHOLD_KEY;
            sleeping_threshold =
                json.value(SLEEPING_THRESHOLD_KEY, SLEEPING_THRESHOLD_DEFAULT);

            last_checked = WAKEUP_THRESHOLD_KEY;
            wakeup_threshold =
                json.value(WAKEUP_THRESHOLD_KEY, WAKEUP_THRESHOLD_DEFAULT);

            last_checked = SLEEPING_FRAMES_KEY;
            sleeping_frames =
                json.value(SLEEPING_FRAMES_KEY, SLEEPING_FRAMES_DEFAULT);

            last_checked = SLEEPING_RATE_KEY;
            sleeping_rate =
                json.value(SLEEPING_RATE_KEY, SLEEPING_RATE_DEFAULT);

            last_checked = FORMAT_ENABLED_KEY;
            format_enabled =
                json.value(FORMAT_ENABLED_KEY, FORMAT_ENABLED_DEFAULT);

            last_checked = FORMAT_KEY;
            format = json.value(FORMAT_KEY, FORMAT_DEFAULT);
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
            message << "Key \"" << FRAMES_KEY
                    << "\" cannot be an empty string.";
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

        if (sleeping_threshold < 1 || sleeping_threshold > 100)
        {
            if (err)
            {
                message << "\n";
            }
            message << "Key \"" << SLEEPING_THRESHOLD_KEY
                    << "\" should be a number in range [1-100] inclusive";
            err = true;
        }

        if (wakeup_threshold < 1 || wakeup_threshold > 100)
        {
            if (err)
            {
                message << "\n";
            }
            message << "Key \"" << WAKEUP_THRESHOLD_KEY
                    << "\" should be a number in range [1-100] inclusive";
            err = true;
        }

        if (sleeping_threshold > wakeup_threshold)
        {
            if (err)
            {
                message << "\n";
            }
            message << "\"" << WAKEUP_THRESHOLD_KEY
                    << "\" should be greater than \"" << SLEEPING_THRESHOLD_KEY
                    << "\"";
        }

        if (sleeping_frames.length() < 1)
        {
            if (err)
            {
                message << "\n";
            }
            message << "Key \"" << SLEEPING_FRAMES_KEY
                    << "\" cannot be an empty string.";
            err = true;
        }

        if (sleeping_rate < 1 ||
            sleeping_rate > std::numeric_limits<uint8_t>().max())
        {
            if (err)
            {
                message << "\n";
            }
            message << "Key \"" << SLEEPING_RATE_KEY
                    << "\" should be a number in range [1-255] inclusive.";
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
        m_sleeping_enabled = sleeping_enabled;
        m_sleeping_threshold = sleeping_threshold;
        m_wakeup_threshold = wakeup_threshold;
        m_sleeping_frames = sleeping_frames;
        m_sleeping_rate = sleeping_rate;
        m_format_enabled = format_enabled;
        m_format = format;
    }

    std::string conf::frames() const noexcept { return m_frames; }

    uint8_t conf::high_rate() const noexcept { return m_high_rate; }

    uint8_t conf::low_rate() const noexcept { return m_low_rate; }

    uint64_t conf::poll_period() const noexcept { return m_poll_period; }

    bool conf::smoothing_enabled() const noexcept
    {
        return m_smoothing_enabled;
    }

    uint64_t conf::smoothing_value() const noexcept
    {
        return m_smoothing_value;
    }

    bool conf::sleeping_enabled() const noexcept { return m_sleeping_enabled; }

    uint8_t conf::sleeping_threshold() const noexcept
    {
        return m_sleeping_threshold;
    }

    uint8_t conf::wakeup_threshold() const noexcept
    {
        return m_wakeup_threshold;
    }

    std::string conf::sleeping_frames() const noexcept
    {
        return m_sleeping_frames;
    }

    uint8_t conf::sleeping_rate() const noexcept { return m_sleeping_rate; }

    bool conf::format_enabled() const noexcept { return m_format_enabled; }

    std::string conf::format() const noexcept { return m_format; }
}
