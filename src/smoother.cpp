#include "smoother.h"

#include <algorithm>

namespace pcat
{

    smoother::smoother(uint64_t period) noexcept :
        m_period(period),
        m_target(0.0f),
        m_prev(0.0f)
    {
    }

    void smoother::target(float target) noexcept { m_target = target; }

    float smoother::value(uint64_t delta) noexcept
    {
        if (m_prev == m_target)
        {
            return m_target;
        }

        float diff = m_target - m_prev;
        int sign = (diff > 0) - (diff < 0);
        float step = static_cast<float>(delta) / static_cast<float>(m_period);

        float result = m_prev;
        result += step * sign;
        result = (sign > 0) ? std::min(result, m_target)
                            : std::max(result, m_target);

        m_prev = result;

        return result;
    }

}
