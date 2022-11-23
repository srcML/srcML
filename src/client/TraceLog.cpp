// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TraceLog.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
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

TraceLog& operator<<(TraceLog& tlog, std::string_view s) {

    if (!tlog.enabled)
        return tlog;

    std::clog << s << '\n';

    return tlog;
}

TraceLog& operator<<(TraceLog& tlog, int n) {

    if (!tlog.enabled)
        return tlog;

    std::clog << n << '\n';

    return tlog;
}

TraceLog::~TraceLog() {

    if (!enabled)
        return;

    int total = count + num_skipped + num_error;

    std::clog << "\nSource Files: " << count << "\tOther Files: " << num_skipped << "\tErrors: " << num_error << "\tTotal Files: " << total << "\n";
}

