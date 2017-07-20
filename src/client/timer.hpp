/**
 * @timer.hpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef TIMER_HPP
#define TIMER_HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <chrono>
#pragma GCC diagnostic pop 

class Timer {
public:
	Timer() : time_limit(0) {}
	Timer(unsigned int limit) : time_limit(limit) {}

	inline void start() {
		real_world_time = std::chrono::high_resolution_clock::now();
		cpu_time = std::clock();
	}

	// time in milliseconds
	inline double real_world_elapsed() {
		return std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - real_world_time).count();
	}

	// time in milliseconds
	inline double cpu_time_elapsed() {
		return  1000.0 * (std::clock() - cpu_time) / CLOCKS_PER_SEC;
	}

	inline bool is_expired() {
		return (time_limit != 0 && (cpu_time_elapsed() >= time_limit));
	}

	inline void set_limit(unsigned int limit) {
		time_limit = limit;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> real_world_time;
	std::clock_t cpu_time;
    unsigned int time_limit; // in seconds
};

#endif
