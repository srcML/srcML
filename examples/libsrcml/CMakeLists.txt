# SPDX-License-Identifier: GPL-3.0-only
##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
# 
# CMake files for libsrcml examples

cmake_minimum_required(VERSION 3.24)
project(srcML-Examples-libsrcml)

if(SRCML_TEST_INSTALLED OR NOT TARGET srcML::LibsrcML)
    find_package(srcml REQUIRED)
endif()

# add examples
file(GLOB EXAMPLES "srcml_*.cpp")

install(FILES ${EXAMPLES} DESTINATION share/srcml/examples COMPONENT EXAMPLES)
install(FILES CMakeLists.examples.txt DESTINATION share/srcml/examples RENAME CMakeLists.txt COMPONENT EXAMPLES)
install(FILES decl_stmt.cpp expr.cpp define.cpp project.xml copy.xsl schema.rng DESTINATION share/srcml/examples COMPONENT EXAMPLES)

file(COPY decl_stmt.cpp expr.cpp define.cpp project.xml schema.rng copy.xsl DESTINATION "${CMAKE_BINARY_DIR}/bin")

if(BUILD_EXAMPLES OR PROJECT_IS_TOP_LEVEL)

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    foreach(EXAMPLE IN ITEMS ${EXAMPLES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
        add_executable(${EXAMPLE_NAME} ${EXAMPLE})
        target_link_libraries(${EXAMPLE_NAME} PRIVATE srcML::LibsrcML)
        set_target_properties(${EXAMPLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    endforeach()

endif()
