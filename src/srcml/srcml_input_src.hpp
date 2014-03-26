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
#include <boost/logic/tribool.hpp>
#include <algorithm>

class srcml_input_src;

typedef std::vector<srcml_input_src> srcml_input_t;
typedef srcml_input_src srcml_output_dest;

enum STATES { INDETERMINATE, SRC, SRCML };

class srcml_input_src {
public:

    srcml_input_src() {}
    srcml_input_src(const std::string& other) : state(INDETERMINATE) { 

        filename = other;

        src_prefix_split_uri(filename, protocol, resource);
        extension = boost::filesystem::path(resource.c_str()).extension().string();
        if (resource != "-")
            state = extension == ".xml" ? SRCML : SRC;
        if (protocol == "stdin")
            fd = 0;
        if (protocol == "stdout")
            fd = 1;
    }

    srcml_input_src(const std::string& other, int fds) {

        srcml_input_src s(other);
        s = fds;

        swap(s);
    }

    srcml_input_src(int fds) {

        srcml_input_src s("-");
        s = fds;

        swap(s);
    }

    srcml_input_src& operator=(const std::string& other) { srcml_input_src t(other); swap(t); return *this; }
    srcml_input_src& operator=(FILE* other) { fileptr = other; return *this; }
    srcml_input_src& operator=(int other) { fd = other; return *this; }

    operator const std::string&() const { return resource; }
    operator FILE*() const { return *fileptr; }
    operator int() const { return *fd; }

    bool operator==(const std::string& other) const { return other == filename; }
    bool operator!=(const char* other) const { return filename != other; }

    const char* c_str() const { return resource.c_str(); }

    void swap(srcml_input_src& other) {

        std::swap(filename, other.filename);
        std::swap(resource, other.resource);
        std::swap(extension, other.extension);
        std::swap(protocol, other.protocol);
        std::swap(fileptr, other.fileptr);
        std::swap(fd, other.fd);
        std::swap(state, other.state);
    }

    std::string filename;
    std::string protocol;
    std::string resource;
    std::string extension;
    boost::optional<FILE*> fileptr;
    boost::optional<int> fd;
    enum STATES state;
};

template <typename T>
inline bool contains(const srcml_input_src&) { return false; }

template <>
inline bool contains<FILE*>(const srcml_input_src& input) { return input.fileptr; }

template <>
inline bool contains<int>(const srcml_input_src& input) { return input.fd; }

inline bool is_src(const srcml_input_src& input) { return input.state == SRC; }

#endif
