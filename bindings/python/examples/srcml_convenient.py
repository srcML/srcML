#!/usr/bin/python
#
# @file srcml_convenient.py
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

# Using the srcml convenience function. 

import srcml

if __name__ == "__main__":

    # setup options and attributes
    srcml.set_version("211")
    srcml.set_tabstop(4)

    # treat files with the .h file extension as C++
    srcml.register_file_extension("h", srcml.LANGUAGE_CXX)

    # change prefix of standard namespace
    srcml.register_namespace("s", "http://www.sdml.info/srcML/src")

    # default prefix is now for cpp namespace
    srcml.register_namespace("", "http://www.sdml.info/srcML/cpp")

    # new prefix for further processing
    srcml.register_namespace("doc", "http://www.sdml.info/srcML/doc")

    #  Translates source code file "main.cpp" to srcML file "main.cpp.xml".
    #  - Translate using the above global options
    #  - The language will be automatically based on the extension of the input (first) filename
    #  - Since there is only a single input file, the output file will be a non-archive by default.
    #  Convenience function can also convert to archive
    srcml.srcml("main.cpp", "main.cpp.xml")
