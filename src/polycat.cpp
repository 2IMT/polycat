#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "framer.h"
#include "rate_poll.h"

std::chrono::milliseconds get_period(uint64_t low_rate, uint64_t high_rate,
    float cpu_load);

int main()
{
    std::setlocale(LC_ALL, "");

    pcat::framer framer("");
    pcat::rate_poll rate_poll(300, "/proc/stat");
    
    uint64_t low_rate = 3;
    uint64_t high_rate = 90;

    std::thread poll_thread([](pcat::rate_poll& rate_poll) { rate_poll.run(); },
        std::ref(rate_poll));

    while (1)
    {
        using namespace std::chrono;
        auto point = steady_clock::now();

        if (rate_poll.io_err())
        {
            std::cout << "io_err!" << std::endl;
            break;
        }

        if (rate_poll.fmt_err())
        {
            std::cout << "fmt_err!" << std::endl;
            break;
        }

        float load = rate_poll.poll();
        point += get_period(low_rate, high_rate, load);

        framer.put();

        std::this_thread::sleep_until(point);
    }

    poll_thread.join();

    return EXIT_SUCCESS;
}

std::chrono::milliseconds get_period(uint64_t low_rate, uint64_t high_rate,
    float cpu_load)
{
    uint64_t diff = high_rate - low_rate;

    uint64_t rate = low_rate + (uint64_t)(cpu_load * diff);

    uint64_t period = 1000 / rate;

    return std::chrono::milliseconds(period);
}
