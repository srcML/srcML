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

#include <pthread.h>
#include <stdio.h>

#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H

template <typename Type, int Capacity>
class ThreadQueue {
public:
    ThreadQueue() :
        used(0), back_index(0), front_index(0),
        mutex(    (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER),
        cond_full( (pthread_cond_t)PTHREAD_COND_INITIALIZER),
        cond_empty((pthread_cond_t)PTHREAD_COND_INITIALIZER)
        {}

    void push(Type& value) {
        pthread_mutex_lock(&mutex);
        while (used == Capacity)
            pthread_cond_wait(&cond_full, &mutex);
        buffer[back_index].swap(value);
        ++back_index;
        back_index %= Capacity;
        ++used;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_empty);
    }

    void pop() {
        pthread_mutex_lock(&mutex);
        while (used == 0)
            pthread_cond_wait(&cond_empty, &mutex);
        ++front_index;
        front_index %= Capacity;
        --used;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_full);
    }

    void pop(Type& place) {
        pthread_mutex_lock(&mutex);
        while (used == 0)
            pthread_cond_wait(&cond_empty, &mutex);
        place.swap(buffer[front_index]);
        ++front_index;
        front_index %= Capacity;
        --used;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_full);
    }

    const Type& front() {
        pthread_mutex_lock(&mutex);
        while (used == 0)
            pthread_cond_wait(&cond_empty, &mutex);
        Type* pvalue = &(buffer[front_index]);
        pthread_mutex_unlock(&mutex);
        pthread_cond_broadcast(&cond_full);
        return *pvalue;
    }

    int size() {
        pthread_mutex_lock(&mutex);
        int tsize = used;
        pthread_mutex_unlock(&mutex);
        return tsize;
    }

    ~ThreadQueue() {}

private:
    Type buffer[Capacity];
    int used;
    int back_index;
    int front_index;
    pthread_mutex_t mutex;
    pthread_cond_t cond_full;
    pthread_cond_t cond_empty;
};

#endif
