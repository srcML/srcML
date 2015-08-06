##
# @file doc.cmake
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

option(GENERATE_DOCUMENTATION "Enables the generation of documentation during the build and installation process." off)
if(${GENERATE_DOCUMENTATION})
    find_program(RONN_EXE REQUIRED NAMES ronn ronn.exe PATHS /bin /usr/bin ${WINDOWS_DEP_PATH}/bin)
    find_package(Doxygen REQUIRED)
    configure_file("${CMAKE_SOURCE_DIR}/doc/config" "${CMAKE_BINARY_DIR}/config")
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
    # Custom commands for creating things using the ronn executable.
    # add_custom_command(OUTPUT srcml.md
    #     COMMAND 
    #     DEPENDS srcml_input.md)
    # add_custom_command(OUTPUT srcml srcml.html 

    #     RONN_EXE 

    ADD_CUSTOM_TARGET(doc ALL DEPENDS ${DOXYGEN_OUTPUT_DIR}/html ${DOXYGEN_OUTPUT_DIR}/latex)
    install(FILES ${CMAKE_BINARY_DIR}/doc/srcml DESTINATION man)
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html ${DOXYGEN_OUTPUT_DIR}/latex DESTINATION doc)

endif()

