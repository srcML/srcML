// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_sax2_reader.cpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#include <srcml_sax2_reader.hpp>
#include <stdlib.h>
#include <iostream>

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

            fprintf(stderr, "Error Parsing: %s\n", error.message.data());

            args->handler->done();
        }

        // might have to release a lock here or set is_done
    }

    return 0;
}

/**
 * srcml_sax2_reader
 * @param input parser input buffer
 *
 * Construct a srcml_sax2_reader using a parser input buffer
 */
srcml_sax2_reader::srcml_sax2_reader(srcml_archive* archive, std::unique_ptr<xmlParserInputBuffer> input)
    : control(std::move(input)), handler() {

    handler.archive = archive;

    // Setup thread here after things are created and settled
    // Do not put in member initialization list as it can cause random crashes
    thread = std::thread(start_routine, &args);

    handler.wait();
}

/**
 * ~srcml_sax2_reader
 *
 * Destructor a srcml_sax2_reader
 */
srcml_sax2_reader::~srcml_sax2_reader() {

    handler.stop();

    if (thread.joinable()) {
        try {
            thread.join();
        } catch(const std::system_error& e) {
            std::cerr << "srcml: Internal error " << e.code() << " meaning " << e.what() << '\n';
        }
    }
}

/**
 * read_header
 * @param language a location to store the language attribute
 * @param filename a location to store the filename attribute
 * @param url a location to store the url attribute
 * @param version a location to store the version attribute
 *
 * Read attributes from next unit.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcml_sax2_reader::read_header(srcml_unit* unit) {

    handler.unit = unit;

    if (handler.is_done)
        return 0;

    handler.skip = true;
    handler.collect_unit_header = true;
    handler.collect_unit_body = false;
    handler.resume_and_wait();
    handler.collect_unit_body = false;
    handler.collect_unit_header = false;
    handler.skip = false;

    if (handler.is_done)
        return 0;

    handler.unit = 0;

    unit->read_header = true;

    return 1;
}

/**
 * read
 * @param language a location to store the language attribute
 * @param filename a location to store the filename attribute
 * @param url a location to store the url attribute
 * @param version a location to store the version attribute
 *
 * Read attributes from next unit.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcml_sax2_reader::read(srcml_unit* unit) {

    handler.unit = unit;

    if (handler.is_done)
        return 0;

    handler.skip = true;
    handler.collect_unit_header = true;
    handler.collect_unit_body = true;
    handler.resume_and_wait();
    handler.collect_unit_body = false;
    handler.collect_unit_header = false;
    handler.skip = false;

    if (handler.is_done)
        return 0;

    handler.unit = 0;

    unit->read_header = true;
    unit->read_body = true;

    return 1;
}

/**
 * read_body
 * @param output_buffer output buffer to write text
 *
 * Read the next unit from a srcML Archive
 * and return in the passed string parameter.
 *
 * @returns 1 on success and 0 if done
 */
int srcml_sax2_reader::read_body(srcml_unit* unit) {

    if (handler.is_done)
        return 0;

    handler.unit = unit;
    handler.collect_unit_body = true;
    handler.resume_and_wait();
    handler.collect_unit_body = false;

    if (handler.is_done)
        return 0;

    unit->read_body = true;

    return 1;
}
