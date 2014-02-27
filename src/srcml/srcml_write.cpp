/*
  srcml_write.cpp

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
  srcml_write calls appropriate libsrcml functions for processing srcml
  or source file data respectively
*/

#include <srcml_write.hpp>
#include <srcml.h>
#include <write_queue.hpp>
#include <iostream>

// Public consumption thread function
void srcml_write(WriteQueue* queue) {

    WriteRequest pr;
    while (true) {

        // write request in the queue
        queue->pop(pr);

        // check if done
        if (!pr.position)
            break;

        // write the unit
        srcml_write_unit(pr.srcml_arch, pr.unit);

        //    std::cerr << std::setw(5) << pr.position << " " << pr.filename << '\n';

        // free the unit
        srcml_free_unit(pr.unit);
    }
}
