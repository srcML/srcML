/*
  srcml_write.cpp

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
  srcml_write calls appropriate libsrcml functions for processing srcml
    or source file data respectively 
*/

#include <srcml_write.hpp>
#include <srcml.h>
#include <thread_queue.hpp>
#include <parse_request.hpp>
#include <src_input_libarchive.hpp>
#include <thread_oqueue.hpp>

// Public consumption thread function
void srcml_write(WriteQueue* queue) {

  while (true) {

    WriteRequest pr;
    queue->pop(pr);

    // Check if termination queue item has been found  
    if (!pr.position)
      break;

    srcml_write_unit(pr.srcml_arch, pr.unit);
  }
}
