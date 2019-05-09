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

#
# This is the function used for building exeucables for srcML.
# This outputs the executable into the bin directory at the top level.
# - EXEC_NAME is the name of the target and the name out of the output by default.
# - EXEC_FILE is the name of the file containing main.
#
# - All subsequent arguments are linked as libraries.
#

set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES}
                CACHE INTERNAL "Libraries needed to build srcml")
if(NOT WIN32 AND APPLE)
    list(APPEND SRCML_LIBRARIES pthread)
elseif(NOT WIN32)
    list(APPEND SRCML_LIBRARIES crypto pthread)
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND SRCML_LIBRARIES ws2_32 crypto)
else()
    list(APPEND SRCML_LIBRARIES ws2_32 ${LIBSRCML_LIBRARIES})
endif()

file(GLOB CLIENT_SOURCE *.hpp *.cpp)

add_executable(srcml ${CLIENT_SOURCE})
target_include_directories(srcml PRIVATE . ${CMAKE_SOURCE_DIR}/src/libsrcml)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(srcml srcml_shared ${SRCML_LIBRARIES} gcov)
else()
    target_link_libraries(srcml srcml_shared ${SRCML_LIBRARIES})
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_target_properties(srcml PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    add_custom_command(TARGET srcml POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/deps/$<CONFIGURATION>/bin"
    $<TARGET_FILE_DIR:srcml>)
elseif(APPLE)
    # Making the exported_symbols_list an empty file reduces size of executable, as strip does not work
    set_target_properties(srcml PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin LINK_FLAGS "-exported_symbols_list /dev/null")
elseif(WIN32)
    # @BUILD Why this when there is a MSVC case above?
    set_target_properties(srcml PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin LINK_FLAGS "-Wl,--allow-multiple-definition")
else()
    set_target_properties(srcml PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

if(APPLE)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\" \"-u\" \"-r\" \"${CMAKE_BINARY_DIR}/bin/srcml\")")
elseif(NOT WIN32)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\" \"${CMAKE_BINARY_DIR}/bin/srcml\")")
endif()

install(TARGETS srcml RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
