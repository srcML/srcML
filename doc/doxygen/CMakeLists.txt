# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for doxygen documentation

find_package(Doxygen REQUIRED)

set(DOXYGEN_TEMPLATE  "${CMAKE_SOURCE_DIR}/doc/doxygen/config.in" CACHE INTERNAL "Doxygen template file")
set(DOXYGEN_INPUT     "${CMAKE_BINARY_DIR}/doc/doxygen/config" CACHE INTERNAL "Doxygen input file")

configure_file(${DOXYGEN_TEMPLATE} ${DOXYGEN_INPUT})
add_custom_command(
    OUTPUT  ${CMAKE_BINARY_DIR}/html
    COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
#    COMMAND ${CMAKE_COMMAND} -P configure_doxygen.cmake
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
    COMMAND ${CMAKE_COMMAND} -E echo "Done."
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS ${DOXYGEN_TEMPLATE} "${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h"
)

add_custom_target(doc_doxygen ALL DEPENDS ${CMAKE_BINARY_DIR}/html)

install(DIRECTORY ${CMAKE_BINARY_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT SRCMLDEV)
