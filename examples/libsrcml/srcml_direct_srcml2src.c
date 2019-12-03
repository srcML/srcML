/**
 * @file srcml_direct_srcml2src.c
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

/*
  Example program of the use of the C API for srcML.

  A straightforward translation from the srcML format back to source code.
  Translates the srcML file "a.cpp.xml" to the source-code file "a.cpp":

  * This creates a single-unit srcML file, i.e., a non-archive srcML
  * The srcML attribute filename will be the name of the file passed as the first
  parameter.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    /* Translate from a srcML file to a source-code file */
    srcml("a.cpp.xml", "a.cpp");

    return 0;
}
