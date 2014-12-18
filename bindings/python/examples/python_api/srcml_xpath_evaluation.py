#!/usr/bin/python
#
# @file srcml_xpath_evaluation.py
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



#  Example program using the python API.
#
#       Build an archive, by writing a unit into it, save the
#       content of the archive within a memory buffer.
#       
#       Reopen the archive for reading using the memory buffer.
#       
#       Register an XPath with the XSLT transformations.
#
#       Apply the XPath to the current archive.
#
#       Display the contents of the resulting archive.

import srcml, os, StringIO

if __name__ == "__main__":

    # Using the srcml memory buffer 
    #   The memory buffer type is used to handle memory allocated by srcml.
    mem_buff = srcml.memory_buffer()

    # Creates an archive and opens it for writing.
    archive = srcml.archive()
    archive.open_write(buff=mem_buff)

    # Create a unit, parses parse data.cpp and writes it into
    # the current archive.
    unit = archive.create_unit()
    unit.parse(filename="data.cpp")
    archive.write_unit(unit)

    # Close the archive
    archive.close()


    # Reopen the new archive that's currently stored within mem_buff
    archive.open_read(buff=mem_buff)

    # Registering our transformation with the XSLT data
    # from the archive
    #   The srcml.xpath("//src:expr") is a reusable object that can
    #   be appended to multiple different transformations if desired.
    archive.xslt.transform.append( srcml.xpath("//src:expr") )

    # Because srcML is meant to handle extremely large amounts of software
    #  the results of a transformation are stored in another archive.
    xpath_results_buffer = srcml.memory_buffer()

    # Create an archive to store the result of our XPath query.
    xpath_results_archive = srcml.archive()
    xpath_results_archive.open_write(buff=xpath_results_buffer)


    # Applying transformations 
    archive.xslt.apply(xpath_results_archive)
    archive.close()
    xpath_results_archive.close()

    # Printing the Contest of the archive that contains the
    # Results of the XPath.
    print str(xpath_results_buffer)
