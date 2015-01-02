#!/usr/bin/python
#
# @file srcml_sort_archive.py
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




# Sorts an archive in alphabetical filename order.


import srcml

if __name__ == "__main__":
    iarchive = None
    oarchive = None
    num_units = 0
    units = []

    inputfile = "project.xml"
    outputfile = "project_tmp.xml"

    # open up an existing archive 
    iarchive = srcml.create_archive()

    # create a new srcml archive structure 
    # options and attributes of cloned archive start the same as
    #   the original archive 
    oarchive = srcml.clone_archive(iarchive)
    srcml.read_open_filename(iarchive, inputfile)

    unit = srcml.read_unit(iarchive)
    while unit != None:
        units.append(unit)
        unit = srcml.read_unit(iarchive)

    units.sort(key=lambda x:srcml.unit_get_filename(x) )
    
    # open a srcML archive for output 
    srcml.write_open_filename(oarchive, outputfile)

    for u in units:    
        # copy the files from the input archive to the output archive 
        # Translate to srcml and append to the archive 
        srcml.write_unit(oarchive, u)
        srcml.free_unit(u)

    # close the archives 
    srcml.close_archive(oarchive)
    srcml.close_archive(iarchive)

    # free the srcML archive data 
    srcml.free_archive(oarchive)
    srcml.free_archive(iarchive)
