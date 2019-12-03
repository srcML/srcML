/**
 * @file ParseQueue.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef PARSE_QUEUE_HPP
#define PARSE_QUEUE_HPP

#include <ParseRequest.hpp>
#include <WriteQueue.hpp>
#include <ctpl_stl.h>
#include <mutex>
#include <srcml_consume.hpp>
#include <memory>
#include <srcml_utilities.hpp>

class ParseQueue {
public:

    ParseQueue(int max_threads, WriteQueue* write_queue)
        : pool(max_threads), wqueue(write_queue) {}

    inline void schedule(std::shared_ptr<ParseRequest> pvalue) {

        int next;
        {
            std::unique_lock<std::mutex> l(e);

            next = ++counter;
        }
        pvalue->position = next;

        // error passthrough to output for proper output in trace
        if (pvalue->status) {
            pvalue->unit = 0;
            wqueue->schedule(pvalue);
            return;
        }

        pool.push(srcml_consume, pvalue, wqueue);
    }

    inline void wait() {

        pool.stop(true);
    }

private:
    ctpl::thread_pool pool;
    WriteQueue* wqueue;
    int counter = 0;
    std::mutex e;
};

#endif
