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

set(CMAKE_CXX_STANDARD 11)

# Needed to expand generator expressions in install()
# Should be default in cmake 3.14
cmake_policy(SET CMP0087 NEW)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    if ("${CMAKE_EXE_LINKER_FLAGS}" STREQUAL "/machine:X86")
        set(BUILD_ARCH "x86")
    else()
        set(BUILD_ARCH "x64")
    endif()

    add_definitions(-DWITH_LIBXSLT)
    set(WINDOWS_DEP_PATH ${PROJECT_SOURCE_DIR}/deps)

    include_directories(${WINDOWS_DEP_PATH}/include)
    link_directories(${WINDOWS_DEP_PATH}/${BUILD_ARCH}/$(ConfigurationName)/lib)
    
    set(LIBXSLT_LIBRARIES libxslt.lib)
    set(LIBXSLT_EXSLT_LIBRARY libexslt.lib)
    set(LibArchive_LIBRARIES archive.lib)
    set(LIBXML2_LIBRARIES libxml2.lib libiconv.lib)
    set(CURL_LIBRARIES libcurl.lib)
    set(ANTLR_LIBRARY antlr.lib)

    set(Boost_NO_SYSTEM_PATHS ON)
    set(Boost_LIBRARY_DIR_RELEASE ${WINDOWS_DEP_PATH}/${BUILD_ARCH}/release/lib)
    set(Boost_LIBRARY_DIR_DEBUG ${WINDOWS_DEP_PATH}/${BUILD_ARCH}/debug/lib)
    set(BOOST_INCLUDE_DIR ${WINDOWS_DEP_PATH}/include)
    set(BOOST_INCLUDEDIR ${WINDOWS_DEP_PATH}/include)
    find_package(Boost COMPONENTS program_options REQUIRED)

else()

    set(WINDOWS_DEP_PATH "")
    
    set(OSX_LIBARCHIVE_PATH ${CMAKE_SOURCE_DIR}/../libarchive)

    # libarchive 3 is necessary
    if(NOT APPLE)
        find_package(LibArchive 3 REQUIRED)

     # macOS with custom-built libarchive
    elseif(EXISTS "${OSX_LIBARCHIVE_PATH}")

        # different versions of libarchive place the generated libarchive.a in different places
        find_library(LIBARCHIVE_LOCAL_LIBRARY NAMES libarchive.a PATHS ${OSX_LIBARCHIVE_PATH} PATH_SUFFIXES libarchive /.libs NO_DEFAULT_PATH)

        set(LibArchive_INCLUDE_DIRS ${OSX_LIBARCHIVE_PATH}/libarchive)

        set(LibArchive_LIBRARIES  ${LIBARCHIVE_LOCAL_LIBRARY} /usr/lib/liblzma.dylib /usr/lib/libbz2.dylib /usr/lib/libcompression.dylib /usr/lib/libz.dylib /usr/lib/libxar.dylib /usr/lib/libiconv.dylib /usr/lib/libexpat.dylib)
    
    # macOS with homebrew
    elseif(EXISTS "/usr/local/opt/libarchive")
        set(LibArchive_INCLUDE_DIRS /usr/local/opt/libarchive/include)
        set(LibArchive_LIBRARIES /usr/local/opt/libarchive/lib/libarchive.a /usr/local/lib/liblzma.dylib /usr/lib/libbz2.dylib /usr/lib/libcompression.dylib /usr/lib/libz.dylib /usr/lib/libxar.dylib /usr/lib/libiconv.dylib /usr/lib/libexpat.dylib)

    else()
        message(FATAL_ERROR "Unable to find libarchive >= 3 via homebrew or local build at ${OSX_LIBARCHIVE_PATH}")
    endif()

    # Locating packages
    find_package(LibXml2 REQUIRED)
    find_package(CURL REQUIRED)
    find_package(Iconv REQUIRED)
    set(Boost_NO_BOOST_CMAKE ON)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost COMPONENTS program_options REQUIRED)

    # add include directories
    include_directories(${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${Iconv_INCLUDE_DIRS} ${LibArchive_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

    if(DYNAMIC_LOAD_ENABLED)
        find_package(LibXslt)
    else()
        find_package(LibXslt REQUIRED)
    endif()

    if(LIBXSLT_FOUND)
        include_directories(${LIBXSLT_INCLUDE_DIR})
        add_definitions(-DWITH_LIBXSLT)
    endif()

    # Helps with new path on default antlr2 install using homebrew on MacOS Mojave
    if(EXISTS /usr/local/opt/antlr@2 AND APPLE)
        list (APPEND CMAKE_PREFIX_PATH "/usr/local/opt/antlr@2")
        include_directories("/usr/local/opt/antlr@2/include")
    endif()

    # Locating the antlr library.
    find_library(ANTLR_LIBRARY NAMES libantlr-pic.a libantlr.a libantlr2-0.dll antlr.lib PATHS /usr/lib /usr/local/lib)

endif()

if(${CMAKE_COMPILER_IS_GNUCXX})

    set(USE_FPIC -fPIC)

    set(GCC_WARNINGS "-Wno-long-long -Wall -Wextra  -Wall -pedantic -Wempty-body -Wignored-qualifiers -Wsign-compare -Wtype-limits -Wuninitialized -Wno-pragmas -Wno-variadic-macros")
    # Adding global compiler definitions.                                                                                      
    set(CMAKE_CXX_FLAGS "${USE_FPIC} ${GCC_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG --coverage -fprofile-arcs")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # Configuring the Clang compiler
    set(CLANG_WARNINGS "-Wno-long-long -Wall -Wextra -Wshorten-64-to-32 -Wno-unknown-pragmas -Wno-int-to-void-pointer-cast")
    set(CMAKE_CXX_FLAGS "-fPIC ${CLANG_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG")
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    message(FATAL_ERROR "Configuration Not Implemented: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
    
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # message(STATUS "MSVC Compiler not completely configured yet")
    set(MSVC_WARNINGS "/W3 /wd4068 /wd4101 /D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${MSVC_WARNINGS} /Od /ZI /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_WARNINGS} /Ox")
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
endif()

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

if(NOT WIN32 AND APPLE)
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} pthread CACHE INTERNAL "Libraries needed to build srcml")
elseif(NOT WIN32)
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} crypto pthread CACHE INTERNAL "Libraries needed to build srcml")
elseif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
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

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
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

if(NOT (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") AND NOT WIN32)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\"  \"-x\" \"$<TARGET_FILE:srcml_shared>\")")
endif()

include(GNUInstallDirs)

set(LIBDIR ${CMAKE_INSTALL_LIBDIR})

# which types of libraries
install(TARGETS srcml_shared RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBDIR} ARCHIVE DESTINATION ${LIBDIR})
if (BUILD_LIBSRCML_STATIC)
    install(TARGETS srcml_static RUNTIME DESTINATION bin LIBRARY DESTINATION ${LIBDIR} ARCHIVE DESTINATION ${LIBDIR})
endif()


if(APPLE)
    install(CODE "execute_process(COMMAND /usr/bin/install_name_tool -id /usr/local/lib/libsrcml.dylib /usr/local/lib/libsrcml.dylib)")
endif()
