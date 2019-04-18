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

find_package(Doxygen REQUIRED)
configure_file("${CMAKE_SOURCE_DIR}/doc/doxygen/config" "${CMAKE_BINARY_DIR}/config")
SET(DOXYGEN_INPUT "${CMAKE_BINARY_DIR}/config")
SET(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}")
ADD_CUSTOM_COMMAND(
    OUTPUT ${DOXYGEN_OUTPUT_DIR}/html ${DOXYGEN_OUTPUT_DIR}/latex
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
    COMMAND ${CMAKE_COMMAND} -E echo "Done."
    WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR}
    DEPENDS ${DOXYGEN_INPUT} ${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h
)

ADD_CUSTOM_TARGET(doc_doxygen ALL DEPENDS ${DOXYGEN_OUTPUT_DIR}/html ${DOXYGEN_OUTPUT_DIR}/latex)
install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html ${DOXYGEN_OUTPUT_DIR}/latex DESTINATION doc)
