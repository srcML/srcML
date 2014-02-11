/*
  srcml_consume.cpp

  Copyright (C) 2014  SDML (www.srcML.org)

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
  srcml_consume calls appropriate libsrcml functions for processing srcml
    or source file data respectively 
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

boost::mutex mtx;

// Public consumption thread function
void srcml_consume(ParseQueue* queue, WriteQueue* wqueue) {

  while (true) {
    ParseRequest pr;
    queue->pop(pr);
    
    // Check if termination queue item has been found  
    if (pr.empty())
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
    wqueue->push(wr);
  }
}

void xml_consume(ParseQueue* queue) {

  while (true) {
    ParseRequest pr;
    queue->pop(pr);
    
    // Check if termination queue item has been found  
    if (pr.empty())
      break;

    // Parse srcml back to source (srcml2src)
    srcml_archive* arch = srcml_create_archive();
    srcml_read_open_filename(arch, pr.filename.c_str());
    srcml_unit* unit;

    while (true) {
      unit = srcml_read_unit(arch);
      
      if (unit == 0)
        break;
      
      srcml_unparse_unit_filename(unit, srcml_unit_get_filename(unit));
      srcml_free_unit(unit);
    }

    srcml_close_archive(arch);
    srcml_free_archive(arch);
  }
}
