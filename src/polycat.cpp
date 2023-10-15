#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "framer.h"
#include "rate_poll.h"

const std::string STAT_FILE_PATH_DEFAULT = "/proc/stat";

std::chrono::milliseconds get_period(uint64_t low_rate, uint64_t high_rate,
    float cpu_load);

int main()
{
    std::setlocale(LC_ALL, "");

    pcat::framer framer("");
    pcat::rate_poll rate_poll(300, STAT_FILE_PATH_DEFAULT);

    uint64_t low_rate = 3;
    uint64_t high_rate = 90;

    std::thread poll_thread([](pcat::rate_poll& rate_poll) { rate_poll.run(); },
        std::ref(rate_poll));

    bool err = false;
    while (1)
    {
        using namespace std::chrono;
        auto point = steady_clock::now();

        if (rate_poll.io_err())
        {
            std::cout << "CPU polling IO error, file " << STAT_FILE_PATH_DEFAULT
                << std::endl << rate_poll.io_err_what() << std::endl;
            err = true;
            break;
        }

        if (rate_poll.fmt_err())
        {
            std::cout << "CPU polling format error, file "
                << STAT_FILE_PATH_DEFAULT << std::endl
                << rate_poll.fmt_err_what() << std::endl;
            err = true;
            break;
        }

        float load = rate_poll.poll();
        point += get_period(low_rate, high_rate, load);

        framer.put();

        std::this_thread::sleep_until(point);
    }

    poll_thread.join();

    return err ? EXIT_FAILURE : EXIT_SUCCESS;
}

std::chrono::milliseconds get_period(uint64_t low_rate, uint64_t high_rate,
    float cpu_load)
{
    uint64_t diff = high_rate - low_rate;

    uint64_t rate = low_rate + (uint64_t)(cpu_load * diff);

    uint64_t period = 1000 / rate;

    return std::chrono::milliseconds(period);
}
