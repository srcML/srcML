// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TraceLog.hpp
 *
 * @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef INCLUDED_TRACE_LOG_HPP
#define INCLUDED_TRACE_LOG_HPP

#include <iostream>
#include <string>
#include <string_view>
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>

class TraceLog {
public:
    TraceLog();

    friend TraceLog& operator<<(TraceLog& tlog, char c);
    friend TraceLog& operator<<(TraceLog& tlog, int n);
    friend TraceLog& operator<<(TraceLog& tlog, std::string_view s);

    void output(const char* s);

    // file at position is being processed
    void start(int position, std::string_view filename);

    // file at position is done processing
    void finish(int position);

    inline void totalLOC(size_t floc) {
        loc += floc;
    }

    inline static size_t totalLOC() {
        return loc;
    }

    // output to a terminal, with a shortened hash
    inline bool isTerminal() const {
        return terminal;
    }

    // length of the shortened hash
    static constexpr int short_hash = 8;

    // width of the language field, for the longest language name, Objective-C
    static constexpr int language_width = 11;

    // width of the LOC field, for files of 100K lines or more
    static constexpr int loc_width = 6;

    // width of the KLOC/s field, for its header
    static constexpr int klocs_width = 6;

    // width of the fields between the count and the filename
    int fieldsWidth() const;

    inline void skip() {
        ++num_skipped;
    }

    ~TraceLog();

private:
    // erase the spinner lines, with the mutex held
    std::string erase();

    // header line and spinner lines for stalled files, with the mutex held
    std::string live(int& lines);

    bool enabled;
    int count = 0;
    int num_skipped = 0;
    int num_error = 0;
    static size_t loc;

    // spinner display of stalled files, for a terminal
    bool terminal = false;
    bool progress = false;
    std::string prefix;
    struct Parsing {
        std::string filename;
        std::chrono::steady_clock::time_point started;
    };
    std::map<int, Parsing> active;
    int drawn = 0;
    std::string shown;
    int frame = 0;
    bool stopping = false;
    std::mutex mutex;
    std::condition_variable cv;
    std::thread ticker;
};

#endif
