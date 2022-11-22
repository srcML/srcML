// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file WriteQueue.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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
