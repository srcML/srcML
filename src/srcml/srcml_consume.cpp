/**
 * @file srcml_consume.cpp
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
 *
 * Thread-designed function to parse and create a unit.
 * Output is schedule for writing on the write queue
 */

#include <srcml_consume.hpp>
#include <srcml.h>
#include <parse_request.hpp>
#include <write_queue.hpp>
#include <boost/static_assert.hpp>
#include <sha1utilities.hpp>

// creates initial unit, parses, and then sends unit to write queue
 void srcml_consume(ParseRequest* request, WriteQueue* write_queue) {

    // error passthrough to output for proper output in trace
    if (request->status) {
        request->unit = 0;
        write_queue->schedule(request);
        return;
    }

    // NOTE: thread task cannot throw exception

    // construct and parse the unit
    srcml_unit* unit = 0;
    int status = SRCML_STATUS_OK;
    try {
        // create the unit start tag
        if (!(unit = srcml_create_unit(request->srcml_arch)))
            throw SRCML_STATUS_ERROR;

        // language attribute, required if from memory
        if ((status = srcml_unit_set_language(unit, request->language.c_str())) != SRCML_STATUS_OK)
            throw status;

        // (optional) directory attribute
        if (request->directory && ((status = srcml_unit_set_directory(unit, request->filename->c_str())) != SRCML_STATUS_OK))
            throw status;

        // (optional) filename attribute
        if (request->filename && ((status = srcml_unit_set_filename(unit, request->filename->c_str())) != SRCML_STATUS_OK))
            throw status;

        // (optional) version attribute
        if (request->version && ((status = srcml_unit_set_version(unit, request->filename->c_str())) != SRCML_STATUS_OK))
            throw status;

        // sha1 attribute, if archive or if option on
        // sha1 value based on the code as encoded (source text encoding) in the original file
        unsigned char md[SHA_DIGEST_LENGTH];
        if (SHA1((const unsigned char*)&request->buffer.front(), (SHA_LONG)request->buffer.size(), md) == 0)
            throw SRCML_STATUS_ERROR;
        const char outmd[] = { HEXCHARASCII(md), '\0' };
        BOOST_STATIC_ASSERT_MSG(sizeof(outmd)/sizeof(outmd[0]) == (SHA_DIGEST_LENGTH * 2 + 1),
            "Wrong size for SHA_DIGEST_LENGTH conversion");
        //srcml_unit_set_hash(unit, outmd);

        // parse the buffer/file
        status = request->disk_filename ?
                                    srcml_parse_unit_filename(unit, request->disk_filename->c_str()) :
                                    srcml_parse_unit_memory(unit, &request->buffer.front(), request->buffer.size());
        if (status != SRCML_STATUS_OK)
            throw status;

    } catch (int status) {
        std::cerr << "Error in constructing srcml\n";
        if (unit)
            srcml_free_unit(unit);
        unit = 0;
        if (request)
            delete request;
        request = 0;
        return;
    }

    // schedule unit for output
    request->unit = unit;
    request->status = status;
    write_queue->schedule(request);
}
