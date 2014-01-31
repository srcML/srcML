/*
  c-pthread-queue - c implementation of a bounded buffer queue using posix threads
  Copyright (C) 2008  Matthew Dickinson

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/thread.hpp>

#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

template <typename Type, int Capacity>
class ThreadQueue {
public:
    ThreadQueue() :
        used(0), back_index(0), front_index(0) {}

    void push(Type& value) {
        {
            boost::unique_lock<boost::mutex> lock(mutexb);
            while (used == Capacity)
                cond_fullb.wait(lock);

            buffer[back_index].swap(value);
            ++back_index;
            back_index %= Capacity;
            ++used;
        }
        cond_emptyb.notify_all();
    }

    void pop(Type& place) {
        {
            boost::unique_lock<boost::mutex> lock(mutexb);
            while (used == 0)
                cond_emptyb.wait(lock);
            place.swap(buffer[front_index]);
            ++front_index;
            front_index %= Capacity;
            --used;
        }
        cond_fullb.notify_all();
    }

    int size() {
        int tsize;
        {
            boost::unique_lock<boost::mutex> lock(mutexb);
            tsize = used;
        }
        return tsize;
    }

    ~ThreadQueue() {}

private:
    Type buffer[Capacity];
    int used;
    int back_index;
    int front_index;
    boost::mutex mutexb;
    boost::condition_variable cond_fullb;
    boost::condition_variable cond_emptyb;
};

#endif
