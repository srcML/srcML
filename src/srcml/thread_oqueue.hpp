/**
 * @file thread_oqueue.hpp
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

#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread.hpp>
#pragma GCC diagnostic warning "-Wshorten-64-to-32"

#ifndef THREAD_OQUEUE_H
#define THREAD_OQUEUE_H

#include <vector>
#include <algorithm>
#include <write_request.hpp>

const int Capacity = 100;

class ThreadOQueue {
public:
    ThreadOQueue() : empty(false), curpos(1) {

        queue.reserve(Capacity);
    }

    /* puts an element in the back of the queue by swapping with parameter */
    void push(WriteRequest& value) {
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            while (queue.size() == (Capacity - 1) || value.position != curpos)
                cond_full.wait(lock);

            /* Need to avoid a copy of the write request */

            // get a temporary element with the same position as the one to be inserted into the queue
            WriteRequest t;
            t.position = value.position;
            queue.push_back(t);

            // swap the value to be inserted with the temporary
            queue.back().swap(value);

            // push the new value into the heap properly
            std::push_heap(queue.begin(), queue.end());
        }
        cond_empty.notify_one();
    }

    /* removes the front element from the queue by swapping with parameter */
    void pop(WriteRequest& value) {
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            while (!empty && (queue.empty() || (queue.front().position != curpos && queue.front().position != 0)))
                cond_empty.wait(lock);

            if (queue.size() == 0 && empty) {
                WriteRequest t;
                value.swap(t);
            } else {
                value.swap(queue.front());
                std::pop_heap(queue.begin(), queue.end());
                queue.pop_back();
            }
            ++curpos;
        }
        cond_full.notify_all();
    }

    void done() {
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            empty = true;
        }
        cond_empty.notify_all();
        cond_full.notify_all();
    }

    size_t size() {
        return queue.size();
    }

    ~ThreadOQueue() {}

private:
    std::vector<WriteRequest> queue;
    bool empty;
    boost::mutex mutex;
    boost::condition_variable cond_full;
    boost::condition_variable cond_empty;
    int curpos;
};

#endif
