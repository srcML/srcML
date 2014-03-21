/**
 * @file srcml.cpp
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

#include <srcml.h>
#include <srcml_cli.hpp>
#include <srcml_display_info.hpp>
#include <srcml_list_unit_files.hpp>
#include <src_input_validator.hpp>
#include <src_language.hpp>
#include <trace_log.hpp>
#include <srcml_options.hpp>
#include <create_srcml.hpp>
#include <create_src.hpp>
#include <isxml.hpp>

#include <archive.h>
#include <archive_entry.h>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <errno.h>

#ifdef WIN32
#include <io.h>
#define  STDIN_FILENO   0       /* standard input file descriptor */
#define STDOUT_FILENO   1       /* standard output file descriptor */
#define STDERR_FILENO   2       /* standard error file descriptor */
#endif

void peek4char(FILE* fp, unsigned char data[], ssize_t* psize);

int main(int argc, char * argv[]) {

    // parse the command line
    srcml_request_t srcml_request = parseCLI(argc, argv);

    SRCMLOptions::set(srcml_request.command);

    // version
    if (srcml_request.command & SRCML_COMMAND_VERSION) {
        std::cout <<  srcml_version_string() << "\n";
        std::cout << "srcml Version Number " << srcml_version_number() << "\n";
        std::cout << "libarchive Version " << ARCHIVE_VERSION_NUMBER << "\n";
        return 0;
    }

    const int SRCML_COMMAND_INSRCML =
        SRCML_COMMAND_LONGINFO |
        SRCML_COMMAND_INFO    |
        SRCML_COMMAND_INFO_FILENAME |
        SRCML_COMMAND_VERSION |
        SRCML_COMMAND_LIST |
        SRCML_COMMAND_UNITS |
        SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE |
        SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY |
        SRCML_COMMAND_DISPLAY_SRCML_FILENAME |
        SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION |
        SRCML_COMMAND_DISPLAY_SRCML_ENCODING;

    // determine whether the input is xml(srcml) or not
    boost::optional<FILE*> fstdin;
    bool createsrc = false;
    bool createsrcml = false;
    bool insrcml = srcml_request.command & SRCML_COMMAND_INSRCML;
    if (srcml_request.command & SRCML_COMMAND_SRC) {
        createsrc = true;
    } else if (srcml_request.command & SRCML_COMMAND_SRCML) {
        createsrcml = true;
    } else if (!srcml_request.files_from.empty()) {
        createsrcml = true;
    } else {

        // Need to determine if we need src->srcml, srcml->src, or src->srcml->src
        // This is determined using the types of the input files
        //  * All src files imply src->srcml
        //  * A single src file implies src->srcml
        // Note: src->srcml->src implies a temporary srcml file

        int count_src = 0;
        BOOST_FOREACH(const std::string& input_filename, srcml_request.input) {
            if (input_filename != "stdin://-" && input_filename != "-") {

                // base on extension
                if (boost::filesystem::path(input_filename.substr(7).c_str()).extension().compare(".xml") != 0)
                    ++count_src;

            } else {

                // Note: If stdin only, then have to read from this FILE*, then make sure to use it below
                fstdin = fdopen(STDIN_FILENO, "r");

                // read the first 4 bytes as separate characters to get around byte ordering
                unsigned char data[4];
                ssize_t size = 0;
                peek4char(*fstdin, data, &size);

                // pass the first 4 bytes and the size actually read in
                if (!isxml(data, size))
                    ++count_src;
            }
        }

        // now see where we are at on the output format
        if (count_src > 0) {
            createsrcml = true;
            createsrc = srcml_request.output_filename && *srcml_request.output_filename != "-"
                && boost::filesystem::path(srcml_request.output_filename->c_str()).extension().compare(".xml") != 0;
        } else {
            createsrcml = srcml_request.output_filename && *srcml_request.output_filename != "-"
                && boost::filesystem::path(srcml_request.output_filename->c_str()).extension().compare(".xml") == 0;
            createsrc = !createsrcml;
        }
    }

    if (createsrcml && (srcml_request.input.empty() || srcml_request.sawstdin) && !srcml_request.att_language) {
        std::cerr << "Using stdin requires a declared language\n";
        exit(1);
    }


    // STAGE 1: Create any srcml that we need
    //          This includes an "merges"
    if (createsrcml)
        create_srcml(srcml_request, fstdin, *srcml_request.output_filename);

    if (insrcml) {
        // srcml->src language
        if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_LANGUAGE){
        }
        // srcml->src directory
        else if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_DIRECTORY){
        }
        // srcml->src filename
        else if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_FILENAME){
        }
        // srcml->src src version
        else if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_SRC_VERSION){
        }
        // srcml->src encoding
        else if (srcml_request.command & SRCML_COMMAND_DISPLAY_SRCML_ENCODING){
        }
        // srcml long info
        else if (srcml_request.command & SRCML_COMMAND_LONGINFO) {
            srcml_display_info(srcml_request.input);
        }
        // srcml info
        else if (srcml_request.command & SRCML_COMMAND_INFO) {
            srcml_display_info(srcml_request.input);
        }
        // list filenames in srcml archive
        else if (srcml_request.command & SRCML_COMMAND_LIST) {
            srcml_list_unit_files(srcml_request.input);

        }
    }

    if (createsrc) {
        create_src(srcml_request, fstdin, *srcml_request.output_filename);
    }

    srcml_cleanup_globals();

    return 0;
}

void peek4char(FILE* fp, unsigned char data[], ssize_t* psize) {
    *psize = 0;
    int c;
    if ((c = getc(fp)) != EOF) {
        data[0] = c;
        ++(*psize);
        if ((c = getc(fp)) != EOF) {
            data[1] = c;
            ++(*psize);
            if ((c = getc(fp)) != EOF) {
                data[2] = c;
                ++(*psize);
                if ((c = getc(fp)) != EOF) {
                    data[3] = c;
                    ++(*psize);
                }
            }
        }
    }

    for (ssize_t i = (*psize) - 1; i >= 0; --i)
        ungetc(data[i], fp);
}

