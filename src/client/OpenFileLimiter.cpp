// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file OpenFileLimiter.hpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <OpenFileLimiter.hpp>

int OpenFileLimiter::open_files = 0;
std::mutex OpenFileLimiter::e;
std::condition_variable OpenFileLimiter::cv;
