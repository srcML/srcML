// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file PerfCounters.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <PerfCounters.hpp>
#include <cstdio>

#if defined(__APPLE__)
#include <libproc.h>
#include <unistd.h>
#elif defined(__linux__) && __has_include(<linux/perf_event.h>)
#define SRCML_PERF_EVENT
#include <linux/perf_event.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

#ifdef SRCML_PERF_EVENT
namespace {
    // user-space count for this process, inherited by new threads
    int open_counter(std::uint64_t config) {
        perf_event_attr attr{};
        attr.type = PERF_TYPE_HARDWARE;
        attr.size = sizeof(attr);
        attr.config = config;
        attr.exclude_kernel = 1;
        attr.exclude_hv = 1;
        attr.inherit = 1;
        return (int) syscall(SYS_perf_event_open, &attr, 0, -1, -1, 0);
    }
}
#endif

PerfCounters::PerfCounters(bool enabled) {
    #ifdef SRCML_PERF_EVENT
        if (!enabled)
            return;
        instructions_fd = open_counter(PERF_COUNT_HW_INSTRUCTIONS);
        cycles_fd = open_counter(PERF_COUNT_HW_CPU_CYCLES);
    #else
        (void) enabled;
    #endif
}

PerfCounters::~PerfCounters() {
    #ifdef SRCML_PERF_EVENT
        if (instructions_fd != -1)
            close(instructions_fd);
        if (cycles_fd != -1)
            close(cycles_fd);
    #endif
}

bool PerfCounters::read(std::uint64_t& instructions, std::uint64_t& cycles) {
    #if defined(__APPLE__)
        rusage_info_v4 info;
        if (proc_pid_rusage(getpid(), RUSAGE_INFO_V4, (rusage_info_t*) &info) != 0)
            return false;
        instructions = info.ri_instructions;
        cycles = info.ri_cycles;
        return instructions != 0 && cycles != 0;
    #elif defined(SRCML_PERF_EVENT)
        if (instructions_fd == -1 || cycles_fd == -1)
            return false;
        if (::read(instructions_fd, &instructions, sizeof(instructions)) != sizeof(instructions))
            return false;
        if (::read(cycles_fd, &cycles, sizeof(cycles)) != sizeof(cycles))
            return false;
        return true;
    #else
        (void) instructions;
        (void) cycles;
        return false;
    #endif
}

std::string PerfCounters::scaled(std::uint64_t count) {
    if (count < 1000)
        return std::to_string(count);

    // next unit when the value would round to 1000.0 or more
    const char* units[] = { "K", "M", "B" };
    double value = count / 1e3;
    int unit = 0;
    while (unit < 2 && value >= 999.95) {
        value /= 1e3;
        ++unit;
    }
    int decimals = value < 10 ? 3 : value < 100 ? 2 : 1;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.*f%s", decimals, value, units[unit]);
    return buffer;
}
