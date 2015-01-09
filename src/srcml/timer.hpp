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
#include <boost/timer.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma GCC diagnostic pop

class Timer {
public:
	Timer() : time_limit(0) {}
	Timer(double limit) : time_limit(limit) {}

	inline void start() {
		real_world_time = boost::posix_time::microsec_clock::local_time();
		cpu_time.restart();
	}

	// time in milliseconds
	inline long real_world_elapsed() {
		return  (boost::posix_time::microsec_clock::local_time() - real_world_time).total_milliseconds();
	}

	// time in milliseconds
	inline long cpu_time_elapsed() {
		return cpu_time.elapsed() * 10000;
	}

	inline bool is_expired() {
		return ((cpu_time.elapsed() >= time_limit) && time_limit != 0);
	}

	inline void set_limit(double limit) {
		time_limit = limit;
	}

private:
	boost::posix_time::ptime real_world_time;
	boost::timer cpu_time;
    int time_limit; // in seconds
};

#endif
