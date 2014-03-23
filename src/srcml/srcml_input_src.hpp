/**
 * @file srcml_input_src.hpp
 *
 * @copyright @copyright Copyright (C) 2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
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
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SRCML_INPUT_SRC_HPP
#define SRCML_INPUT_SRC_HPP

#include <srcml.h>
#include <string>
#include <vector>
#include <boost/optional.hpp>
#include <iostream>

class srcml_input_src {
public:
    srcml_input_src& operator=(const std::string& other) { filename = other; return *this; }
    srcml_input_src& operator=(FILE* other) { fileptr = other; return *this; }
    srcml_input_src& operator=(int other) { fd = other; return *this; }

    void isxml(bool result) { is_xml = result; }
    bool isxml() const { return is_xml && *is_xml; }
    bool issrc() const { return is_xml && !*is_xml; }

    operator std::string() { return !filename ? *filename : ""; }

    bool has_name() const { return filename; }
    bool has_fileptr() const { return fileptr; }
    bool has_fd() const { return fd; }

    const std::string& get_filename() const { return *filename; }
    FILE* get_fileptr() const { return *fileptr; }
    int get_fd() const { return *fd; }

private:
    boost::optional<std::string> filename;
    boost::optional<FILE*> fileptr;
    boost::optional<int> fd;
    boost::optional<bool> is_xml;
};

#endif
