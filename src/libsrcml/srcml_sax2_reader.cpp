/**
 * @file srcml_sax2_reader.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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

#include <srcml_sax2_reader.hpp>

#include <srcmlns.hpp>
#include <srcml.h>

#include <stdlib.h>
#include <cstring>

/**
 * start_routine
 * @param arguments thread_args structure with control and handler
 *
 * Starts the parsing of the document.
 */
static void* start_routine(thread_args* args) {

    try {

        args->control->parse(args->handler);

    } catch(SAXError error) {

        if (!(error.error_code == XML_ERR_EXTRA_CONTENT || error.error_code == XML_ERR_DOCUMENT_END)) {

            // TODO: Make sure error code is passed back, so that SRCML_STATUS_OK is not returned
            // See #1218
            fprintf(stderr, "Error Parsing: %s\n", error.message.c_str());

            // This is not necessary on certain errors (such as missing '>'), as
            // the thread will finish on its own. But if the document is empty it will
            // freeze up until it is shut down here.
            if (error.error_code == XML_ERR_DOCUMENT_EMPTY)
                args->handler->done();
        }

        // might have to release a lock here or set is_done;
    }

    return 0;
}

/**
 * srcml_sax2_reader
 * @param filename name of a file
 * @param encoding the character encoding
 *
 * Construct a srcml_sax2_reader using a filename
 */
srcml_sax2_reader::srcml_sax2_reader(srcml_archive* archive, const char* filename, const char* encoding, const boost::optional<size_t>& revision_number)
    : control(filename, encoding), handler(revision_number), thread(start_routine, &args) {

    handler.archive = archive;

    handler.wait();
}

/**
 * srcml_sax2_reader
 * @param input parser input buffer
 *
 * Construct a srcml_sax2_reader using a parser input buffer
 */
srcml_sax2_reader::srcml_sax2_reader(srcml_archive* archive, xmlParserInputBufferPtr input, const boost::optional<size_t>& revision_number)
    : control(input), handler(revision_number), thread(start_routine, &args) {

    handler.archive = archive;

    handler.wait();
}

/**
 * ~srcml_sax2_reader
 *
 * Destructor a srcml_sax2_reader
 */
srcml_sax2_reader::~srcml_sax2_reader() {

    handler.stop();
    
    thread.join();
}

/**
 * read_unit_attributes
 * @param language a location to store the language attribute
 * @param filename a location to store the filename attribute
 * @param url a location to store the url attribute
 * @param version a location to store the version attribute
 *
 * Read attributes from next unit.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcml_sax2_reader::read_unit_attributes(srcml_unit* unit) {

    handler.unit = unit;

    if (handler.is_done)
        return 0;

    handler.skip = true;
    handler.collect_unit_attributes = true;
    handler.resume_and_wait();
    handler.collect_unit_attributes = false;
    handler.skip = false;

    if (handler.is_done)
        return 0;

    handler.unit = 0;

    return 1;
}

/**
 * read_srcml
 * @param unit location in which to read srcML unit.
 *
 * Read the next unit from a srcML Archive
 * and return in the passed string parameter.
 *
 * @returns 1 on success and 0 if done
 */
int srcml_sax2_reader::read_srcml(srcml_unit* unit) {

    unit->unit = "";

    handler.unit = unit;

    if (handler.is_done)
        return 0;

    handler.collect_srcml = true;
    handler.resume_and_wait();
    handler.collect_srcml = false;

    if (handler.is_done)
        return 0;

    handler.unit = 0;

    return 1;
}

/**
 * read_src
 * @param output_buffer output buffer to write text
 *
 * Read the next unit from a srcML Archive
 * and return in the passed string parameter.
 *
 * @returns 1 on success and 0 if done
 */
int srcml_sax2_reader::read_src(xmlOutputBufferPtr output_buffer) {

    if (handler.is_done)
        return 0;

    control.enable_comment(false);
    control.enable_cdataBlock(false);
    handler.output_buffer = output_buffer;
    handler.collect_src = true;
    handler.resume_and_wait();
    handler.collect_src = false;
    handler.output_buffer = 0;
    control.enable_comment(true);
    control.enable_cdataBlock(true);

    if (handler.is_done)
        return 0;

    return 1;
}
