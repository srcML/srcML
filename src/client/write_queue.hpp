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
#include <mutex>
#include <condition_variable>
#include <functional>

class WriteQueue {
public:

    WriteQueue(std::function<void(ParseRequest*)> writearg, bool ordered = true)
        : write(writearg), counter(0), ordered(ordered) {
    }

    /* writes out the current srcml */
    inline void schedule(ParseRequest* pvalue) {
        std::unique_lock<std::mutex> lock(this->mutex);

        if (ordered) {
            while (pvalue->position != counter + 1)
                cv.wait(lock);

            ++counter;
        }
        
        write(pvalue);

        if (ordered) {
            lock.unlock();
            cv.notify_all();
        }
    }

    inline void eos(ParseRequest* pvalue) {
//        pvalue->status = 1000;
        write(pvalue);
    }

    inline void wait() {
        std::unique_lock<std::mutex> lock(this->mutex);
    }


private:
    std::function<void(ParseRequest*)> write;
    std::mutex mutex;
    std::condition_variable cv;
    int counter;
    bool ordered;
};

#endif
