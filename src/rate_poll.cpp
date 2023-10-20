#include "rate_poll.h"

#include <thread>

#include "cpu.h"

namespace pcat
{

rate_poll::rate_poll(uint64_t period, const std::string& stat_path) noexcept :
    m_cpu(stat_path),
    m_period(period),
    m_done(false),
    m_io_err(false),
    m_fmt_err(false),
    m_cpu_load(0.0f)
{
}

void rate_poll::run() noexcept
{
    using namespace std::chrono;

    while (true)
    {
        m_done_mut.lock();
        if (m_done)
        {
            m_done_mut.unlock();
            break;
        }
        m_done_mut.unlock();

        auto point = steady_clock::now();
        point += m_period;

        float cpu_load = 0.0f;

        try
        {
            cpu_load = m_cpu.poll();
        }
        catch (cpu::io_err& e)
        {
            std::lock_guard guard(m_io_err_mut);
            m_io_err = true;
            m_io_err_what = e.what();
            break;
        }
        catch (cpu::fmt_err& e)
        {
            std::lock_guard guard(m_fmt_err_mut);
            m_fmt_err = true;
            m_fmt_err_what = e.what();
            break;
        }

        m_cpu_load_mut.lock();
        m_cpu_load = cpu_load;
        m_cpu_load_mut.unlock();

        std::this_thread::sleep_until(point);
    }
}

void rate_poll::stop() noexcept
{
    std::lock_guard guard(m_done_mut);
    m_done = true;
}

bool rate_poll::io_err() noexcept
{
    std::lock_guard guard(m_io_err_mut);
    return m_io_err;
}

std::string rate_poll::io_err_what() noexcept
{
    std::lock_guard guard(m_io_err_mut);
    return m_io_err_what;
}

bool rate_poll::fmt_err() noexcept
{
    std::lock_guard guard(m_fmt_err_mut);
    return m_fmt_err;
}

std::string rate_poll::fmt_err_what() noexcept
{
    std::lock_guard guard(m_fmt_err_mut);
    return m_fmt_err_what;
}

float rate_poll::poll() noexcept
{
    std::lock_guard guard(m_cpu_load_mut);
    return m_cpu_load;
}

}
