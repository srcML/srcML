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

if(DYNAMIC_LOAD_ENABLED)
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} ${Iconv_LIBRARIES} dl pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC" AND NOT WIN32)
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} ${Iconv_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} ${Iconv_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
else()
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${Boost_LIBRARIES} ${ANTLR_LIBRARY}
                CACHE INTERNAL "Libraries needed to build libsrcml")
endif()

if(NOT WIN32 AND NOT APPLE)
    list(APPEND LIBSRCML_LIBRARIES rt crypto)
endif()

# Building static and dynamic libraries for srcML.
add_library(srcml_static STATIC $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:libsrcml>)
set_target_properties(srcml_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(srcml_static PRIVATE ${LIBSRCML_LIBRARIES})
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(srcml_static PRIVATE gcov)
endif()

add_library(srcml_shared SHARED $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:libsrcml>)
set_target_properties(srcml_shared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(srcml_shared PRIVATE ${LIBSRCML_LIBRARIES})
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(srcml_shared PRIVATE gcov)
endif()

# Set libsrcml version
file(STRINGS "${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h" SRCML_HEADER)
string(REGEX MATCH "SRCML_VERSION_STRING \"([0-9].[0-9].[0-9])\"" _ "${SRCML_HEADER}")
set(SRCML_VERSION ${CMAKE_MATCH_1})

set_target_properties(srcml_shared PROPERTIES VERSION "${SRCML_VERSION}" SOVERSION "${SRCML_VERSION}")

if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") AND NOT WIN32)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\"  \"-x\" \"$<TARGET_FILE:srcml_shared>\")")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_target_properties(srcml_shared srcml_static PROPERTIES OUTPUT_NAME libsrcml LINK_FLAGS_DEBUG "/SAFESEH:NO")
elseif(APPLE)
    set_target_properties(srcml_shared srcml_static PROPERTIES OUTPUT_NAME srcml LINK_FLAGS "-Wl,-exported_symbols_list ${CMAKE_SOURCE_DIR}/src/libsrcml/export_list" MACOSX_RPATH OFF)
else()
    set_target_properties(srcml_shared srcml_static PROPERTIES OUTPUT_NAME srcml)
endif()

# which types of libraries
install(TARGETS srcml_shared RUNTIME DESTINATION bin LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
if (BUILD_LIBSRCML_STATIC)
    install(TARGETS srcml_static RUNTIME DESTINATION bin LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if(APPLE)
    install(CODE "execute_process(COMMAND /usr/bin/install_name_tool -id /usr/local/lib/libsrcml.dylib /usr/local/lib/libsrcml.dylib)")
endif()

file(GLOB LIBSRCML_SOURCE *.h *.hpp *.cpp)

add_library(libsrcml OBJECT ${LIBSRCML_SOURCE})
target_include_directories(libsrcml PRIVATE . ${CMAKE_SOURCE_DIR}/src/parser ${CMAKE_GENERATED_SOURCE_DIR})
add_dependencies(libsrcml parser)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/srcml.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    file(GLOB SRCML_DLLS ${WINDOWS_DEP_PATH}/${BUILD_ARCH}/release/bin/*.dll)
    install(FILES ${SRCML_DLLS} DESTINATION bin)
endif()
