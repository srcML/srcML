/*
  srcml_consume.cpp

  @copyright Copyright (C) 2014 SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  srcml_consume calls appropriate libsrcml functions for processing source file data
*/

#include <srcml_consume.hpp>
#include <srcml.h>
#include <thread_queue.hpp>
#include <parse_request.hpp>
#include <src_input_libarchive.hpp>
#include <boost/thread.hpp>
#include <parse_queue.hpp>
#include <write_request.hpp>
#include <write_queue.hpp>

// Public consumption thread function
void srcml_consume(ParseQueue* queue, WriteQueue* wqueue) {

    while (true) {
        ParseRequest pr;
        queue->pop(pr);

        // Check if termination queue item has been found
        if (pr.position == 0)
            break;

        // build and parse
        srcml_unit * unit = srcml_create_unit(pr.srcml_arch);
        srcml_unit_set_filename(unit, pr.filename.c_str());
        srcml_unit_set_language(unit, pr.lang.c_str());
        srcml_parse_unit_memory(unit, &pr.buffer[0], pr.buffer.size());

        // write unit
        WriteRequest wr;
        wr.srcml_arch = pr.srcml_arch;
        wr.unit = unit;
        wr.position = pr.position;
        wr.filename = pr.filename;
        wqueue->push(wr);
    }
}
