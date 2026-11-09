// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TraceLog.cpp
 *
 * @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <TraceLog.hpp>
#include <srcml_cli.hpp>
#include <srcml_options.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdio>

#ifndef _MSC_VER
#include <unistd.h>
#include <sys/ioctl.h>
#endif

size_t TraceLog::loc = 0;

namespace {

    const char* const spinner[] = { "⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏" };
    const int spinner_size = sizeof(spinner) / sizeof(spinner[0]);

    const char* const checkmark = "\x1b[32m✓\x1b[0m";

    // synchronized output, so the terminal paints a redraw all at once
    const char* const sync_begin = "\x1b[?2026h";
    const char* const sync_end = "\x1b[?2026l";

    // parsing time before a file is considered stalled
    const auto stall = std::chrono::seconds(1);

    // end of the filename that fits in the width
    std::string fit(std::string_view filename, std::size_t width) {

        if (filename.size() <= width)
            return std::string(filename);

        if (width <= 3)
            return std::string(filename.substr(filename.size() - width));

        return "..." + std::string(filename.substr(filename.size() - (width - 3)));
    }

    void terminal_size(int& rows, int& cols) {

        rows = 24;
        cols = 80;
#ifndef _MSC_VER
        struct winsize ws;
        if (ioctl(fileno(stderr), TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0 && ws.ws_col > 0) {
            rows = ws.ws_row;
            cols = ws.ws_col;
        }
#endif
    }
}

TraceLog::TraceLog()
    : enabled(option(SRCML_COMMAND_VERBOSE)) {

#ifndef _MSC_VER
    terminal = enabled && isatty(fileno(stderr));
#endif
}

int TraceLog::fieldsWidth() const {

    int width = option(SRCML_DEBUG_MODE) ? 52 + 15 : 52;
    if (terminal) {
        width += (language_width - 4) + (loc_width - 5) - (40 - short_hash);
        if (option(SRCML_DEBUG_MODE))
            width += klocs_width - 5;
    }

    return width;
}

void TraceLog::output(const char* s) {

    if (!enabled)
        return;

    std::clog << s;
}

void TraceLog::start(int position, std::string_view filename) {

    if (!terminal)
        return;

    std::lock_guard<std::mutex> lock(mutex);

    active.emplace(position, Parsing{ std::string(filename), std::chrono::steady_clock::now() });

    // spinner animation runs until the log is done
    if (!progress) {
        progress = true;
        ticker = std::thread([this]() {
            std::unique_lock<std::mutex> lock(mutex);
            while (!stopping) {
                cv.wait_for(lock, std::chrono::milliseconds(80));
                ++frame;
                int lines = 0;
                std::string region = live(lines);
                if (region == shown)
                    continue;
                // single write, so the terminal never shows a partial redraw
                std::clog << (sync_begin + erase() + region + "\x1b[J" + sync_end) << std::flush;
                drawn = lines;
                shown = std::move(region);
            }
        });
    }
}

void TraceLog::finish(int position) {

    if (!terminal)
        return;

    std::lock_guard<std::mutex> lock(mutex);

    active.erase(position);
}

std::string TraceLog::erase() {

    if (drawn == 0)
        return "";

    // move up only, as the lines are overwritten in place
    std::string out = "\r\x1b[" + std::to_string(drawn) + "A";
    drawn = 0;

    return out;
}

std::string TraceLog::live(int& lines) {

    // header with the field names, in the same columns as the fields
    std::ostringstream outs;
    outs << "\x1b[2m  " << std::setw(5) << std::right << "#" << ' ';
    outs << std::setw(language_width) << std::left << "Language" << ' ';
    outs << std::setw(loc_width) << std::right << "LOC";
    if (option(SRCML_DEBUG_MODE)) {
        outs << ' ' << std::setw(8) << std::right << "Time";
        outs << ' ' << std::setw(klocs_width) << std::right << "KLOC/s";
    }
    outs << ' ' << std::setw(short_hash) << std::left << "Hash" << ' ' << "Filename\x1b[0m\x1b[K\n";
    std::string out = outs.str();
    lines = 1;

    if (active.empty())
        return out;

    int rows, cols;
    terminal_size(rows, cols);

    // limited to the terminal height and width so that cursor movement is correct
    int limit = rows > 4 ? rows - 3 : 1;
    // spinner in the column of the checkmark, filename in the same column as in the other lines
    const std::string indent(6 + fieldsWidth(), ' ');
    std::size_t used = 2 + indent.size() + 1;
    std::size_t width = static_cast<std::size_t>(cols) > used ? static_cast<std::size_t>(cols) - used : 1;

    auto now = std::chrono::steady_clock::now();
    int stalled = 0;
    for (const auto& [position, parsing] : active) {
        if (now - parsing.started < stall)
            continue;
        ++stalled;
        if (stalled > limit)
            continue;
        out += "\x1b[36m";
        out += spinner[frame % spinner_size];
        out += "\x1b[0m ";
        out += indent;
        out += fit(parsing.filename, width);
        out += "\x1b[K\n";
        ++lines;
    }
    if (stalled > limit) {
        out += "  ... " + std::to_string(stalled - limit) + " more\x1b[K\n";
        ++lines;
    }

    return out;
}

TraceLog& operator<<(TraceLog& tlog, char c) {

    if (!tlog.enabled)
        return tlog;

    std::ostringstream outs;
    if (c != '-') {
        outs << std::setw(5) << ++tlog.count << ' ';
    } else {
        outs << "    - ";
        ++tlog.num_skipped;
    }

    if (tlog.terminal) {
        tlog.prefix = c != '-' ? checkmark : " ";
        tlog.prefix += ' ';
        tlog.prefix += outs.str();
        return tlog;
    }

    std::clog << outs.str();

    return tlog;
}

TraceLog& operator<<(TraceLog& tlog, std::string_view s) {

    if (!tlog.enabled)
        return tlog;

    if (tlog.terminal) {
        std::lock_guard<std::mutex> lock(tlog.mutex);
        // line goes above the spinner lines
        std::string out = sync_begin + tlog.erase();
        out += tlog.prefix;
        out += s;
        out += "\x1b[K\n";
        int lines = 0;
        tlog.shown = tlog.live(lines);
        out += tlog.shown;
        out += "\x1b[J";
        out += sync_end;
        std::clog << out << std::flush;
        tlog.drawn = lines;
        tlog.prefix.clear();
        return tlog;
    }

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

    if (progress) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stopping = true;
            cv.notify_one();
        }
        ticker.join();

        std::clog << erase() << "\x1b[J";
    }

    int total = count + num_skipped + num_error;

    std::clog << "\nSource Files: " << count << "\tOther Files: " << num_skipped << "\tErrors: " << num_error << "\tTotal Files: " << total << "\n";
}
