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
#include <SRCMLStatus.hpp>
#include <filesystem>

void src_output_filesystem(srcml_archive* srcml_arch, std::string_view output_dir, TraceLog& log) {

    // construct the relative directory
    std::string prefix;
    if (output_dir != "." && output_dir != "./")
        prefix = output_dir;

    // read and output each unit
    int count = 0;
    while (std::unique_ptr<srcml_unit> unit{srcml_archive_read_unit(srcml_arch)}) {

        // extract the filename from the unit
        const char* cfilename = srcml_unit_get_filename(unit.get());

        // skip if no filename
        if (!cfilename)
            continue;

        // construct the full, relative filename
        std::filesystem::path path(prefix);
        path /= cfilename;

        // create the file path
        auto directory = path;
        directory.remove_filename();
        std::error_code ec;
        if (!std::filesystem::create_directories(directory, ec) && ec.value() != 0) {
            SRCMLstatus(ERROR_MSG, "Unable to create directory %s", directory.c_str());
            continue;
        }

        // unparse directly to the file
        // log before so error is on last shown filename
        log << ++count << path.string().c_str();
        int result = srcml_unit_unparse_filename(unit.get(), path.string().c_str());
        if (result != SRCML_STATUS_OK) {
           SRCMLstatus(ERROR_MSG, "Unable to extract file %s", path.string().c_str());
        }
    }
}
