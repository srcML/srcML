/**
 * @file srcml_direct_src2srcml.c
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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
 */

/*
  Example program of the use of the C API for srcML.

  A straightforward translation of source code to the srcML format.
  Translates the file "a.cpp" to the srcML format in "a.cpp.xml":

  * The language is determined automatically from the source file extension
  * This creates a single-unit srcML file, i.e., a non-archive srcML
  * The srcML attribute filename will be the name of the file passed as the first
  parameter.
*/

#include "srcml.h"

int main(int argc, char* argv[]) {

    /* Translate from a source-code file to a srcML file */
    int error = srcml("a.cpp", "a.cpp.xml");

    if(error) puts(srcml_error_string());

    return 0;
}
