/**
 * @file srcml_input_src.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef SRCML_INPUT_SRC_HPP
#define SRCML_INPUT_SRC_HPP

#include <srcml.h>
#include <string>
#include <list>
#include <vector>
#include <sstream>
#include <boost/optional.hpp>
#include <src_prefix.hpp>
#include <algorithm>
#include <src_archive.hpp>
#include <archive.h>
#include <sys/stat.h>
#include <numeric>

#ifdef WIN32
#include <io.h>
#define  STDIN_FILENO   0       /* standard input file descriptor */
#define STDOUT_FILENO   1       /* standard output file descriptor */
#define STDERR_FILENO   2       /* standard error file descriptor */
#endif

class srcml_input_src;

typedef std::vector<srcml_input_src> srcml_input_t;
typedef srcml_input_src srcml_output_dest;

enum STATES { INDETERMINATE, SRC, SRCML };

class srcml_input_src {
public:

    srcml_input_src() : unit(0) {}
    srcml_input_src(const std::string& other);
    srcml_input_src(const std::string& other, int fds);
    srcml_input_src(int fds);

    srcml_input_src& operator=(const std::string& other);
    srcml_input_src& operator=(FILE* other);
    srcml_input_src& operator=(int other);

    operator const std::string&() const { return resource; }
    operator FILE*() const { return *fileptr; }
    operator int() const { return *fd; }

    bool operator==(const std::string& other) const { return other == filename; }
    bool operator!=(const char* other) const { return filename != other; }

    const char* c_str() const { return resource.c_str(); }

    std::string filename;
    // only used by filesystem input
    std::string prefix;
    std::string protocol;
    std::string resource;
    std::string plainfile;
    std::string extension;
    boost::optional<FILE*> fileptr;
    boost::optional<int> fd;
    archive* arch;
    enum STATES state;
    std::list<std::string> compressions;
    std::list<std::string> archives;
    bool isdirectory;
    bool exists;
    bool isdirectoryform;
    bool skip;
    int unit;
};

struct srcMLReadArchiveError {
    srcMLReadArchiveError(int status, const std::string& emsg)
    : status(status), errmsg(emsg) {}
    int status;
    std::string errmsg;
};

template <typename T>
inline bool contains(const srcml_input_src&) { return false; }

template <>
inline bool contains<FILE*>(const srcml_input_src& input) { return bool(input.fileptr); }

template <>
inline bool contains<int>(const srcml_input_src& input) { return bool(input.fd); }

inline std::ostream& operator<<(std::ostream& out, const srcml_input_src& input) {

    out << "filename:" << input.filename << '\n';
    out << "protocol:" << input.protocol << '\n';
    out << "resource:" << input.resource << '\n';
    out << "plainfile:" << input.plainfile << '\n';
    out << "extension:" << input.extension << '\n';
    if (input.fileptr)
        out << "fileptr:" << *input.fileptr << '\n';
    if (input.fd)
        out << "fd:" << *input.fd << '\n';
    out << "state:" << input.state << '\n';
    for (const auto& compression : input.compressions)
        out << "compression:" << compression << '\n';
    for (const auto& archive : input.archives)
        out << "archive:" << archive << '\n';
    out << "isdirectory:" << input.isdirectory << '\n';

    return out;
}

int srcml_archive_read_open(srcml_archive* arch, const srcml_input_src& input_source);

#endif
