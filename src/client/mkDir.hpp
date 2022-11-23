// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file mkdir.hpp
 *
 * @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef INCLUDED_MKDIR_HPP
#define INCLUDED_MKDIR_HPP

#include <archive.h>
#include <archive_entry.h>
#include <string_view>

class mkDir {
public:
    mkDir();

    void mkdir(std::string_view path);

    ~mkDir();
private:
    archive* arch;
    archive_entry* entry;
    std::string_view last;
};

#endif
