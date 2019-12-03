/**
 * @file WriteQueue.hpp
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

#ifndef WRITE_QUEUE_HPP
#define WRITE_QUEUE_HPP

#include <srcml.h>
#include <ParseRequest.hpp>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <deque>
#include <thread>
#include <TraceLog.hpp>
#include <srcml_input_src.hpp>

class WriteQueue {

public:
    WriteQueue(TraceLog& log, const srcml_output_dest& destination, bool ordered = true);

    // writes out the current srcml
    void schedule(std::shared_ptr<ParseRequest> pvalue);

    // stop the write process, allowing it to finish up
    void stop();

    // actual process
    void process();

    // number of units writtent
    int numWritten() const { return total; }

public:
    TraceLog& log;
    const srcml_output_dest& destination;
    bool ordered;
    std::thread write_thread;
    int maxposition;
    std::priority_queue<std::shared_ptr<ParseRequest>, std::deque<std::shared_ptr<ParseRequest>>, bool (*)(std::shared_ptr<ParseRequest>, std::shared_ptr<ParseRequest>)> q;
    std::mutex qmutex;
    std::condition_variable cv;
    int total = 0;
    bool completed = false;
};

#endif
