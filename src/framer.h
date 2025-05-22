#pragma once

#include <string>
#include <cstdint>

namespace pcat
{

    class framer
    {
    public:
        /**
         * @brief Construct an instance with specified frames
         * @param frames String of frames
         */
        framer(const std::string& frames) noexcept;

        /**
         * @brief Tells the current frame and switches to next
         */
        std::string get() noexcept;

    private:
        uint64_t m_curr;
        std::u32string m_frames;
        uint64_t m_count;
    };

}
