// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_sax2_reader.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
 */

#ifndef INCLUDED_SRCML_SAX2_READER_HPP
#define INCLUDED_SRCML_SAX2_READER_HPP

#include <srcml_reader_handler.hpp>

#include <srcSAXController.hpp>

#include <srcml_types.hpp>
#include <srcml.h>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include <string>
#include <vector>
#include <thread>
#include <optional>

/**
 * thread_args
 *
 * Structure to hold information to pass
 * to thread function.
 */
struct thread_args {

    /** control for sax processing */
    srcSAXController* control = nullptr;

    /** handler with hooks for sax processing */
    srcml_reader_handler* handler = nullptr;
};

/**
 * srcml_sax2_reader
 *
 * Extend XML Text Reader interface to
 * progressively read a srcML Archive collecting
 * units and reading unit attributes.
 */
class srcml_sax2_reader {

public :

    /** control for sax parsing */
    srcSAXController control;

    /** hooks for sax parsing */
    srcml_reader_handler handler;

private :

    /** thread running execution */
    std::thread thread;
    thread_args args = { &control, &handler };

public :

    // constructors
    srcml_sax2_reader(srcml_archive* archive, std::unique_ptr<xmlParserInputBuffer> input);

    // destructors
    ~srcml_sax2_reader();

    /* finds next unit tag if not current unit and sets attributes.  Consumes unit.
       Unit is still avaible for readsrcML or read.  But not readUnitAttributes.
    */
    int read_header(srcml_unit* unit);

    int read(srcml_unit* unit);

    // reads the next unit and returns it in parameter as srcML
    int read_body(srcml_unit* unit);
};

#endif
