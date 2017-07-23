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
#include <ctpl_stl.h>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include <deque>
#include <thread>

class WriteQueue {

public:
    WriteQueue(std::function<void(ParseRequest*)> writearg, bool ordered = true);

    // writes out the current srcml
    void schedule(ParseRequest* pvalue);

    // end of stream
    void eos(ParseRequest* pvalue);

    // start the write proces
    void start();

    // stop the write process, allowing it to continue
    void stop();

    // actual process
    void process();

public:
    std::function<void(ParseRequest*)> write;
    bool ordered;
    std::thread write_thread;
    int maxposition;
    std::priority_queue<ParseRequest*, std::deque<ParseRequest*>, std::function<bool(ParseRequest*, ParseRequest*)>> q;
    std::mutex qmutex;
    std::condition_variable cv;
};

#endif
