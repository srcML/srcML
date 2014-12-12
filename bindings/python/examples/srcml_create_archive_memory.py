#!/usr/bin/python
#
# @file srcml_create_archive_memory.py
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
 



# Create an archive, file by file, with an output memory.
import srcml, sys, ctypes

if __name__ == "__main__":
    archive = None
    unit = None
    bufferContent = ctypes.c_char_p()
    bufferSize = ctypes.c_int()
    # create a new srcml archive structure
    archive = srcml.create_archive()

    # open a srcML archive for output
    srcml.write_open_memory(archive, bufferContent, bufferSize)

    # add all the files to the archive#/
    for arg in sys.argv[1:]:

        unit = srcml.create_unit(archive)

        # Translate to srcml and append to the archive
        fileContent = "".join(open(arg, "r").readlines())
        srcml.unit_set_language(unit, srcml.archive_check_extension(archive, arg))

        srcml.parse_unit_memory(unit, fileContent, len(fileContent))

        # Translate to srcml and append to the archive
        srcml.write_unit(archive, unit)

        srcml.free_unit(unit)

    # close the srcML archive
    srcml.close_archive(archive)

    # free the srcML archive data
    srcml.free_archive(archive);

    print bufferContent.value
