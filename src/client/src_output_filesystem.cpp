// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file src_output_filesystem.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <src_output_filesystem.hpp>
#include <srcml.h>
#include <iostream>
#include <srcml_utilities.hpp>
#include <mkDir.hpp>

void src_output_filesystem(srcml_archive* srcml_arch, const std::string& output_dir, TraceLog& log) {

    // construct the relative directory
    std::string prefix;
    if (output_dir != "." && output_dir != "./")
        prefix = output_dir;

    // create output directory structure as needed
    mkDir dir;

    int count = 0;
    std::string last;
    while (std::unique_ptr<srcml_unit> unit{srcml_archive_read_unit(srcml_arch)}) {

        const char* cfilename = srcml_unit_get_filename(unit.get());
        if (!cfilename)
            continue;

        std::string filename = cfilename;

        // separate pathname from filename
        std::string path = prefix;
        path += '/';
        auto pos = filename.rfind('/');
        path += pos != std::string::npos ? filename.substr(0, pos) : "";

        std::string fullfilename = prefix;
        fullfilename += "/";
        fullfilename += filename;

        // use libarchive to create the file path
        dir.mkdir(path);

        // unparse directory to filename
        log << ++count << fullfilename;

        srcml_unit_unparse_filename(unit.get(), fullfilename.data());
    }
}
