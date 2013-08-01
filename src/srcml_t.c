/*
  srcml_t.c

  Copyright (C) 2013  SDML (www.sdml.info)

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Test program for the syntax of the srcml.h functions
*/

#include "srcml.h"

int main() {

    /* Convert a single file to srcML */
    src2srcml("a.cpp", "a.cpp.xml");

    /* Convert a single srcML file back to a file */
    /* Note:  name of output file could come from srcML */
    srcml2src("a.cpp.xml", "a.cpp");

    /* Convert a single file to srcML */
    src2srcml_filename_filename("a.cpp", "a.cpp.xml", 0, 0, 0, 0);

    /* Convert a srcML file to a source code file */
    srcml2src_filename_filename("a.cpp.xml", "a.cpp", 0, 0, 0, 0);

    return 0;
}
