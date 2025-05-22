#include "framer.h"

static std::u32string _utf8_to_utf32(const std::string& utf8)
{
    std::u32string result;
    size_t i = 0;
    while (i < utf8.size())
    {
        uint32_t cp = 0;
        unsigned char ch = utf8[i];
        if (ch < 0x80)
        {
            cp = ch;
            i += 1;
        }
        else if ((ch >> 5) == 0x6)
        {
            cp = ((utf8[i] & 0x1F) << 6) | (utf8[i + 1] & 0x3F);
            i += 2;
        }
        else if ((ch >> 4) == 0xE)
        {
            cp = ((utf8[i] & 0x0F) << 12) | ((utf8[i + 1] & 0x3F) << 6) |
                 (utf8[i + 2] & 0x3F);
            i += 3;
        }
        else if ((ch >> 3) == 0x1E)
        {
            cp = ((utf8[i] & 0x07) << 18) | ((utf8[i + 1] & 0x3F) << 12) |
                 ((utf8[i + 2] & 0x3F) << 6) | (utf8[i + 3] & 0x3F);
            i += 4;
        }
        result += cp;
    }
    return result;
}

static std::string _encode_utf8(char32_t cp)
{
    std::string result;

    if (cp <= 0x7F)
    {
        result += static_cast<char>(cp);
    }
    else if (cp <= 0x7FF)
    {
        result += static_cast<char>(0xC0 | ((cp >> 6) & 0x1F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else if (cp <= 0xFFFF)
    {
        result += static_cast<char>(0xE0 | ((cp >> 12) & 0x0F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    else if (cp <= 0x10'FF'FF)
    {
        result += static_cast<char>(0xF0 | ((cp >> 18) & 0x07));
        result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }

    return result;
}

namespace pcat
{

    framer::framer(const std::string& frames) noexcept :
        m_curr(0),
        m_frames(_utf8_to_utf32(frames)),
        m_count(m_frames.length())
    {
    }

    std::string framer::get() noexcept
    {
        std::string frame = _encode_utf8(m_frames.at(m_curr));
        m_curr = (m_curr + 1) % m_count;
        return frame;
    }

}
