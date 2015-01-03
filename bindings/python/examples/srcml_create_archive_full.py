#!/usr/bin/python
#
#  @file srcml_create_archive_full.c
# 
#  @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
# 
#  The srcML Toolkit is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
# 
#  The srcML Toolkit is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with the srcML Toolkit; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 

# Create an archive, file by file, with an output filename, showing
# most of the option features

import sys, srcml

if __name__ == "__main__":
    archive = None

    
    # Setup archive
    

    # create a new srcml archive structure 
    archive = srcml.create_archive()

    # setup options and attributes 
    srcml.archive_set_version(archive, "211")
    srcml.archive_set_tabstop(archive, 4)

    # treat "*.h" as C++ 
    srcml.archive_register_file_extension(archive, "h", srcml.LANGUAGE_CXX)

     # change prefix of standard namespace 
    srcml.archive_register_namespace(archive, "s", "http://www.sdml.info/srcML/src")

    # default prefix is now for cpp namespace
    srcml.archive_register_namespace(archive, "", "http://www.sdml.info/srcML/cpp")

    # new prefix for further processing
    srcml.archive_register_namespace(archive, "doc", "http://www.sdml.info/srcML/doc")

    # Open and write to the archive


    # open a srcML archive for output
    srcml.write_open_filename(archive, "project.xml")

    # add all files on the command line to the archive
    for arg in sys.argv:

        # Setup this entry 
        unit = srcml_create_unit(archive)
        srcml.unit_set_language(unit, SRCML_LANGUAGE_C)
        srcml.unit_set_filename(unit, arg)

        # Translate the entry to srcML 
        srcml.parse_unit_filename(unit, arg)

        # Append unit to the archive 
        srcml.write_unit(archive, unit)

        # Done with the entry for now 
        srcml.free_unit(unit)

    # Clean up
    # close the srcML archive 
    srcml.close_archive(archive)

    # free the srcML archive data 
    srcml.free_archive(archive)
