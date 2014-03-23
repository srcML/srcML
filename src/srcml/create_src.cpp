/**
 * @file create_src.hpp
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

#include <create_src.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <srcml.h>
#include <src_output_libarchive.hpp>
#include <src_output_filesystem.hpp>

// create srcml from the current request
void create_src(srcml_input_t& input_sources,
                srcml_request_t& srcml_request) {

    // srcml->src srcML file to filesystem
    if (srcml_request.command & SRCML_COMMAND_TO_DIRECTORY) {

        TraceLog log(std::cerr, *srcml_request.markup_options);

        // process command line inputs
        BOOST_FOREACH(const srcml_input_src& input_file, input_sources) {

            srcml_archive* arch = srcml_create_archive();
            int status;
            if (input_file.has_fd())
                status = srcml_read_open_fd(arch, input_file);
            else if (input_file.has_fileptr())
                status = srcml_read_open_FILE(arch, input_file);
            else
                status = srcml_read_open_filename(arch, input_file.c_str());

            src_output_filesystem(arch, *srcml_request.output_filename, log);

            srcml_close_archive(arch);
            srcml_free_archive(arch);
        }

        // srcml->src extract individual unit in XML
    } else if ((srcml_request.command & SRCML_COMMAND_XML) && srcml_request.unit != 0 && input_sources.size() == 1) {

        const srcml_input_src& input_file = input_sources[0];

        srcml_archive* arch = srcml_create_archive();
        int status;
        if (input_file.has_fd())
            status = srcml_read_open_fd(arch, input_file);
        else if (input_file.has_fileptr())
            status = srcml_read_open_FILE(arch, input_file);
        else
            status = srcml_read_open_filename(arch, input_file.c_str());

        srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

        // TODO: We would have to use extend the API, or we will be creating/closing files
        srcml_archive* oarch = srcml_create_archive();
        srcml_write_open_filename(oarch, srcml_request.output_filename->c_str());

        srcml_write_unit(oarch, unit);

        srcml_close_archive(oarch);
        srcml_free_archive(oarch);

        srcml_close_archive(arch);
        srcml_free_archive(arch);

        // srcml->src extract individual unit to file
    } else if (input_sources.size() == 1 && srcml_request.unit > 0) {

        const srcml_input_src& input_file = input_sources[0];

        srcml_archive* arch = srcml_create_archive();
        int status;
        if (input_file.has_fd())
            status = srcml_read_open_fd(arch, input_file);
        else if (input_file.has_fileptr())
            status = srcml_read_open_FILE(arch, input_file);
        else
            status = srcml_read_open_filename(arch, input_file.c_str());

        srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

        if (*srcml_request.output_filename == "-")
            srcml_unparse_unit_fd(unit, STDOUT_FILENO);
        else
            srcml_unparse_unit_filename(unit, srcml_request.output_filename->c_str());

        srcml_close_archive(arch);
        srcml_free_archive(arch);

        // srcml->src srcML file extracted to stdout
    } else if (input_sources.size() == 1 && *srcml_request.output_filename == "-") {

        const srcml_input_src& input_file = input_sources[0];

        srcml_archive* arch = srcml_create_archive();
        int status;
        if (input_file.has_fd())
            status = srcml_read_open_fd(arch, input_file);
        else if (input_file.has_fileptr())
            status = srcml_read_open_FILE(arch, input_file);
        else
            status = srcml_read_open_filename(arch, input_file.c_str());

        srcml_unit* unit = srcml_read_unit(arch);

        srcml_unparse_unit_fd(unit, STDOUT_FILENO);

        srcml_close_archive(arch);
        srcml_free_archive(arch);

        // srcml->src srcML file to libarchive file
    } else {

        // TODO: What if this is a simple, single file? or to stdout?
        archive* ar = archive_write_new();

        // setup compression and format
        // TODO: Needs to be generalized from output file extension
        archive_write_set_compression_gzip(ar);
        archive_write_set_format_pax_restricted(ar);

        archive_write_open_filename(ar, srcml_request.output_filename->c_str());

        // process command line inputs
        BOOST_FOREACH(const srcml_input_src& input_file, input_sources) {

            srcml_archive* arch = srcml_create_archive();
            int status;
            if (input_file.has_fd())
                status = srcml_read_open_fd(arch, input_file);
            else if (input_file.has_fileptr())
                status = srcml_read_open_FILE(arch, input_file);
            else
                status = srcml_read_open_filename(arch, input_file.c_str());

            // extract this srcml archive to the source archive
            src_output_libarchive(arch, ar);

            srcml_close_archive(arch);
            srcml_free_archive(arch);
        }

        archive_write_close(ar);
        archive_write_finish(ar);

    }
}
