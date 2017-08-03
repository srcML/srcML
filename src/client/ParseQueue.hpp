/**
 * @file parse_queue.hpp
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

#ifndef PARSE_QUEUE_HPP
#define PARSE_QUEUE_HPP

#include <functional>
#include <ParseRequest.hpp>
#include <WriteQueue.hpp>
#include <ctpl_stl.h>
#include <mutex>
#include <srcml_consume.hpp>

class ParseQueue {
public:

	ParseQueue(int max_threads, WriteQueue* write_queue)
	    : pool(max_threads), wqueue(write_queue) {}

	inline void schedule(ParseRequest* pvalue) {

		int next;
		{
			std::unique_lock<std::mutex> l(e);

	    	next = ++counter;
		}
		pvalue->position = next;

	    // error passthrough to output for proper output in trace
	    if (pvalue->status) {
	        pvalue->unit = 0;
	        wqueue->schedule(pvalue);
	        return;
	    }

        pool.push(std::bind(srcml_consume, pvalue, wqueue));
	}

	inline void wait() {

        pool.stop(true);
	}

private:
    ctpl::thread_pool pool;
    WriteQueue* wqueue;
    int counter = 0;
    std::mutex e;
};

#endif
