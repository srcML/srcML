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

struct srcMLReadArchiveError {
    srcMLReadArchiveError(int status, const std::string& emsg)
        : status(status), errmsg(emsg) {}
    int status;
    std::string errmsg;
};

class srcMLReadArchive {
public:
    srcMLReadArchive(const srcml_input_src& input_source) {

        arch = srcml_create_archive();
        if (!arch)
            throw srcMLReadArchiveError(0, input_source);

        int status;
        if (contains<int>(input_source))
            status = srcml_read_open_fd(arch, input_source);
        else if (contains<FILE*>(input_source))
            status = srcml_read_open_FILE(arch, input_source);
        else
            status = srcml_read_open_filename(arch, input_source.c_str());
        if (status != SRCML_STATUS_OK)
            throw status;
    }

    operator srcml_archive*() { return arch; }

    ~srcMLReadArchive() {

        srcml_close_archive(arch);
        srcml_free_archive(arch);
    }

private:
    srcml_archive* arch;
};

// create srcml from the current request
void create_src(const srcml_request_t& srcml_request,
                const srcml_input_t& input_sources,
                const srcml_output_dest& destination) {

    try {

        if (srcml_request.command & SRCML_COMMAND_TO_DIRECTORY) {

            // srcml->src extract all archives to the filesystem

            TraceLog log(std::cerr, *srcml_request.markup_options);

            BOOST_FOREACH(const srcml_input_src& input_source, input_sources) {
                srcMLReadArchive arch(input_source);

                src_output_filesystem(arch, destination, log);
            }

        } else if ((srcml_request.command & SRCML_COMMAND_XML) && srcml_request.unit != 0 && input_sources.size() == 1) {

            // srcml->src extract individual unit in XML

            srcMLReadArchive arch(input_sources[0]);

            srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

            srcml_archive* oarch = srcml_create_archive();
            if (contains<int>(destination))
                srcml_write_open_fd(oarch, destination);
            else
                srcml_write_open_filename(oarch, destination.c_str());

            srcml_write_unit(oarch, unit);

            srcml_free_unit(unit);

            srcml_close_archive(oarch);
            srcml_free_archive(oarch);

        } else if (input_sources.size() == 1 && srcml_request.unit > 0) {

            // srcml->src extract individual unit

            srcMLReadArchive arch(input_sources[0]);

            srcml_unit* unit = srcml_read_unit_position(arch, srcml_request.unit);

            if (contains<int>(destination))
                srcml_unparse_unit_fd(unit, destination);
            else
                srcml_unparse_unit_filename(unit, destination.c_str());

            srcml_free_unit(unit);

        } else if (input_sources.size() == 1 && destination == "-") {

            // srcml->src extract to stdout

            srcMLReadArchive arch(input_sources[0]);

            srcml_unit* unit = srcml_read_unit(arch);

            srcml_unparse_unit_fd(unit, STDOUT_FILENO);

            srcml_free_unit(unit);

        } else if (input_sources.size() == 1 && srcml_check_extension(destination.c_str())) {

            // srcml->src extract to plain code file

            srcMLReadArchive arch(input_sources[0]);

            srcml_unit* unit = srcml_read_unit(arch);

            srcml_unparse_unit_filename(unit, destination.c_str());

            srcml_free_unit(unit);

        } else {

            // srcml->src extract to libarchive file

            archive* ar = archive_write_new();

            // setup compression and format
            // TODO: Needs to be generalized from destination file extension
            archive_write_set_compression_gzip(ar);
            archive_write_set_format_pax_restricted(ar);

            int status = ARCHIVE_OK;
            if (contains<int>(destination)) {
                status = archive_write_open_fd(ar, destination);
            } else {
                status = archive_write_open_filename(ar, destination.resource.c_str());
            }

            // extract all the srcml archives to this libarchive
            BOOST_FOREACH(const srcml_input_src& input_source, input_sources) {

                srcMLReadArchive arch(input_source);

                // extract this srcml archive to the source archive
                src_output_libarchive(arch, ar);
            }

            archive_write_close(ar);
            archive_write_finish(ar);
        }

    } catch (srcMLReadArchiveError e) {

        std::cerr << "Error " << e.status << " with " << e.errmsg << '\n';

    } catch (...) {

    }
}
