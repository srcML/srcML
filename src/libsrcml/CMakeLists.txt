##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
# CMake files for the library libsrcml

# strip executables
# Note: Does not seem to have an effect, so post-build strip is added
set(CPACK_STRIP_FILES ON)

if(DYNAMIC_LOAD_ENABLED)
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${ANTLR_LIBRARY} ${Iconv_LIBRARIES} dl pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${ANTLR_LIBRARY} ${Iconv_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
else()
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${ANTLR_LIBRARY} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} 
                CACHE INTERNAL "Libraries needed to build libsrcml")
endif()

if(NOT WIN32 AND NOT APPLE)
    list(APPEND LIBSRCML_LIBRARIES rt crypto)
endif()

include_directories(BEFORE . ${CMAKE_SOURCE_DIR}/src/parser ${CMAKE_INSTALL_INCLUDEDIR})

# Building static and dynamic libraries for srcML.
add_library(libsrcml_static STATIC $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:libsrcml>)
set_target_properties(libsrcml_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_include_directories(libsrcml_static BEFORE PRIVATE . ${CMAKE_SOURCE_DIR}/src/parser ${CMAKE_GENERATED_SOURCE_DIR})
target_link_libraries(libsrcml_static PRIVATE ${LIBSRCML_LIBRARIES})
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(libsrcml_static PRIVATE gcov)
endif()

add_library(libsrcml_shared SHARED $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:libsrcml>)
set_target_properties(libsrcml_shared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_include_directories(libsrcml_shared BEFORE PRIVATE . ${CMAKE_SOURCE_DIR}/src/parser ${CMAKE_GENERATED_SOURCE_DIR})
target_link_libraries(libsrcml_shared PRIVATE ${LIBSRCML_LIBRARIES})
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_COMPILER_IS_GNUCXX)
    target_link_libraries(libsrcml_shared PRIVATE gcov)
endif()

set_target_properties(libsrcml_shared PROPERTIES VERSION "${PROJECT_VERSION}" SOVERSION "${PROJECT_VERSION_MAJOR}")

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set_target_properties(libsrcml_shared libsrcml_static PROPERTIES OUTPUT_NAME libsrcml LINK_FLAGS_DEBUG "/SAFESEH:NO")
elseif(APPLE)
    set_target_properties(libsrcml_shared libsrcml_static PROPERTIES OUTPUT_NAME srcml LINK_FLAGS "-Wl,-exported_symbols_list ${CMAKE_SOURCE_DIR}/src/libsrcml/export_list" MACOSX_RPATH OFF)
else()
    set_target_properties(libsrcml_shared libsrcml_static PROPERTIES OUTPUT_NAME srcml)
endif()

# which types of libraries
install(TARGETS libsrcml_shared LIBRARY COMPONENT SRCML NAMELINK_COMPONENT DEVLIBS)
if (BUILD_LIBSRCML_STATIC)
    install(TARGETS libsrcml_static COMPONENT DEVLIBS)
endif()

if(LINK_LIBSRCML_STATIC AND BUILD_LIBSRCML_STATIC)
    add_library(libsrcml_link ALIAS libsrcml_static)
else()
    add_library(libsrcml_link ALIAS libsrcml_shared)
endif()

if(APPLE)
#    add_custom_command(TARGET libsrcml_shared POST_BUILD COMMAND /usr/bin/install_name_tool -id /usr/local/lib/libsrcml.dylib /usr/local/lib/libsrcml.dylib)
endif()

file(GLOB LIBSRCML_SOURCE *.h *.hpp *.cpp)

add_library(libsrcml OBJECT ${LIBSRCML_SOURCE})
target_include_directories(libsrcml BEFORE PRIVATE . ${CMAKE_SOURCE_DIR}/src/parser ${CMAKE_GENERATED_SOURCE_DIR})
add_dependencies(libsrcml parser)

# install the libsrcml include file
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/srcml.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} COMPONENT INCLUDE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    file(GLOB SRCML_DLLS ${WINDOWS_DEP_PATH}/release/bin/*.dll)
    install(FILES ${SRCML_DLLS} DESTINATION bin)
endif()

if(NOT APPLE)
    set_target_properties(libsrcml_shared PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

if(APPLE)
    add_custom_command(TARGET libsrcml_static POST_BUILD
        COMMAND /usr/bin/strip -ur ${CMAKE_BINARY_DIR}/bin/libsrcml.a
        COMMAND ${CMAKE_COMMAND} -E echo "-- Stripping bin/libsrcml.a")
elseif(NOT WIN32)
    add_custom_command(TARGET libsrcml_static POST_BUILD
        COMMAND /usr/bin/strip ${CMAKE_BINARY_DIR}/bin/libsrcml.a
        COMMAND ${CMAKE_COMMAND} -E echo "-- Stripping bin/libsrcml.a")
endif()
