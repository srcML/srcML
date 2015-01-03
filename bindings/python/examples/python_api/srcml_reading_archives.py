#!/usr/bin/python
#
# @file srcml_reading_archives.py
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



#  Example program using the python API to open an archive for reading and
#       to read individual units from that archive.


import srcml, os

if __name__ == "__main__":

    # using the srcml convenience function to create a quick archive
    # from a single file to use within the example.
    srcml.srcml("data.cpp", "srcml_reading_archives.cpp.xml")


    # Create an archive to read from
    archive = srcml.archive()

    # Open archive using a filename
    archive.open_read(filename="srcml_reading_archives.cpp.xml")

    # Read each unit from the archive and print it's associated
    #   file name and XML content.
    for u in archive.units():
        print u.filename
        print u.xml()

    # Close archive when finished.
    archive.close()

    # Cleaning up file used for test.
    os.remove("srcml_reading_archives.cpp.xml")