// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_input_src.cpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <srcml_input_src.hpp>

#if defined(WIN32) || defined(WIN64)
#include <sys/stat.h>
 // Copied from linux libc sys/stat.h:
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

namespace {
    std::string cur_extension(const std::string& filename) {

        size_t pos = filename.rfind('.');
        if (pos == std::string::npos)
            return "";

        return filename.substr(pos);
    }

    void pop_extension(std::string& filename) {

        size_t pos = filename.rfind('.');
        if (pos == std::string::npos)
            return;

        filename = filename.substr(0, pos);
    }
}

srcml_input_src::srcml_input_src(const std::string& other) : arch(0), state(INDETERMINATE), isdirectory(false), exists(false), isdirectoryform(false), unit(0) {

    skip = false;

    filename = src_prefix_add_uri(other);

    // filename into protocol and resource
    std::tie(protocol, resource) = src_prefix_split_uri(filename);

    // remove any query string
    if (protocol != "text" && protocol != "filelist" && protocol != "stdin") {
        size_t query_pos = resource.find('?');
        if (query_pos != std::string::npos) {
            resource = resource.substr(0, query_pos);
        }
    }
    exists = false;

    if (protocol == "file") {
        struct stat s;
        exists = stat(resource.data(), &s) == 0;

        isdirectory = exists && S_ISDIR(s.st_mode);
    }

    isdirectoryform = resource.back() == '/';

    if (!isdirectory && protocol != "text") {

        plainfile = resource;

        for (;(extension = cur_extension(plainfile)) != "" && (is_compressed(extension) || is_archive(extension)); pop_extension(plainfile)) {

            // collect compressions
            if (is_compressed(extension)) {
                compressions.push_back(extension);
            }

            // collect archives
            if (is_archive(extension)) {
                archives.push_back(extension);
            }
        }
    }

    if (resource != "-" && protocol != "text")
        state = (extension == ".xml" || extension == ".srcml") ? SRCML : SRC;

    if (protocol == "text")
        state = SRC;

    if (protocol == "stdin")
        fd = STDIN_FILENO;
    if (protocol == "stdout")
        fd = STDOUT_FILENO;
}

srcml_input_src::srcml_input_src(const std::string& other, int fds) : unit(0) {

    srcml_input_src s(other);
    s = fds;

    *this = std::move(s);
}

srcml_input_src::srcml_input_src(int fds) : unit(0) {

    srcml_input_src s("-");
    s = fds;

    *this = std::move(s);
}

srcml_input_src& srcml_input_src::operator=(const std::string& other) { srcml_input_src t(other); *this = std::move(t); return *this; }
srcml_input_src& srcml_input_src::operator=(FILE* other) { fileptr = other; return *this; }
srcml_input_src& srcml_input_src::operator=(int other) { fd = other; return *this; }

extern "C" {
    static int srcml_read_callback(void* context, char* buffer, int len) {
        archive* libarchive_srcml = (archive*) context;

        auto status = archive_read_data(libarchive_srcml, buffer, static_cast<size_t>(len));

        return (int) status;
    }

    static int srcml_close_callback(void* context) {
        archive* libarchive_srcml = (archive*) context;

        archive_read_close(libarchive_srcml);
        archive_read_free(libarchive_srcml);

        return 0;
    }
}

int srcml_archive_read_open(srcml_archive* arch, const srcml_input_src& input_source) {

    int status;
    if (input_source.arch)
        status = srcml_archive_read_open_io(arch, input_source.arch, srcml_read_callback, srcml_close_callback);
    else if (contains<int>(input_source))
        status = srcml_archive_read_open_fd(arch, input_source);
    else if (contains<FILE*>(input_source))
        status = srcml_archive_read_open_FILE(arch, input_source);
    else
        status = srcml_archive_read_open_filename(arch, input_source.data());

    return status;
}

