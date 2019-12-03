/**
 * @file test_srcml_archive_check_extension.cpp
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

  Test cases for srcml_archive_check_extension
*/
#include <srcml.h>

#include <dassert.hpp>

int main(int, char* argv[]) {

    /*
      srcml_check_extension
    */

    srcml_archive* archive = srcml_archive_create();
    srcml_archive_register_file_extension(archive, "h", "C++");

    {
        dassert(srcml_archive_check_extension(archive, "a.h"), std::string("C++"));
    }

    {
        dassert(srcml_archive_check_extension(archive, "a.h.gz"), std::string("C++"));
    }

    {
        dassert(srcml_archive_check_extension(archive, "a.foo"), 0);
    }

    {
        dassert(srcml_archive_check_extension(archive, 0), 0);
    }

    {
        dassert(srcml_archive_check_extension(0, "a.h"), 0);
    }

    srcml_archive_free(archive);

    return 0;
}
