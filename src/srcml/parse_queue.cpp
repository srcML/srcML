/**
 * @file parse_queue.cpp
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

#include <parse_queue.hpp>

ParseQueue::ParseQueue(int max_threads, boost::function<void()> consumearg)
    : max_threads(max_threads), consume(consumearg), counter(0), qsize(0), back(0), front(0), empty(false) {}

/* puts an element in the back of the queue by swapping with parameter */
void ParseQueue::push(ParseRequest& value) {

    // create threads as requests are pushed
    if (writers.size() < max_threads)
        writers.create_thread( consume );

    {
        boost::unique_lock<boost::mutex> lock(mutex);
        while (qsize == CAPACITY)
            cond_full.wait(lock);

        buffer[back].swap(value);
        buffer[back].position = ++counter;
        ++back %= CAPACITY;
        ++qsize;
    }
    cond_empty.notify_one();
}

/* removes the front element from the queue by swapping with parameter */
void ParseQueue::pop(ParseRequest& value) {
    {
        boost::unique_lock<boost::mutex> lock(mutex);
        while (qsize == 0 && !empty)
            cond_empty.wait(lock);

        if (qsize == 0 && empty)
            value.swap(empty_request);

        else {
            value.swap(buffer[front]);
            ++front %= CAPACITY;
            --qsize;
        }
    }
    cond_full.notify_one();
}

void ParseQueue::join() {
    {
        boost::unique_lock<boost::mutex> lock(mutex);
        empty = true;
    }
    cond_empty.notify_all();

    writers.join_all();
}
