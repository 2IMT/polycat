#include "rate_poll.h"

#include <thread>

#include "cpu.h"

namespace pcat
{

    rate_poll::rate_poll(uint64_t period,
        const std::string& stat_path) noexcept :
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

        auto period = milliseconds(m_period);

        auto next = steady_clock::now();
        auto prev = next - period;
        while (true)
        {
            m_done_mut.lock();
            if (m_done)
            {
                m_done_mut.unlock();
                break;
            }
            m_done_mut.unlock();

            auto now = steady_clock::now();
            prev = now;
            next += period;

            float cpu_load = 0.0f;

            try
            {
                cpu_load = m_cpu.poll();
            }
            catch (cpu::io_err&)
            {
                std::lock_guard guard(m_io_err_mut);
                m_io_err = true;
                break;
            }
            catch (cpu::fmt_err&)
            {
                std::lock_guard guard(m_fmt_err_mut);
                m_fmt_err = true;
                break;
            }

            m_cpu_load_mut.lock();
            m_cpu_load = cpu_load;
            m_cpu_load_mut.unlock();

            std::this_thread::sleep_until(next);
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

    bool rate_poll::fmt_err() noexcept
    {
        std::lock_guard guard(m_fmt_err_mut);
        return m_fmt_err;
    }

    float rate_poll::poll() noexcept
    {
        std::lock_guard guard(m_cpu_load_mut);
        return m_cpu_load;
    }

}
