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

if(NOT DYNAMIC_LOAD_ENABLED)
    add_definitions(-DNO_DLLOAD)
endif()

if(${CMAKE_COMPILER_IS_GNUCXX})

    set(USE_FPIC -fPIC)

    set(GCC_WARNINGS "-Wno-long-long -Wall -Wextra  -Wall -pedantic -Wempty-body -Wignored-qualifiers -Wsign-compare -Wtype-limits -Wuninitialized -Wno-pragmas -Wno-variadic-macros")
    # Adding global compiler definitions.                                                                                      
    set(CMAKE_CXX_FLAGS "${USE_FPIC} --std=c++0x ${GCC_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG --coverage -fprofile-arcs")

elseif("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xClang")
    # Configuring the Clang compiler
    set(CLANG_WARNINGS "-Wno-long-long -Wall -Wextra -Wshorten-64-to-32 -Wno-unknown-pragmas -Wno-int-to-void-pointer-cast")
    set(CMAKE_CXX_FLAGS "-fPIC --std=c++11 ${CLANG_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG")
    
elseif("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xIntel")
    message(FATAL_ERROR "Configuration Not Implemented: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
    
elseif("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC")
    # message(STATUS "MSVC Compiler not completely configured yet")
    set(MSVC_WARNINGS "/W3 /wd4068 /wd4101 /D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${MSVC_WARNINGS} /Od /ZI /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_WARNINGS} /Ox")
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
endif()

if(DYNAMIC_LOAD_ENABLED)
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} ${ICONV_LIBRARIES} dl pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
elseif(NOT "x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC" AND NOT WIN32)
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} ${ICONV_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
elseif(NOT "x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC")
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} ${ICONV_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
else()
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${Boost_LIBRARIES} ${ANTLR_LIBRARY}
                CACHE INTERNAL "Libraries needed to build libsrcml")
endif()


if(NOT WIN32 AND NOT APPLE)
list(APPEND LIBSRCML_LIBRARIES rt crypto)
endif()

if(NOT WIN32 AND APPLE)
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} pthread CACHE INTERNAL "Libraries needed to build srcml")
elseif(NOT WIN32)
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} crypto pthread CACHE INTERNAL "Libraries needed to build srcml")
elseif(NOT "x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC")
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} ws2_32 crypto CACHE INTERNAL "Libraries needed to build srcml")
else()
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} ws2_32 ${LIBSRCML_LIBRARIES} CACHE INTERNAL "Libraries needed to build srcml")
endif()

add_subdirectory(parser)
add_subdirectory(libsrcml)
add_subdirectory(client)

# Building static and dynamic libraries for srcML.
add_library(srcml_static STATIC $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:libsrcml>)
set_target_properties(srcml_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(srcml_static PRIVATE ${LIBSRCML_LIBRARIES})

add_library(srcml_shared SHARED $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:libsrcml>)
set_target_properties(srcml_shared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(srcml_shared PRIVATE ${LIBSRCML_LIBRARIES})

if("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC")
    set_target_properties(srcml_shared srcml_static PROPERTIES OUTPUT_NAME libsrcml LINK_FLAGS_DEBUG "/SAFESEH:NO")
elseif(APPLE)
    set_target_properties(srcml_shared srcml_static PROPERTIES OUTPUT_NAME srcml LINK_FLAGS "-Wl,-exported_symbols_list ${CMAKE_SOURCE_DIR}/src/libsrcml/export_list" MACOSX_RPATH OFF)
else()
    set_target_properties(srcml_shared srcml_static PROPERTIES OUTPUT_NAME srcml)
endif()

# Set libsrcml version
file(STRINGS "${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h" SRCML_HEADER)
string(REGEX MATCH "SRCML_VERSION_STRING \"([0-9].[0-9].[0-9])\"" _ "${SRCML_HEADER}")
set(SRCML_VERSION ${CMAKE_MATCH_1})

set_target_properties(srcml_shared PROPERTIES VERSION "${SRCML_VERSION}" SOVERSION "${SRCML_VERSION}")

if(APPLE)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\"  \"-x\" \"${CMAKE_BINARY_DIR}/bin/libsrcml.dylib\")")
elseif(NOT ("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC") AND NOT WIN32)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\"  \"-x\" \"${CMAKE_BINARY_DIR}/bin/libsrcml.so\")")
endif()

set(LIBDIR "lib")

# openSUSE installs in lib64
if(OS_NAME STREQUAL "openSUSE" AND CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(LIBDIR "lib64")
endif()

# which types of libraries
if (BUILD_LIBSRCML_STATIC)
    set(LIBTYPES "srcml_shared srcml_static")
else()
    set(LIBTYPES "srcml_shared")
endif()

install(TARGETS ${LIBTYPES} RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBDIR} ARCHIVE DESTINATION ${LIBDIR})
