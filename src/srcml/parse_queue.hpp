/*
  parse_queue.hpp

  Copyright (C) 2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
*/

#ifndef PARSE_QUEUE_HPP
#define PARSE_QUEUE_HPP

#include <srcml.h>
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread.hpp>
#pragma GCC diagnostic warning "-Wshorten-64-to-32"
#include <parse_request.hpp>
#include <thread_queue.hpp>
#include <srcml_consume.hpp>
#include <string>

typedef ThreadQueue<ParseRequest, 10> ParseQueue_Type;

class ParseQueue {
public:
    ParseQueue(int max_threads) : max_threads(max_threads) {}

    /* puts an element in the back of the queue by swapping with parameter */
    void push(ParseRequest& value) {

        // create threads as requests are pushed
        // no more then max threads however
        if (writers.size() < max_threads)
            writers.create_thread( boost::bind(srcml_consume, &queue) );

        queue.push(value);
    }

    /* removes the front element from the queue by swapping with parameter */
    void pop(ParseRequest& value) {
        queue.pop(value);
    }

    void done() {
        queue.done();
        writers.join_all();
    }

    int size() {
        return queue.size();
    }

private:
    ThreadQueue<ParseRequest, 10> queue;
    boost::thread_group writers;
    size_t max_threads;
};

#endif
