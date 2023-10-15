#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "conf.h"
#include "framer.h"
#include "rate_poll.h"

const std::string STAT_FILE_PATH_DEFAULT = "/proc/stat";
const std::string CONF_FILE_PATH_DEFAULT = "polycat-config.json";

std::chrono::milliseconds get_period(uint64_t low_rate, uint64_t high_rate,
    float cpu_load);

int main()
{
    std::setlocale(LC_ALL, "");

    pcat::conf conf(CONF_FILE_PATH_DEFAULT);

    try
    {
        conf.load();
    }
    catch (std::exception& e)
    {
        std::cout << "Config error, file " << CONF_FILE_PATH_DEFAULT
            << std::endl << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    pcat::framer framer(conf.frames());
    pcat::rate_poll rate_poll(conf.poll_period(), STAT_FILE_PATH_DEFAULT);

    uint64_t low_rate = conf.low_rate();
    uint64_t high_rate = conf.high_rate();

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
