// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Timer.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#ifdef _MSC_VER
#include <time.h>
#endif

#include <chrono>

class Timer {
public:
    Timer() {
        start();
    }
    Timer(unsigned int limit) : time_limit(limit) {
        start();
    }

    inline void start() {
        real_world_time = std::chrono::high_resolution_clock::now();
        #ifdef _MSC_VER
            cpu_time = clock();
        #else
            cpu_time = std::clock();
        #endif
    }

    // time in milliseconds
    inline double real_world_elapsed() {
        return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - real_world_time).count();
    }

    // time in milliseconds
    inline double cpu_time_elapsed() {
        #ifdef _MSC_VER
            return  1000.0 * (clock() - cpu_time) / CLOCKS_PER_SEC;
        #else
            return  1000.0 * (std::clock() - cpu_time) / CLOCKS_PER_SEC;
        #endif
    }

    inline bool is_expired() {
        return (time_limit != 0 && (cpu_time_elapsed() >= time_limit));
    }

    inline void set_limit(unsigned int limit) {
        time_limit = limit;
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> real_world_time;
    #ifdef _MSC_VER
        clock_t cpu_time;
    #else
        std::clock_t cpu_time;
    #endif
    unsigned int time_limit = 0; // in seconds
};

#endif
