#!/usr/bin/python
#
# @file srcml_split_archive.c
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


# Split an archive into two, one for .h files and one for other extensions


import srcml

if __name__ == "__main__":

    iarchive = None
    includearchive = None
    otherarchive = None
    unit = None

    # open up an existing archive 
    iarchive = srcml.create_archive()
    srcml.read_open_filename(iarchive, "project.xml")

    # create a new srcml archive structure 
    # options and attributes of cloned archive start the same as
    #   the original archive 
    includearchive = srcml.clone_archive(iarchive)
    otherarchive = srcml.clone_archive(iarchive)

    # open a srcML archive for output 
    srcml.write_open_filename(includearchive, "project_include.xml")
    srcml.write_open_filename(otherarchive, "project_other.xml")

    # copy the files from the input archive to the output archive 
    unit = srcml.read_unit(iarchive)
    while unit != None:
        # Get the filename 
        filename = srcml.unit_get_filename(unit)
        if cmp(".h", filename[len(filename) - 2:]) == 0:
            srcml.write_unit(includearchive, unit)
        else:
            srcml.write_unit(otherarchive, unit)
        srcml.free_unit(unit)
        unit = srcml.read_unit(iarchive)

    # close the archives 
    srcml.close_archive(includearchive)
    srcml.close_archive(otherarchive)
    srcml.close_archive(iarchive)

    # free the srcML archive data 
    srcml.free_archive(includearchive)
    srcml.free_archive(otherarchive)
    srcml.free_archive(iarchive)
