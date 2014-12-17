#!/usr/bin/python
#
# @file srcml_direct_language_xml.c
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



# Example program of the use of the C API for srcML.

# A straightforward translation from the srcML format back to source code.
# Translates the srcML file "a.cpp.xml" to the source-code file "a.cpp", however
# in this case the language is specified:

# * This creates a single-unit srcML file, i.e., a non-archive srcML
# * The srcML attribute filename will be the name of the file passed as the first
# parameter.



import srcml

if __name__ == "__main__":

    # Translate from a source-code file to a srcML file
    srcml.set_language(srcml.LANGUAGE_XML);
    srcml.srcml("a.cpp.xml", "a.cpp");
