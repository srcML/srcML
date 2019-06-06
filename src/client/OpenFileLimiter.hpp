/**
 * @file OpenFileLimiter.hpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
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

#ifndef ARCHIVE_OPEN_HPP
#define ARCHIVE_OPEN_HPP

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

