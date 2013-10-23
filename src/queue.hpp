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

#ifndef _QUEUE_H
#define _QUEUE_H

template <typename Type, int Capacity>
class queue {

  queue() :
    buffer(0), capacity(0), qsize(0), in(0), out(0) {

    mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    cond_full = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    cond_empty = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
  }


    Type buffer[Capacity];
    int qsize;
    int in;
    int out;
    pthread_mutex_t mutex;
    pthread_cond_t cond_full;
    pthread_cond_t cond_empty;


void push(Type& value) {
    pthread_mutex_lock(&mutex);
    while (qsize == Capacity)
        pthread_cond_wait(&cond_full, &mutex);
    buffer[in].swap(value);
    ++qsize;
    ++in;
    in %= Capacity;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_empty);
}

void pop() {
    pthread_mutex_lock(&mutex);
    while (qsize == 0)
        pthread_cond_wait(&cond_empty, &mutex);
    --qsize;
    ++out;
    out %= Capacity;
    pthread_mutex_unlock(&mutex);
    pthread_cond_signal(&cond_full);
}

const Type& front() {
    pthread_mutex_lock(&mutex);
    while (qsize == 0)
        pthread_cond_wait(&cond_empty, &mutex);
    Type* pvalue = &(buffer[out]);
    pthread_mutex_unlock(&mutex);
    pthread_cond_broadcast(&cond_full);
    return *pvalue;
}

int size() {
    pthread_mutex_lock(&mutex);
    int tsize = qsize;
    pthread_mutex_unlock(&mutex);
    return tsize;
}

};

#endif
