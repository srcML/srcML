#!/usr/bin/python
#
# @file srcml_read_archive_memory.py
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
# Take an archive and extract the invidual units and write to a filesystem.
#


import srcml, ctypes

if __name__ == "__main__":

    language = ""
    filename = ""
    archive = None
    unit = None
    # int srcml_input;
    buffer = ctypes.c_char_p()
    size = ctypes.c_int()
    s ="""<?xml version="1.0" encoding="UTF-8" standalone="yes"?>
<unit xmlns="http://www.sdml.info/srcML/src">

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="a.cpp"><decl_stmt><decl><type><name>int</name></type> <name>a</name></decl>;</decl_stmt>"
</unit>

<unit xmlns:cpp="http://www.sdml.info/srcML/cpp" language="C++" filename="b.cpp"><decl_stmt><decl><type><name>int</name></type> <name>b</name></decl>;</decl_stmt>"
</unit>

</unit>
        """
    # create a new srcml archive structure 
    archive = srcml.create_archive()

    srcml.read_open_memory(archive, s, len(s))

    # add all the files to the archive 
    unit = srcml.read_unit(archive)
    while unit != None:

        # can inquire about the current unit 
        language = srcml.unit_get_language(unit)
        filename = srcml.unit_get_filename(unit)

        # uparse and write to a file 
        srcml.unparse_unit_memory(unit, buffer, size)

        print buffer.value
        # free((void *)buffer);
        srcml.free(buffer)

        srcml.free_unit(unit)
        unit = srcml.read_unit(archive)
    

    # close the srcML archive 
    srcml.close_archive(archive)

    # free the srcML archive data 
    srcml.free_archive(archive)
