/**
 * @file parse_queue.hpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 */

#ifndef PARSE_QUEUE_HPP
#define PARSE_QUEUE_HPP

#include <parse_request.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread.hpp>
#pragma GCC diagnostic pop
#include <string>

class ParseQueue {
public:
    static const int CAPACITY = 40;

    ParseQueue(int max_threads, boost::function<void()>);

    /* puts an element in the back of the queue by swapping with parameter */
    void push(ParseRequest& value);

    /* removes the front element from the queue by swapping with parameter */
    void pop(ParseRequest& value);

    void join();

private:
    boost::thread_group writers;
    size_t max_threads;
    boost::function<void()> consume;
    int counter;
    ParseRequest buffer[CAPACITY];
    int qsize;
    int back;
    int front;
    ParseRequest empty_request;
    bool empty;
    boost::mutex mutex;
    boost::condition_variable cond_full;
    boost::condition_variable cond_empty;
};

#endif
