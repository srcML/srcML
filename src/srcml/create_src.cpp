/**
 * @file create_src.hpp
 *
 * @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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
#include <src_prefix.hpp>
#include <srcml_options.hpp>


class srcMLReadArchive {
public:
    srcMLReadArchive(const srcml_input_src& input_source) {

        arch = srcml_archive_create();
        if (!arch)
            throw srcMLReadArchiveError(0, input_source);

        int status = srcml_archive_read_open(arch, input_source);
        if (status != SRCML_STATUS_OK)
            throw status;
    }

    operator srcml_archive*() { return arch; }

    ~srcMLReadArchive() {

        srcml_archive_close(arch);
        srcml_archive_free(arch);
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

            TraceLog log(SRCMLOptions::get());

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
                srcml_unit_free(unit);
            }

            srcml_unit* unit = srcml_read_unit_header(arch);

            if (!unit) {
                std::cerr << "Requested unit " << srcml_request.unit << " out of range.\n";
                exit(4);
            }

            // SETUP OUTPUT ARCHIVE
            srcml_archive* oarch = srcml_archive_create();
            
            // set options for the output srcml archive
            if (srcml_request.att_xml_encoding)
                srcml_archive_set_xml_encoding(oarch, srcml_request.att_xml_encoding->c_str());

            if (srcml_request.src_encoding)
                srcml_archive_set_src_encoding(oarch, srcml_request.src_encoding->c_str());

            // for single input src archives (e.g., .tar), filename attribute is the source filename (if not already given)
            if (srcml_request.att_url) {
                srcml_archive_set_url(oarch, src_prefix_resource(*srcml_request.att_url).c_str());
            } else if (input_sources.size() == 1 && input_sources[0].archives.size() > 0) {
                srcml_archive_set_url(oarch, src_prefix_resource(input_sources[0].filename).c_str());
            }

            if (srcml_request.att_version)
                srcml_archive_set_version(oarch, srcml_request.att_version->c_str());

            if (srcml_request.markup_options)
                srcml_archive_set_options(oarch, srcml_archive_get_options(oarch) | *srcml_request.markup_options);

            if (*srcml_request.markup_options & SRCML_OPTION_XML_DECL)
                srcml_archive_disable_option(oarch, SRCML_OPTION_XML_DECL);
            else
                srcml_archive_enable_option(oarch, SRCML_OPTION_XML_DECL);

            if (*srcml_request.markup_options & SRCML_OPTION_NAMESPACE_DECL)
                srcml_archive_disable_option(oarch, SRCML_OPTION_NAMESPACE_DECL);
            else
                srcml_archive_enable_option(oarch, SRCML_OPTION_NAMESPACE_DECL);

            if (srcml_request.att_language)
                srcml_archive_set_language(oarch, srcml_request.att_language->c_str());
            else
                srcml_archive_set_language(oarch, SRCML_LANGUAGE_NONE);

            srcml_archive_set_tabstop(oarch, srcml_request.tabs);

            // non-archive when:
            //   only one input
            //   no cli request to make it an archive
            //   not a directory (if local file)
            // TODO: check if a plain file. Source archives, i.e., .tar.gz, always produce srcml archives
            if (input_sources.size() == 1 && input_sources[0].protocol != "filelist" &&
                !(srcml_request.markup_options && (*srcml_request.markup_options & SRCML_OPTION_ARCHIVE)) &&
                !input_sources[0].isdirectory) {

                srcml_archive_disable_option(oarch, SRCML_OPTION_ARCHIVE);
                srcml_archive_disable_option(oarch, SRCML_OPTION_HASH);
            } else {
                
                srcml_archive_enable_option(oarch, SRCML_OPTION_ARCHIVE);
                srcml_archive_enable_option(oarch, SRCML_OPTION_HASH);
            }

            // register file extensions
            BOOST_FOREACH(const std::string& ext, srcml_request.language_ext) {
                size_t pos = ext.find('=');
                srcml_archive_register_file_extension(oarch, ext.substr(0, pos).c_str(), ext.substr(pos+1).c_str());
            }

            // register xml namespaces
            std::map<std::string, std::string>::const_iterator itr;
            for(itr = srcml_request.xmlns_namespaces.begin(); itr != srcml_request.xmlns_namespaces.end(); ++itr){
                srcml_archive_register_namespace(oarch, (*itr).first.c_str(), (*itr).second.c_str());
            }
            
            /**** SHOULD THIS GO
            if (srcml_request.markup_options)
                srcml_archive_set_options(oarch, srcml_archive_get_options(oarch) | *srcml_request.markup_options);

            if (*srcml_request.markup_options & SRCML_OPTION_XML_DECL)
                    srcml_archive_disable_option(oarch, SRCML_OPTION_XML_DECL);

            srcml_archive_disable_option(oarch, SRCML_OPTION_ARCHIVE);
            *****/

            unsigned short compression = 0;
            if (!destination.compressions.empty() && destination.compressions.front() == ".gz")
                compression = 9;

            if (contains<int>(destination))
                srcml_archive_write_open_fd(oarch, destination);
            else
                srcml_archive_write_open_filename(oarch, destination.c_str(), compression);

            
            srcml_write_unit(oarch, unit);

            srcml_unit_free(unit);

            srcml_archive_close(oarch);
            srcml_archive_free(oarch);

        } else if (input_sources.size() == 1 && contains<int>(destination) &&
                   destination.compressions.empty() && destination.archives.empty()) {

            // srcml->src extract to stdout

            srcMLReadArchive arch(input_sources[0]);

            // move to the correct unit
            for (int i = 1; i < srcml_request.unit; ++i) {
                srcml_unit* unit = srcml_read_unit_header(arch);
                srcml_unit_free(unit);
            }

            srcml_unit* unit = srcml_read_unit_header(arch);

            if (!unit) {
                std::cerr << "Requested unit " << srcml_request.unit << " out of range.\n";
                exit(4);
            }
            
            // set encoding for source output
            // NOTE: How this is done may change in the future
            if (srcml_request.src_encoding)
                srcml_archive_set_src_encoding(arch, srcml_request.src_encoding->c_str());

            srcml_unit_unparse_fd(unit, destination);

            srcml_unit_free(unit);

        } else if (input_sources.size() == 1 && destination.compressions.empty() && destination.archives.empty()) {

            // srcml->src extract to plain code file

            srcMLReadArchive arch(input_sources[0]);

            // move to the correct unit
            for (int i = 1; i < srcml_request.unit; ++i) {
                srcml_unit* unit = srcml_read_unit_header(arch);
                srcml_unit_free(unit);
            }

            srcml_unit* unit = srcml_read_unit_header(arch);

            if (!unit) {
                std::cerr << "Requested unit " << srcml_request.unit << " out of range.\n";
                exit(4);
            }

            // set encoding for source output
            // NOTE: How this is done may change in the future
            if (srcml_request.src_encoding)
                srcml_archive_set_src_encoding(arch, srcml_request.src_encoding->c_str());

            srcml_unit_unparse_filename(unit, destination.c_str(), 0);

            srcml_unit_free(unit);

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
