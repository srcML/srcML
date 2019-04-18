##
# @file CMakeLists.txt
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

file(GLOB PARSER_TESTS *.xml)

if(NOT ${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})

	configure_file(Makefile.gen ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	configure_file(suite.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

	file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/xsl)

	file(GLOB XSL ./xsl/*)

	foreach(xslfile ${XSL})
		configure_file(${xslfile} xsl COPYONLY)
	endforeach()

endif()
