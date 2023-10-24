#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <exception>

namespace pcat
{

/**
 * @brief Polls CPU usage
 */
class cpu
{
public:
    /**
     * @brief Thrown on IO errors
     */
    class io_err : public std::exception
    {
    public:
        io_err(const std::string& message) noexcept;

        const char* what() const noexcept;

    private:
        std::string m_message;
    };

    /**
     * @brief Thrown on stat file format errors
     */
    class fmt_err : public std::exception
    {
    public:
        fmt_err(const std::string& message) noexcept;

        const char* what() const noexcept;

    private:
        std::string m_message;
    };

    /**
     * @brief Constructs an instance that polls specific stat file
     * @param stat_path Stat file path
     */
    cpu(const std::string& stat_path) noexcept;

    /**
     * @brief Polls stat file and calculates CPU usage
     * @return CPU usage in range [0-1]
     * @exception pcat::cpu::io_err
     * @exception pcat::cpu::fmt_err
     */
    float poll();

private:
    /**
     * @brief CPU state
     */
    struct state
    {
        uint64_t total;
        uint64_t work;
    };

    std::string m_stat_path;
    state m_state_prev;

    /**
     * @brief Extracts CPU state from stat file
     * @return CPU state structure
     * @exception pcat::cpu::io_err
     * @exception pcat::cpu::fmt_err
     */
    state get_state();
};

}
