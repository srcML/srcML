/**
 * @file src_output_filesystem.cpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcml command-line client; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <src_output_filesystem.hpp>
#include <srcml.h>
#include <iostream>
#include <boost/filesystem.hpp>

void src_output_filesystem(srcml_archive* srcml_arch, const std::string& output_dir, TraceLog& log) {

    // construct the relative directory
    std::string prefix;
    if (output_dir != "." && output_dir != "./")
        prefix = output_dir;

    int count = 0;
    while (srcml_unit* unit = srcml_archive_read_unit(srcml_arch)) {

        // construct the relative directory
        boost::filesystem::path out(prefix);
        if (const char* filename = srcml_unit_get_filename(unit))
            out /= filename;

        // create the path
        if (out.has_parent_path() && !is_directory(out.parent_path()))
            boost::filesystem::create_directories(out.parent_path());

        // unparse directory to filename
        log << ++count << out.string();

        srcml_unit_unparse_filename(unit, (const char *)out.c_str());

        srcml_unit_free(unit);
    }
}
