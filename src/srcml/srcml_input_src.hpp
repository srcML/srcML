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
#include <src_prefix.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>

class srcml_input_src;

typedef std::vector<srcml_input_src> srcml_input_t;
typedef srcml_input_src srcml_output_src;

class srcml_input_src {
public:
    srcml_input_src() {}
    srcml_input_src(const std::string& other) { 

        filename = src_prefix_add_uri(other);

        src_prefix_split_uri(filename, protocol, resource);
        extension = boost::filesystem::path(resource.c_str()).extension().string();
        isxml(extension == ".xml");
    }

    srcml_input_src& operator=(const std::string& other) { srcml_input_src t(other); swap(t); return *this; }
    srcml_input_src& operator=(FILE* other) { fileptr = other; return *this; }
    srcml_input_src& operator=(int other) { fd = other; return *this; }

    operator const std::string&() const { return filename; }
    operator FILE*() const { return *fileptr; }
    operator int() const { return *fd; }

    bool operator==(const std::string& other) const { return other == filename; }
    bool operator!=(const char* other) const { return filename != other; }

    void isxml(bool result) { is_xml = result; }
    bool isxml() const { return is_xml && *is_xml; }
    bool issrc() const { return is_xml && !*is_xml; }

    const char* c_str() const { return filename.c_str(); }

    void swap(srcml_input_src& other) {

        std::swap(filename, other.filename);
        std::swap(resource, other.resource);
        std::swap(extension, other.extension);
        std::swap(protocol, other.protocol);
        std::swap(fileptr, other.fileptr);
        std::swap(fd, other.fd);
        std::swap(is_xml, other.is_xml);
    }

    std::string filename;
    std::string resource;
    std::string extension;
    std::string protocol;
    boost::optional<FILE*> fileptr;
    boost::optional<int> fd;
    boost::optional<bool> is_xml;
};

template <typename T>
inline bool contains(const srcml_input_src&) { return false; }

template <>
inline bool contains<FILE*>(const srcml_input_src& input) { return input.fileptr; }

template <>
inline bool contains<int>(const srcml_input_src& input) { return input.fd; }

#endif
