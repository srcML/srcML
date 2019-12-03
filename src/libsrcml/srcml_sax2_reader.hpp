/**
 * @file srcml_sax2_reader.hpp
 *
 * @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef INCLUDED_SRCML_SAX2_READER_HPP
#define INCLUDED_SRCML_SAX2_READER_HPP

#include <srcml_reader_handler.hpp>

#include <srcSAXController.hpp>

#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml.h>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include <string>
#include <vector>
#include <thread>
#include <boost/optional.hpp>

/**
 * thread_args
 *
 * Structure to hold information to pass
 * to thread function.
 */
struct thread_args {

    /** control for sax processing */
    srcSAXController* control;

    /** handler with hooks for sax processing */
    srcml_reader_handler* handler;
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
