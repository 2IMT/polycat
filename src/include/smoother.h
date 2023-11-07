#pragma once

#include <chrono>
#include <cstdint>

namespace pcat
{

    /**
     * @brief Makes CPU polling results smooth
     */
    class smoother
    {
    public:
        /**
         * @brief Constructs an instance with specified smoothing period
         * @param period Period of smoothing
         */
        smoother(uint64_t period) noexcept;

        /**
         * @brief Sets current target for smoother
         * @param target Target
         */
        void target(float target) noexcept;

        /**
         * @brief Retrieves smoothed value based on delta
         * @param delta Time difference
         */
        float value(uint64_t delta) noexcept;

    private:
        uint64_t m_period;
        float m_target;
        float m_prev;
    };

}
