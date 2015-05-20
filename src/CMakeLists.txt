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

add_subdirectory(parser)
add_subdirectory(libsrcml)
add_subdirectory(srcml)

file(GLOB SRCSAX_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} srcSAX/*.hpp)
file(GLOB SRCSAX_SOURCE  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} srcSAX/*.cpp)
if(NOT WIN32)
	list(REMOVE_ITEM SRCSAX_SOURCE srcSAX/srctools_windows.cpp)
endif()

add_library(srcsax OBJECT ${SRCSAX_SOURCE} ${SRCSAX_HEADERS})

# buildLib
#  Used to help with the creation of the srcML library.
#  - LIB_NAME is the name of the library and target
#  - LIB_TYPE is either STATIC or SHARED.
# 
macro(buildLib LIB_NAME LIB_TYPE)

    add_library(${LIB_NAME} ${LIB_TYPE} $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:libsrcml> $<TARGET_OBJECTS:srcsax> build_hack.cpp)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME libsrcml LINK_FLAGS_DEBUG "/SAFESEH:NO")
elseif(APPLE)
    set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME srcml LINK_FLAGS "-Wl,-exported_symbols_list ${CMAKE_SOURCE_DIR}/CMake/export_list" MACOSX_RPATH OFF)
else()
    set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME srcml)

endif()

    target_link_libraries(${LIB_NAME} PRIVATE ${LIBSRCML_LIBRARIES})
endmacro()

# Building static and dynamic libraries for srcML.
buildLib(srcml_static STATIC)
set_target_properties(srcml_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

buildLib(srcml_shared SHARED)
set_target_properties(srcml_shared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(APPLE)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\"  \"-x\" \"${CMAKE_BINARY_DIR}/bin/libsrcml.dylib\")")
elseif(NOT ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC") AND NOT WIN32)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\"  \"-x\" \"${CMAKE_BINARY_DIR}/bin/libsrcml.so\")")
endif()

if(OS_NAME STREQUAL "Fedora" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	install(TARGETS srcml_shared srcml_static RUNTIME DESTINATION bin LIBRARY DESTINATION lib64 ARCHIVE DESTINATION lib64)
elseif(OS_NAME STREQUAL "openSUSE" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
	install(TARGETS srcml_shared srcml_static RUNTIME DESTINATION bin LIBRARY DESTINATION lib64 ARCHIVE DESTINATION lib64)
else()
	install(TARGETS srcml_shared srcml_static RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif()

