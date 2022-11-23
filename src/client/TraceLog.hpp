// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TraceLog.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef INCLUDED_TRACE_LOG_HPP
#define INCLUDED_TRACE_LOG_HPP

#include <iostream>
#include <string>
#include <string_view>

class TraceLog {
public:
    TraceLog();

    friend TraceLog& operator<<(TraceLog& tlog, char c);
    friend TraceLog& operator<<(TraceLog& tlog, int n);
    friend TraceLog& operator<<(TraceLog& tlog, std::string_view s);

    void output(const char* s);

    inline void totalLOC(size_t floc) {
        loc += floc;
    }

    inline static size_t totalLOC() {
        return loc;
    }

    inline void skip() {
        ++num_skipped;
    }

    ~TraceLog();

private:
    bool enabled;
    int count = 0;
    int num_skipped = 0;
    int num_error = 0;
    static size_t loc;
};

#endif
