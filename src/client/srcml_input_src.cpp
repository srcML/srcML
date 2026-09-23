// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_input_src.cpp
 *
 * @copyright Copyright (C) 2014-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <srcml_input_src.hpp>
#include <string_view>

using namespace ::std::literals::string_view_literals;

#if defined(WIN32) || defined(WIN64)
#include <sys/stat.h>
 // Copied from linux libc sys/stat.h:
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#endif

namespace {
    std::string cur_extension(std::string_view filename) {

        size_t pos = filename.rfind('.');
        if (pos == std::string::npos)
            return "";

        return std::string(filename.substr(pos));
    }

    void pop_extension(std::string& filename) {

        size_t pos = filename.rfind('.');
        if (pos == std::string::npos)
            return;

        filename = filename.substr(0, pos);
    }
}

srcml_input_src::srcml_input_src(std::string_view other) {

    skip = false;

    filename = src_prefix_add_uri(other);

    // filename into protocol and resource
    std::tie(protocol, resource) = src_prefix_split_uri(filename);

    // remove any query string
    if (protocol != "text"sv && protocol != "filelist"sv && protocol != "stdin"sv) {
        size_t query_pos = resource.find('?');
        if (query_pos != std::string::npos) {
            resource = resource.substr(0, query_pos);
        }
    }

    // local files may carry a trailing line, and column, suffix, e.g. "main.cpp:191" or "main.cpp:191:5"
    if (protocol == "file"sv) {

        // remove a trailing ":N", returning N, or -1 when there is none
        auto pop_position = [](std::string& s) {

            size_t colon_pos = s.rfind(':');
            if (colon_pos == std::string::npos || colon_pos + 1 >= s.size() ||
                s.find_first_not_of("0123456789", colon_pos + 1) != std::string::npos)
                return -1;

            int position = 0;
            try {
                position = std::stoi(s.substr(colon_pos + 1));
            } catch (const std::out_of_range&) {
                // a number too large for a position is part of the filename
                return -1;
            }

            s = s.substr(0, colon_pos);

            return position;
        };

        if (int last = pop_position(resource); last != -1) {

            // two positions are ":line:column", a single position is ":line"
            // lines and columns start at one, so a position of zero is invalid
            if (int first = pop_position(resource); first != -1) {
                line = first != 0 ? first : INVALID_POSITION;
                column = last != 0 ? last : INVALID_POSITION;
            } else {
                line = last != 0 ? last : INVALID_POSITION;
            }

            filename = src_prefix_add_uri(protocol, resource);
        }
    }

    exists = false;

    if (protocol == "file"sv) {
        struct stat s;
        exists = stat(resource.data(), &s) == 0;

        isdirectory = exists && S_ISDIR(s.st_mode);
    }

    isdirectoryform = !resource.empty() && resource.back() == '/';

    if (!isdirectory && protocol != "text"sv) {

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

    if (resource != "-"sv && protocol != "text"sv)
        state = (extension == ".xml"sv || extension == ".srcml"sv) ? SRCML : SRC;

    if (protocol == "text"sv)
        state = SRC;

    if (protocol == "stdin"sv)
        fd = STDIN_FILENO;
    if (protocol == "stdout"sv)
        fd = STDOUT_FILENO;
}

srcml_input_src::srcml_input_src(std::string_view other, int fds) {

    srcml_input_src s(other);
    s = fds;

    *this = std::move(s);
}

srcml_input_src::srcml_input_src(int fds) {

    srcml_input_src s("-");
    s = fds;

    *this = std::move(s);
}

srcml_input_src& srcml_input_src::operator=(std::string_view other) { srcml_input_src t(other); *this = std::move(t); return *this; }
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
    if (input_source.memory)
        status = srcml_archive_read_open_memory(arch, input_source.memory->data(), input_source.memory->size());
    else if (input_source.arch)
        status = srcml_archive_read_open_io(arch, input_source.arch, srcml_read_callback, srcml_close_callback);
    else if (contains<int>(input_source))
        status = srcml_archive_read_open_fd(arch, input_source);
    else if (contains<FILE*>(input_source))
        status = srcml_archive_read_open_FILE(arch, input_source);
    else
        status = srcml_archive_read_open_filename(arch, input_source.data());

    return status;
}
