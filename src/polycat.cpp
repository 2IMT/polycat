#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>

#include "args.h"
#include "conf.h"
#include "framer.h"
#include "smoother.h"
#include "rate_poll.h"

uint64_t get_period(uint64_t low_rate, uint64_t high_rate, float cpu_load);

int main(int argc, char** argv)
{
    std::setlocale(LC_ALL, "");

    pcat::args args(argc, argv);

    try
    {
        args.parse();
    }
    catch (pcat::args::parse_err& e)
    {
        std::cout << "Arguments error" << std::endl << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    pcat::conf conf(args.conf_path());

    try
    {
        conf.load();
    }
    catch (std::exception& e)
    {
        std::cout << "Config error, file " << args.conf_path() << std::endl
                  << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    pcat::framer framer(conf.frames());
    pcat::framer sleeping_framer(conf.sleeping_frames());
    pcat::rate_poll rate_poll(conf.poll_period(), args.stat_path());
    pcat::smoother smoother(conf.smoothing_value());

    uint64_t low_rate = conf.low_rate();
    uint64_t high_rate = conf.high_rate();

    std::thread poll_thread([](pcat::rate_poll& rate_poll) { rate_poll.run(); },
        std::ref(rate_poll));

    bool err = false;
    uint64_t period_prev = get_period(low_rate, high_rate, 0.0f);
    bool sleeping = false;
    while (true)
    {
        using namespace std::chrono;
        auto point = steady_clock::now();

        if (rate_poll.io_err())
        {
            std::cout << "CPU polling IO error, file " << args.stat_path()
                      << std::endl
                      << rate_poll.io_err_what() << std::endl;
            err = true;
            break;
        }

        if (rate_poll.fmt_err())
        {
            std::cout << "CPU polling format error, file " << args.stat_path()
                      << std::endl
                      << rate_poll.fmt_err_what() << std::endl;
            err = true;
            break;
        }

        float load = rate_poll.poll();
        smoother.target(load);
        float load_smoothed = smoother.value(period_prev);
        float load_displayed = conf.smoothing_enabled() ? load_smoothed : load;

        // Change sleeping state
        if (!sleeping)
        {
            sleeping = conf.sleeping_enabled()
                         ? load_displayed <= conf.sleeping_threshold() / 100.0f
                         : false;
        }
        else
        {
            sleeping = (load_displayed <= conf.wakeup_threshold() / 100.0f);
        }

        // Set the period and print the cat
        if (!sleeping)
        {
            uint64_t period = get_period(low_rate, high_rate, load_displayed);
            point += std::chrono::milliseconds(period);
            period_prev = period;
            std::cout << framer.get() << std::endl;
        }
        else
        {
            uint64_t period = 1000.0f / conf.sleeping_rate();
            point += std::chrono::milliseconds(period);
            std::cout << sleeping_framer.get() << std::endl;
        }

        std::this_thread::sleep_until(point);
    }

    poll_thread.join();

    return err ? EXIT_FAILURE : EXIT_SUCCESS;
}

uint64_t get_period(uint64_t low_rate, uint64_t high_rate, float cpu_load)
{
    uint64_t diff = high_rate - low_rate;

    uint64_t rate = low_rate + (uint64_t)(cpu_load * diff);

    uint64_t period = 1000 / rate;

    return period;
}
