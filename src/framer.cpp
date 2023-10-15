#include "framer.h"

#include <sstream>

namespace pcat
{

    framer::framer(const std::string& frames) noexcept :
        m_curr(0),
        m_frames(m_cvt.from_bytes(frames)),
        m_count(m_frames.length())
    {
    }

    std::string framer::get() noexcept
    {
        std::stringstream frame;
        frame << m_cvt.to_bytes(m_frames.at(m_curr));

        m_curr = (m_curr + 1) % m_count;

        return frame.str();
    }

}
