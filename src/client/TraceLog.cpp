/**
 * @file TraceLog.cpp
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

#include <TraceLog.hpp>
#include <srcml_cli.hpp>
#include <srcml_options.hpp>
#include <iomanip>
#include <iostream>

size_t TraceLog::loc = 0;

TraceLog::TraceLog()
    : enabled(option(SRCML_COMMAND_VERBOSE)) {
}

void TraceLog::output(const char* s) {

    if (!enabled)
        return;

    std::clog << s;
}

TraceLog& operator<<(TraceLog& tlog, char c) {

    if (!tlog.enabled)
        return tlog;

    if (c != '-') {
        std::clog << std::setw(5) << ++tlog.count << ' ';
    } else {
        std::clog << "    - ";
        ++tlog.num_skipped;
    }

    return tlog;
}

TraceLog& operator<<(TraceLog& tlog, const std::string& s) {

    if (!tlog.enabled)
        return tlog;

    std::clog << s << '\n';

    return tlog;
}

TraceLog::~TraceLog() {

    if (!enabled)
        return;

    int total = count + num_skipped + num_error;

    std::clog << "\nSource Files: " << count << "\tOther Files: " << num_skipped << "\tErrors: " << num_error << "\tTotal Files: " << total << "\n";
}

