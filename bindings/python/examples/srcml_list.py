#!/usr/bin/python
#
# @file srcml_list.c
#
# @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
#
# The srcML Toolkit is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# The srcML Toolkit is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with the srcML Toolkit; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


#
#  Example program of the use of the C API for srcML.
#
#  Gather info from an archive from a given unit.

import srcml

if __name__ == "__main__":
    archive = None
    unit = None

    archive = srcml.create_archive()
    srcml.read_open_filename(archive, "project.xml")
    unit = srcml.read_unit_header(archive)
    while unit != None:
        print srcml.unit_get_filename(unit)
        srcml.free_unit(unit)
        unit = srcml.read_unit_header(archive)


    srcml.close_archive(archive)
    srcml.free_archive(archive)
