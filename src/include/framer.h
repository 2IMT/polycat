#pragma once

#include <string>
#include <locale>
#include <codecvt>
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
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> m_cvt;
    uint64_t m_curr;
    std::u32string m_frames;
    uint64_t m_count;
};

}
