/**
 * @timer.hpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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

#include <time.h>

class Timer {
public:
	Timer() : time_limit(0) {}
	Timer(double limit) : time_limit(limit) {}

	inline void start() {
		start_time = clock();
	}

	// time in seconds
	inline double elapsed() {
		return (double)((clock() - start_time) / (CLOCKS_PER_SEC / 1000));
	}

	inline bool is_expired() {
		return ((((clock() - start_time)/CLOCKS_PER_SEC) >= time_limit) && time_limit != 0);
	}

	inline void set_limit(double limit) {
		time_limit = limit;
	}

private:
    double time_limit; // in seconds
    clock_t start_time; // start_timeing time
};

#endif