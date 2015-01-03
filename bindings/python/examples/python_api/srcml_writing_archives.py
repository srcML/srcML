#!/usr/bin/python
#
# @file srcml_writing_archives.py
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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



#  Example program using the python API to create an archive within a python StringIO
#   stream parse data.cpp into srcml and store it in the archive.
#
#   Once complete, print the contents of the entire archive.



import srcml, os, StringIO

if __name__ == "__main__":

    # Create an archive to read from
    archive = srcml.archive()
    archive_data_stream = StringIO.StringIO()

    # Open an archive for writing using a custom stream
    #   the parameter close_stream=False means that the
    #   archive_data_stream will still be open for writing after 
    #   the archive is closed.
    archive.open_write(stream=archive_data_stream, close_stream=False)

    # Create a unit to parse data.cpp
    unit = archive.create_unit()

    # Parse the data using srcml.
    unit.parse(filename="data.cpp")

    # Write the unit into the archive.
    archive.write_unit(unit)

    # Close the archive
    archive.close()

    print archive_data_stream.getvalue()