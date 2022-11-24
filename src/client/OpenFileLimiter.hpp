// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file OpenFileLimiter.hpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef OPENFILELIMITER_HPP
#define OPENFILELIMITER_HPP

#include <mutex>
#include <condition_variable>

class OpenFileLimiter {
public:

    static const int MAX_OPEN_FILES = 200;

    static void open() {
        std::unique_lock<std::mutex> l(e);

        while (open_files > MAX_OPEN_FILES) {
            cv.wait(l);
        }

        ++open_files;
    }

    static void close() {
        std::unique_lock<std::mutex> l(e);

        --open_files;

        cv.notify_one();
    }

private:
    static std::mutex e;
    static std::condition_variable cv;
    static int open_files;
};

#endif
