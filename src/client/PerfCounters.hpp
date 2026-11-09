// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file PerfCounters.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * Hardware counters (instructions retired and cycles) for the whole process,
 * including all threads. Available on macOS and Linux.
 */

#ifndef PERFCOUNTERS_HPP
#define PERFCOUNTERS_HPP

#include <cstdint>
#include <string>

class PerfCounters {
public:
    // On Linux, counting starts here and includes threads created afterwards
    PerfCounters(bool enabled);

    ~PerfCounters();

    PerfCounters(const PerfCounters&) = delete;
    PerfCounters& operator=(const PerfCounters&) = delete;

    // false if the counters are not available
    bool read(std::uint64_t& instructions, std::uint64_t& cycles);

    // count with K, M, or B units and 4 significant digits, e.g., 86.65M, 2.576B, 3443.0B
    static std::string scaled(std::uint64_t count);

private:
    // Linux perf_event file descriptors
    [[maybe_unused]] int instructions_fd = -1;
    [[maybe_unused]] int cycles_fd = -1;
};

#endif
