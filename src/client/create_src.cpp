// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file create_src.hpp
 *
 * @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcml command-line client.
 */

#include <create_src.hpp>
#include <srcml.h>
#include <src_output_libarchive.hpp>
#include <src_output_filesystem.hpp>
#include <src_prefix.hpp>
#include <input_curl.hpp>
#include <input_file.hpp>
#include <input_archive.hpp>
#include <SRCMLStatus.hpp>
#include <libarchive_utilities.hpp>
#include <srcml_utilities.hpp>

#include <stdio.h>
#if !defined(_MSC_VER)
#include <sys/uio.h>
#include <unistd.h>
#define READ read
#define WRITE write
#else
#include <io.h>
#define READ _read
#define WRITE _write
#endif

static std::unique_ptr<srcml_archive> srcml_read_open_internal(const srcml_input_src& input_source, const std::optional<size_t>& revision) {

    OpenFileLimiter::open();
    std::unique_ptr<srcml_archive> arch(srcml_archive_create());
    if (!arch)
        return 0;

    int status = SRCML_STATUS_OK;

    if (revision) {
        status = srcml_archive_set_srcdiff_revision(arch.get(), *revision);
        if (status != SRCML_STATUS_OK)
            return 0;
    }

    // may need to modify input source based on url and compressions
    srcml_input_src curinput = input_source;

    // urls
    bool isCurl = false;
    if (curinput.protocol != "file" && curl_supported(curinput.protocol)) {
        srcml_input_src uninput = curinput;
        if (!input_curl(uninput))
            return 0;

        isCurl = true;
        curinput.fd = *uninput.fd;
    }

    // compressed files
    if (!curinput.compressions.empty() && curinput.archives.empty()) {
        srcml_input_src uninput = curinput;

#if true //WIN32
        // In Windows, the archive_read_open_fd() does not seem to work. The input is read as an empty archive,
        // or cut short. 
        // So for Windows, convert to a FILE*. Note sure when to close the FILE*
        if (isCurl) {
            uninput.fileptr = fdopen(*(uninput.fd), "r");
            uninput.fd = std::nullopt;
        }
#endif

        input_file(uninput);
        curinput.fd = *uninput.fd;
    }

    // archives (and possibly compressions)
    else if (!curinput.archives.empty()) {

#if true //WIN32
        if (isCurl) {
            // In Windows, the archive_read_open_fd() does not seem to work. The input is read as an empty archive,
            // or cut short. 
            // So for Windows, convert to a FILE*. Note sure when to close the FILE*
            curinput.fileptr = fdopen(*(curinput.fd), "r");
            curinput.fd = std::nullopt;
        }
#endif
        
        curinput.fd = input_archive(curinput);
    }

    // open input source
    if (curinput.fd) {
        status = srcml_archive_read_open_fd(arch.get(), *curinput.fd);
    } else {
        status = srcml_archive_read_open(arch.get(), input_source);
    }
    if (status != SRCML_STATUS_OK) {
        SRCMLstatus(WARNING_MSG, "srcml: Unable to open srcml file " + std::string(src_prefix_resource(input_source.filename)));
        return 0;
    }

    return arch;
}

// create source from the current request
void create_src(const srcml_request_t& srcml_request,
                const srcml_input_t& input_sources,
                const srcml_output_dest& destination) {

    if (option(SRCML_COMMAND_TO_DIRECTORY)) {

        // srcml->src extract all archives to the filesystem

        TraceLog log;

        for (const auto& input_source : input_sources) {
            auto arch(srcml_read_open_internal(input_source, srcml_request.revision));

            src_output_filesystem(arch.get(), destination, log);
        }

    } else if (input_sources.size() == 1 && contains<int>(destination) &&
               destination.compressions.empty() && destination.archives.empty()) {

        // srcml->src extract to stdout

        auto arch(srcml_read_open_internal(input_sources[0], srcml_request.revision));

        // move to the correct unit
        for (int i = 1; i < srcml_request.unit; ++i) {
            if (!srcml_archive_skip_unit(arch.get())) {
                SRCMLstatus(ERROR_MSG, "Requested unit %s out of range.", srcml_request.unit);
                exit(1);
            }
        }

        int count = 0;
        while (1) {
            std::unique_ptr<srcml_unit> unit(srcml_archive_read_unit(arch.get()));
            if (srcml_request.unit && !unit) {
                SRCMLstatus(ERROR_MSG, "Requested unit %s out of range.", srcml_request.unit);
                exit(1);
            }
            if (!unit)
                break;

            // set encoding for source output
            // NOTE: How this is done may change in the future
            if (srcml_request.src_encoding)
                srcml_archive_set_src_encoding(arch.get(), srcml_request.src_encoding->data());

            // if requested eol, then use that
            if (srcml_request.eol)
                srcml_unit_set_eol(unit.get(), *srcml_request.eol);

            // null separator before every unit (except the first)
            if (count) {
                if (write(1, "", 1) == -1) {
                    SRCMLstatus(ERROR_MSG, "Unable to write to stdout");
                    break;
                }
            }

            // unparse directly to the destintation
            srcml_unit_unparse_fd(unit.get(), destination);

            // get out if only one unit
            if (srcml_request.unit)
                break;

            ++count;
        }

    } else if (input_sources.size() == 1 && destination.compressions.empty() && destination.archives.empty()) {

        auto arch(srcml_read_open_internal(input_sources[0], srcml_request.revision));

        // move to the correct unit
        for (int i = 1; i < srcml_request.unit; ++i) {
            if (!srcml_archive_skip_unit(arch.get())) {
                SRCMLstatus(ERROR_MSG, "Requested unit %s out of range.", srcml_request.unit);
                exit(1);
            }
        }

        std::unique_ptr<srcml_unit> unit(srcml_archive_read_unit(arch.get()));
        if (!unit) {
            SRCMLstatus(ERROR_MSG, "Requested unit %s out of range.", srcml_request.unit);
            exit(1);
        }

        // set encoding for source output
        // NOTE: How this is done may change in the future
        if (srcml_request.src_encoding)
            srcml_archive_set_src_encoding(arch.get(), srcml_request.src_encoding->data());

        // if requested eol, then use that
        if (srcml_request.eol)
            srcml_unit_set_eol(unit.get(), *srcml_request.eol);

        int status = srcml_unit_unparse_filename(unit.get(), destination.data());
        if (status) {
            SRCMLstatus(ERROR_MSG, "srcml: unable to open output file " + destination.resource);
            exit(1);
        }

    } else {

        // srcml->src extract to libarchive file
        if (destination.archives.size() == 0) {
            SRCMLstatus(ERROR_MSG, "srcml: source output requires an archive format (tar, zip, etc.)");
            exit(1);
        }

        std::unique_ptr<archive> ar(archive_write_new());

        // setup format
        for (const auto& ext : destination.archives)
            archive_write_set_format_by_extension(ar.get(), ext.data());

        // setup compressions
        for (const auto& ext : destination.compressions)
            archive_write_set_compression_by_extension(ar.get(), ext.data());

        int status = ARCHIVE_OK;
        if (contains<int>(destination)) {
            status = archive_write_open_fd(ar.get(), destination);
        } else {
            status = archive_write_open_filename(ar.get(), destination.resource.data());
        }
        if (status != ARCHIVE_OK) {
            SRCMLstatus(ERROR_MSG, std::to_string(status));
            exit(1);
        }

        // extract all the srcml archives to this libarchive
        for (const auto& input_source : input_sources) {

            auto arch(srcml_read_open_internal(input_source, srcml_request.revision));

            // extract this srcml archive to the source archive
            src_output_libarchive(arch.get(), ar.get());
        }
    }
}
