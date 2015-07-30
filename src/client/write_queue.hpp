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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <threadpool.hpp>
#pragma GCC diagnostic pop
#include <boost/function.hpp>
#include <prio_scheduler_strict.hpp>

class WriteQueue {
public:

    WriteQueue(boost::function<void(ParseRequest*)> writearg, bool order)
        : write(writearg), pool(1) {

        set_ordering(order);
    }

    /* puts an element in the back of the queue by swapping with parameter */
    inline void schedule(ParseRequest* pvalue) {

        pool.schedule(prio_strict_task_func(pvalue->position, boost::bind(write, pvalue)));
    }

    inline void wait() {

        pool.wait();
    }

private:
    boost::function<void(ParseRequest*)> write;
    prio_strict_pool pool;
};

#endif
