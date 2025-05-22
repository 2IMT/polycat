#include "conf.h"

#include <limits>

namespace pcat
{

    conf::fmt_err::fmt_err(const std::string& message) noexcept :
        m_message(message)
    {
    }

    const char* conf::fmt_err::what() const noexcept
    {
        return m_message.c_str();
    }

    bool conf::load_errs::any() const
    {
        return !parse_errs.empty() || !type_errs.empty() ||
               !no_key_errs.empty() || !fmt_errs.empty();
    }

    conf::conf(const std::string& config_path) noexcept :
        m_path(config_path)
    {
    }

    conf::load_errs conf::load()
    {
        load_errs errs;

        parse p;

        errs.parse_errs = p.load(m_path);

        if (errs.any())
        {
            return errs;
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

        bool frames_loaded = false;
        bool low_rate_loaded = false;
        bool high_rate_loaded = false;
        bool poll_period_loaded = false;
        [[maybe_unused]] bool smoothing_enabled_loaded = false;
        bool smoothing_value_loaded = false;
        [[maybe_unused]] bool sleeping_enabled_loaded = false;
        bool sleeping_threshold_loaded = false;
        bool wakeup_threshold_loaded = false;
        bool sleeping_frames_loaded = false;
        bool sleeping_rate_loaded = false;
        [[maybe_unused]] bool format_enabled_loaded = false;
        [[maybe_unused]] bool format_loaded = false;

#define _GET_VALUE(name, type) \
    try \
    { \
        name = p.get_##type(#name); \
        name##_loaded = true; \
    } \
    catch (const parse::no_key_err& e) \
    { \
        errs.no_key_errs.push_back(e); \
    } \
    catch (const parse::type_err& e) \
    { \
        errs.type_errs.push_back(e); \
    }

        _GET_VALUE(frames, string);
        _GET_VALUE(low_rate, int);
        _GET_VALUE(high_rate, int);
        _GET_VALUE(poll_period, int);
        _GET_VALUE(smoothing_enabled, bool);
        _GET_VALUE(smoothing_value, int);
        _GET_VALUE(sleeping_enabled, bool);
        _GET_VALUE(sleeping_threshold, int);
        _GET_VALUE(wakeup_threshold, int);
        _GET_VALUE(sleeping_frames, string);
        _GET_VALUE(sleeping_rate, int);
        _GET_VALUE(format_enabled, bool);
        _GET_VALUE(format, string);

#undef _GET_VALUE

        if (frames_loaded && (frames.length() < 1))
        {
            errs.fmt_errs.push_back(
                fmt_err("`frames` cannot be an empty string"));
        }

        if (low_rate_loaded &&
            (low_rate < 1 || low_rate > std::numeric_limits<uint8_t>().max()))
        {
            errs.fmt_errs.push_back(
                fmt_err("`low_rate` should be an integer in range [1-255]"));
        }

        if (high_rate_loaded &&
            (high_rate < 1 || high_rate > std::numeric_limits<uint8_t>().max()))
        {
            errs.fmt_errs.push_back(
                fmt_err("`high_rate` should be an integer in range [1-255]"));
        }

        if (poll_period_loaded && (poll_period < 1))
        {
            errs.fmt_errs.push_back(
                fmt_err("`poll_period` should be an integer greater than 1"));
        }

        if (smoothing_value_loaded &&
            (smoothing_value < 1 || smoothing_value > 10'000))
        {
            errs.fmt_errs.push_back(fmt_err(
                "`smoothing_value` should be an integer in range [1-10000]"));
        }

        if (sleeping_threshold_loaded &&
            (sleeping_threshold < 1 || sleeping_threshold > 100))
        {
            errs.fmt_errs.push_back(fmt_err(
                "`sleeping_threshold` should be an integer in range [1-100]"));
        }

        if (wakeup_threshold_loaded &&
            (wakeup_threshold < 1 || wakeup_threshold > 100))
        {
            errs.fmt_errs.push_back(fmt_err(
                "`wakeup_threshold` should be an integer in range [1-100]"));
        }

        if (sleeping_threshold_loaded && wakeup_threshold_loaded &&
            (sleeping_threshold > wakeup_threshold))
        {
            errs.fmt_errs.push_back(
                fmt_err("`wakeup_threshold` should be greater "
                        "than `sleeping_threshold`"));
        }

        if (sleeping_frames_loaded && (sleeping_frames.length() < 1))
        {
            errs.fmt_errs.push_back(
                fmt_err("`sleeping_frames` cannot be an empty string"));
        }

        if (sleeping_rate_loaded &&
            (sleeping_rate < 1 ||
                sleeping_rate > std::numeric_limits<uint8_t>().max()))
        {
            errs.fmt_errs.push_back(fmt_err(
                "`sleeping_rate` should be an integer in range [1-255]"));
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

        return errs;
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
