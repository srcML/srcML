/**
 * @file create_src.hpp
 *
 * @copyright Copyright (C) 2014 SDML (www.srcML.org)
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <create_src.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <srcml.h>
#include <src_output_libarchive.hpp>
#include <src_output_filesystem.hpp>



class srcMLReadArchive {
public:
    srcMLReadArchive(const srcml_input_src& input_source) {

        arch = srcml_create_archive();
        if (!arch)
            throw srcMLReadArchiveError(0, input_source);

        int status = srcml_read_open(arch, input_source);
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

            TraceLog log(*srcml_request.markup_options);

            BOOST_FOREACH(const srcml_input_src& input_source, input_sources) {
                srcMLReadArchive arch(input_source);

                src_output_filesystem(arch, destination, log);
            }

        } else if (input_sources.size() == 1 && (srcml_request.command & SRCML_COMMAND_XML)) {

            // srcml->src extract individual unit in XML

            srcMLReadArchive arch(input_sources[0]);

            // move to the correct unit
            for (int i = 1; i < srcml_request.unit; ++i) {
                srcml_unit* unit = srcml_read_unit_header(arch);
                srcml_free_unit(unit);
            }

            srcml_unit* unit = srcml_read_unit_header(arch);

            if (!unit) {
                std::cerr << "Requested unit " << srcml_request.unit << " out of range.\n";
                exit(4);
            }

            srcml_archive* oarch = srcml_create_archive();
            srcml_archive_disable_option(oarch, SRCML_OPTION_ARCHIVE);

            if (contains<int>(destination))
                srcml_write_open_fd(oarch, destination);
            else
                srcml_write_open_filename(oarch, destination.c_str());

            if (!destination.compressions.empty() && destination.compressions.front() == ".gz")
                srcml_archive_enable_option(oarch, SRCML_OPTION_COMPRESS);

            srcml_write_unit(oarch, unit);

            srcml_free_unit(unit);

            srcml_close_archive(oarch);
            srcml_free_archive(oarch);

        } else if (input_sources.size() == 1 && contains<int>(destination) &&
                   destination.compressions.empty() && destination.archives.empty()) {

            // srcml->src extract to stdout

            srcMLReadArchive arch(input_sources[0]);

            // move to the correct unit
            for (int i = 1; i < srcml_request.unit; ++i) {
                srcml_unit* unit = srcml_read_unit_header(arch);
                srcml_free_unit(unit);
            }

            srcml_unit* unit = srcml_read_unit_header(arch);

            if (!unit) {
                std::cerr << "Requested unit " << srcml_request.unit << " out of range.\n";
                exit(4);
            }
            
            srcml_unparse_unit_fd(unit, destination);

            srcml_free_unit(unit);

        } else if (input_sources.size() == 1 && destination.compressions.empty() && destination.archives.empty()) {

            // srcml->src extract to plain code file

            srcMLReadArchive arch(input_sources[0]);

            // move to the correct unit
            for (int i = 1; i < srcml_request.unit; ++i) {
                srcml_unit* unit = srcml_read_unit_header(arch);
                srcml_free_unit(unit);
            }

            srcml_unit* unit = srcml_read_unit_header(arch);

            if (!unit) {
                std::cerr << "Requested unit " << srcml_request.unit << " out of range.\n";
                exit(4);
            }

            srcml_unparse_unit_filename(unit, destination.c_str());

            srcml_free_unit(unit);

        } else {

            // srcml->src extract to libarchive file

            archive* ar = archive_write_new();

            // setup format
            BOOST_FOREACH(const std::string& ext, destination.archives)
                archive_write_set_format_by_extension(ar, ext.c_str());

            // setup compressions
            BOOST_FOREACH(const std::string& ext, destination.compressions)
                archive_write_set_compression_by_extension(ar, ext.c_str());

            int status = ARCHIVE_OK;
            if (contains<int>(destination)) {
                status = archive_write_open_fd(ar, destination);
            } else {
                status = archive_write_open_filename(ar, destination.resource.c_str());
            }
            if (status != ARCHIVE_OK) {
                std::cerr << status;
                exit(1);
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
