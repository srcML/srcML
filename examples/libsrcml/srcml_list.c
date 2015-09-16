/**
 * @file srcml_list.c
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

  Gather info from an archive from a given unit.
*/

#include <stdio.h>
#include "srcml.h"

int main(int argc, char* argv[]) {

    struct srcml_archive * archive;
    struct srcml_unit * unit;

    archive = srcml_archive_create();
    srcml_archive_read_open_filename(archive, "project.xml");

    while((unit = srcml_archive_read_unit_header(archive))) {

        puts(srcml_unit_get_filename(unit));
        srcml_unit_free(unit);

    }


    srcml_archive_close(archive);
    srcml_archive_free(archive);

    return 0;
}
