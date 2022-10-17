// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file CPUCount.cpp
 *
 * @copyright Copyright (C) 2021 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <CPUCount.hpp>

#if _WIN64
    #include <windows.h>
#elif __APPLE__
    #include <sys/types.h>
    #include <sys/sysctl.h>
#else
    #include <sched.h>
#endif

// Number of CPU cores
int CPUCount() {
#ifdef _WIN64
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return (int) sysinfo.dwNumberOfProcessors;
#elif __APPLE__
    int count = 0;
    size_t len = sizeof(count);
    sysctlbyname("machdep.cpu.core_count", &count, &len, NULL, 0);
    return count;
#else
    cpu_set_t cs;
    CPU_ZERO(&cs);
    sched_getaffinity(0, sizeof(cs), &cs);
    return CPU_COUNT(&cs);
#endif
}
