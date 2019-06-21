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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

find_package(Doxygen REQUIRED)

set(DOXYGEN_INPUT "${CMAKE_SOURCE_DIR}/doc/doxygen/config")
add_custom_command(
    OUTPUT  ${CMAKE_BINARY_DIR}/html
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
    COMMAND ${CMAKE_COMMAND} -E echo "Done."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${DOXYGEN_INPUT} "${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h"
)

add_custom_target(doc_doxygen ALL DEPENDS ${CMAKE_BINARY_DIR}/html)

install(DIRECTORY ${CMAKE_BINARY_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT SRCMLDEV)
