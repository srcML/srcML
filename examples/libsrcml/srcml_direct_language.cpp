/**
 * @file test_srcml_direct_language.cpp
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
  Example program of the use of the libsrcml C API.

  A straightforward translation of source code to the srcML format.
  Translates the file "a.cpp" to the srcML format in "a.cpp.xml", however
  in this case the language is specified:

  * This creates a single-unit srcML file, i.e., a non-archive srcML
  * The srcML attribute filename will be the name of the file passed as the first
  parameter.
*/

#include <srcml.h>

int main(int argc, char* argv[]) {

    // translate from a source-code file to a srcML file
    srcml_set_language(SRCML_LANGUAGE_CXX);
    srcml("a.cpp", "a.cpp.xml");

    return 0;
}