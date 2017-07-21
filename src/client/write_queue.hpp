/**
 * @file write_queue.hpp
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

#ifndef WRITE_QUEUE_HPP
#define WRITE_QUEUE_HPP

#include <srcml.h>
#include <parse_request.hpp>
#include <ctpl_stl.h>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <deque>

struct cmp
{
    bool operator()(ParseRequest* r1, ParseRequest* r2) {
        return r1->position > r2->position;
    }
};

class WriteQueue {
public:

    WriteQueue(std::function<void(ParseRequest*)> writearg, bool ordered = true)
        : write(writearg), pool(1), counter(0), ordered(ordered) {}

    /* writes out the current srcml */
    inline void schedule(ParseRequest* pvalue) {

      	if (!ordered) {

       		pool.push(std::bind(write, pvalue));

       	} else {

       		std::unique_lock<std::mutex> lock(mutex);

   			// put this request into the queue
       		q.push(pvalue);

	       	// as long as there are parse requests in order, put them into the general pool
       		while (!q.empty()) {
       			ParseRequest* cur = q.top();
       			if (cur->position != counter + 1)
       				break;

      			pool.push(std::bind(write, cur));

       			q.pop();

       			++counter;
       		}
       	}
    }

    inline void eos(ParseRequest* pvalue) {
        write(pvalue);
    }

    inline void wait() {
    	pool.stop(true);
    }

private:
    std::function<void(ParseRequest*)> write;
    std::mutex mutex;
    ctpl::thread_pool pool;
	std::atomic<int> counter;
    bool ordered;
  	std::priority_queue<ParseRequest*, std::deque<ParseRequest*>, cmp > q;
};

#endif
