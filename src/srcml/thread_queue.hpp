/*
  srcml.cpp

  Copyright (C) 2013  SDML (www.srcML.org)

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

#include <boost/thread.hpp>

#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

template <typename Type, int Capacity>
class ThreadQueue {
public:
    ThreadQueue() : qsize(0), back(0), front(0) {}

    void push(Type& value) {
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            while (qsize == Capacity)
                cond_full.wait(lock);

            buffer[back].swap(value);
            ++back %= Capacity;
            ++qsize;
        }
        cond_empty.notify_all();
    }

    void pop(Type& place) {
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            while (qsize == 0)
                cond_empty.wait(lock);

            place.swap(buffer[front]);
            ++front %= Capacity;
            --qsize;
        }
        cond_full.notify_all();
    }

    int size() {
        int size;
        {
            boost::unique_lock<boost::mutex> lock(mutex);
            size = qsize;
        }
        return size;
    }

    ~ThreadQueue() {}

private:
    Type buffer[Capacity];
    int qsize;
    int back;
    int front;
    boost::mutex mutex;
    boost::condition_variable cond_full;
    boost::condition_variable cond_empty;
};

#endif
