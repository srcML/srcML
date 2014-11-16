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
	Timer(int limit) : time_limit(limit) {}

	inline void start() {
		gettimeofday(&start_t,NULL);
	}

	// time in milliseconds
	inline double elapsed() {
		struct timeval end;
		gettimeofday(&end,NULL);
		long microseconds = (end.tv_sec - start_t.tv_sec) * 1000000 + ((long)end.tv_usec - (long)start_t.tv_usec);
		long milliseconds = microseconds / 1000;
		return milliseconds; /* microseconds */
	}

	inline bool is_expired() {
		struct timeval end;
		gettimeofday(&end,NULL);
		return (((end.tv_sec - start_t.tv_sec) >= time_limit) && time_limit != 0);
	}

	inline void set_limit(int limit) {
		time_limit = limit;
	}

private:
	struct timeval start_t;
    int time_limit; // in seconds
};

#endif