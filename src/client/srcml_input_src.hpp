// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_input_src.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#ifndef SRCML_INPUT_SRC_HPP
#define SRCML_INPUT_SRC_HPP

#include <srcml.h>
#include <string>
#include <string_view>
#include <list>
#include <vector>
#include <sstream>
#include <optional>
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

    srcml_input_src() {}
    srcml_input_src(std::string_view other);
    srcml_input_src(std::string_view other, int fds);
    srcml_input_src(int fds);

    srcml_input_src& operator=(std::string_view other);
    srcml_input_src& operator=(FILE* other);
    srcml_input_src& operator=(int other);

    operator std::string() const { return resource; }
    operator const std::string&() const { return resource; }
    operator std::string_view() const { return resource; }
    operator FILE*() const { return *fileptr; }
    operator int() const { return *fd; }

    bool operator==(std::string_view other) const { return other == filename; }
    bool operator!=(const char* other) const { return filename != other; }

    const char* data() const { return resource.data(); }

    std::string filename;
    // only used by filesystem input
    std::string prefix;
    std::string protocol;
    std::string resource;
    std::string plainfile;
    std::string extension;
    std::optional<FILE*> fileptr;
    std::optional<int> fd;
    archive* arch = nullptr;
    enum STATES state = INDETERMINATE;
    std::list<std::string> compressions;
    std::list<std::string> archives;
    bool isdirectory = false;
    bool exists = false;
    bool isdirectoryform = false;
    bool skip = false;
    int unit = 0;
};

struct srcMLReadArchiveError {
    srcMLReadArchiveError(int status, std::string_view emsg)
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
