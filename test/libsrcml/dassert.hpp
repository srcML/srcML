// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file dassert.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_DASSERT_HPP
#define INCLUDED_DASSERT_HPP

#include <iostream>
#include <assert.h>

// difference assertion
#define dassert(LEFT, RIGHT) if(!((LEFT) == (RIGHT))) { \
        std::cerr << "'" << LEFT << "'" << "\n";        \
        std::cerr << "'" << RIGHT << "'" << "\n";       \
        std::cerr << argv[0] << ": " __FILE__ << ":" << __LINE__ << ": " << __FUNCTION__ << " Check '" << #LEFT << "' == '" << #RIGHT << "' failed.\n"; \
        exit(1); } else { std::cerr << __LINE__ << '\n'; }

#endif
