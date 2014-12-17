#!/usr/bin/python
#
# @file srcml_copy_archive.py
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
 


# Example program that uses the python API.
# Copy an archive.


import sys, srcml

if __name__ == "__main__":
    # int i;
    iarchive = None
    oarchive = None
    unit = None

     # open up an existing archive 
    iarchive = srcml.create_archive()
    srcml.read_open_filename(iarchive, "main.cpp.xml")

    # create a new srcml archive structure 
    # options and attributes of cloned archive start the same as
    #   the original archive 
    oarchive = srcml.clone_archive(iarchive)

    # open a srcML archive for output 
    srcml.write_open_filename(oarchive, "cloned_archive.xml")

    # copy the files from the input archive to the output archive 
    unit = srcml.read_unit(iarchive)
    while unit != None:
        # Translate to srcml and append to the archive 
        srcml.write_unit(oarchive, unit)

        srcml.free_unit(unit)
        unit = srcml.read_unit(iarchive)

    # close the archives 
    srcml.close_archive(oarchive)
    srcml.close_archive(iarchive)

    # free the srcML archive data 
    srcml.free_archive(oarchive)
    srcml.free_archive(iarchive)
