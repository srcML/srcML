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

#include <parse_request.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <threadpool.hpp>
#pragma GCC diagnostic pop
#include <boost/function.hpp>
#include <write_queue.hpp>

class ParseQueue {
public:

	ParseQueue(int max_threads, boost::function<void(ParseRequest*)> consumearg, WriteQueue& write_queue)
	    : consume(consumearg), pool(max_threads), counter(0), wqueue(write_queue) {}

	inline void schedule(ParseRequest* pvalue) {

	    pvalue->position = ++counter;

	    // error passthrough to output for proper output in trace
	    if (pvalue->status) {
	        pvalue->unit = 0;
	        wqueue.schedule(pvalue);
	        return;
	    }

	    pool.schedule( boost::bind(consume, pvalue));
	}

	inline void wait() {

		pool.wait();
	}

private:
    boost::function<void(ParseRequest*)> consume;
    boost::threadpool::pool pool;
    int counter;
    WriteQueue& wqueue;
};

#endif
