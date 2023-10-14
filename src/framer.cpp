#include "framer.h"

namespace pcat
{

    framer::framer(const std::u32string& frames) noexcept :
        m_curr(0),
        m_frames(frames),
        m_count(frames.length())
    {
    }

    void framer::put() noexcept
    {
        std::u32string frame(1, m_frames[m_curr]);

        std::cout << m_cvt.to_bytes(frame) << std::endl;

        m_curr = (m_curr + 1) % m_count;
    }

}
