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

    // local files may carry a trailing range suffix, e.g. "main.cpp:191", "main.cpp:191:5", or "main.cpp:191:5-222:12"
    if (protocol == "file"sv) {

        // a number at position i of s, advancing i, where a position of zero is invalid,
        // and a number too large for a position is not part of a suffix at all
        auto number = [](std::string_view s, size_t& i) -> std::optional<int> {

            size_t digits = i;
            while (digits < s.size() && s[digits] >= '0' && s[digits] <= '9')
                ++digits;

            if (digits == i)
                return std::nullopt;

            int position = 0;
            try {
                position = std::stoi(std::string(s.substr(i, digits - i)));
            } catch (const std::out_of_range&) {
                return std::nullopt;
            }

            i = digits;

            return position != 0 ? position : INVALID_POSITION;
        };

        // "LINE[:COLUMN][-[LINE[:COLUMN]]]" or "-LINE[:COLUMN]", the part of a suffix after the colon
        auto parse_suffix = [&number](std::string_view s, int& line, int& column, int& end_line, int& end_column) {

            line = 1;
            column = 0;
            end_line = 0;
            end_column = 0;

            size_t i = 0;
            bool isstart = !s.empty() && s[0] != '-';

            if (isstart) {

                auto n = number(s, i);
                if (!n)
                    return false;

                line = *n;

                if (i < s.size() && s[i] == ':') {

                    auto c = number(s, ++i);
                    if (!c)
                        return false;

                    column = *c;
                }
            }

            // without a range, the end is the end of the start line
            if (i == s.size()) {

                end_line = line;

                return isstart;
            }

            if (s[i] != '-')
                return false;

            // an open range ends at the end of the file
            if (++i == s.size()) {

                end_line = END_OF_FILE;

                return isstart;
            }

            auto n = number(s, i);
            if (!n)
                return false;

            end_line = *n;

            if (i < s.size() && s[i] == ':') {

                auto c = number(s, ++i);
                if (!c)
                    return false;

                end_column = *c;
            }

            return i == s.size();
        };

        // the suffix starts at the first colon of the filename whose remainder is an entire suffix
        size_t base = resource.find_last_of("/\\");
        for (size_t colon_pos = resource.find(':', base == std::string::npos ? 0 : base + 1);
             colon_pos != std::string::npos; colon_pos = resource.find(':', colon_pos + 1)) {

            int suffix_line = 0;
            int suffix_column = 0;
            int suffix_end_line = 0;
            int suffix_end_column = 0;

            if (!parse_suffix(std::string_view(resource).substr(colon_pos + 1), suffix_line, suffix_column, suffix_end_line, suffix_end_column))
                continue;

            // a file of that exact name, suffix and all, is a filename, not a suffix
            struct stat s;
            if (stat(resource.data(), &s) != 0) {

                line = suffix_line;
                column = suffix_column;
                end_line = suffix_end_line;
                end_column = suffix_end_column;

                resource = resource.substr(0, colon_pos);
                filename = src_prefix_add_uri(protocol, resource);
            }

            break;
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
