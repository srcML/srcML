##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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
# 
# CMake files for libsrcml examples

# add examples
file(GLOB EXAMPLES "srcml_*.c")

# install examples
install(FILES ${EXAMPLES} DESTINATION share/srcml/examples COMPONENT EXAMPLES)

# install input files
set(INPUT_FILES
    input/main.cpp
    input/main.cpp.xml
    input/project.xml
    input/f.cpp
    input/g.cpp
    input/h.cpp
)
install(FILES ${INPUT_FILES} DESTINATION share/srcml/examples/input COMPONENT EXAMPLES)

if(BUILD_EXAMPLES)

    set(CMAKE_C_STANDARD 99)

    # Create each example
    foreach(EXAMPLE ${EXAMPLES})
        get_filename_component(EXAMPLE_NAME ${EXAMPLE} NAME_WE)
        add_executable(${EXAMPLE_NAME} ${EXAMPLE})
        target_include_directories(${EXAMPLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/libsrcml)
        target_link_libraries(${EXAMPLE_NAME} libsrcml_link)
        set_target_properties(${EXAMPLE_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    endforeach()

    # Copy input files
    foreach(INPUT ${INPUT_FILES})
        configure_file(${INPUT} ${CMAKE_BINARY_DIR}/bin/${INPUT} COPYONLY)
    endforeach()

endif()
