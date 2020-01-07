/**
 * @file Timer.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#ifdef _MSC_BUILD
#include <time.h>
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <chrono>
#pragma GCC diagnostic pop

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
        #ifdef _MSC_BUILD
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
        #ifdef _MSC_BUILD
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
    #ifdef _MSC_BUILD
        clock_t cpu_time;
    #else
        std::clock_t cpu_time;
    #endif
    unsigned int time_limit = 0; // in seconds
};

#endif
