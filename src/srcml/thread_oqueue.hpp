/*
  srcml.cpp

  Copyright (C) 2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
n  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/thread.hpp>
#pragma GCC diagnostic warning "-Wshorten-64-to-32"

#ifndef THREAD_OQUEUE_H
#define THREAD_OQUEUE_H

#include <queue>

template <typename Type, int Capacity>
class ThreadOQueue {
public:
    ThreadOQueue() : qsize(0), back(0), front(0), empty(false) {}

    /* puts an element in the back of the queue by swapping with parameter */
    void push(Type& value) {
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            while (qsize == Capacity)
                cond_full.wait(lock);

            buffer[back].swap(value);
            ++back %= Capacity;
            ++qsize;
        }
        cond_empty.notify_one();
    }

    /* removes the front element from the queue by swapping with parameter */
    void pop(Type& value) {
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            while (qsize == 0 && !empty)
                cond_empty.wait(lock);
            
            if (qsize == 0 && empty) {
                Type t;
                value.swap(empty_request);
            } else {
                value.swap(buffer[front]);
                ++front %= Capacity;
                --qsize;
            }
        }
        cond_full.notify_one();
    }

    void done() {
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            empty = true;
        }
        cond_empty.notify_one();
    }

    int size() {
        return qsize;
    }

    ~ThreadOQueue() {}

private:
    Type buffer[Capacity];
    int qsize;
    int back;
    int front;
    Type empty_request;
    bool empty;
    boost::mutex mutex;
    boost::condition_variable cond_full;
    boost::condition_variable cond_empty;
};

#endif
