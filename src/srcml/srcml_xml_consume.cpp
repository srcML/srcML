/*
  srcml_consume.cpp

  @copyright @copyright Copyright (C) 2014  SDML (www.srcML.org)

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
  srcml_xml_consume calls appropriate libsrcml functions for processing srcml file data
*/

#include <parse_queue.hpp>

// Convert srcml back to src on disk
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
