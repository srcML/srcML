##
# @file config.cmake
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
# It is expected that the windows dependency directoy will contain a folder
# named dep which will contain all of the dependencies. For windows.
# 
# The layout of the folder will be as follows.
#       bin
#       include
#       lib

if(EXISTS /etc/redhat-release)

    file(READ /etc/redhat-release REDHAT_RELEASE)
    if ( REDHAT_RELEASE MATCHES "Fedora" )
        string(REGEX MATCH "Fedora" OS_NAME ${REDHAT_RELEASE})
    elseif( REDHAT_RELEASE MATCHES "CentOS" )
        string(REGEX MATCH "CentOS" OS_NAME ${REDHAT_RELEASE})
    endif()

elseif(EXISTS /etc/os-release)
    file(READ /etc/os-release OS_RELEASE)
    string(REGEX MATCH "^NAME=\"([a-zA-Z ]*)\"" OS_NAME ${OS_RELEASE})
    set(OS_NAME ${CMAKE_MATCH_1})

endif()

set(OS_NAME ${OS_NAME} CACHE INTERNAL "Detected OS name")

# Dynamic Load libraries (Unix only)
if(NOT WIN32)
    option(DYNAMIC_LOAD_ENABLED "Dynamically load some libraries such as libxslt and libexslt" ON)
endif()

if(NOT DYNAMIC_LOAD_ENABLED)
    add_definitions(-DNO_DLLOAD)
endif()


# Setting some windows only properties.
if("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC")

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

    # Locating packages.e
    find_package(LibXml2 REQUIRED)
    find_package(CURL REQUIRED)
    find_package(Iconv REQUIRED)
    set(Boost_NO_BOOST_CMAKE ON)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost COMPONENTS program_options REQUIRED)

    # add include directories
    include_directories(${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${ICONV_INCLUDE_DIRS} ${LibArchive_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${ICONV_INCLUDE_DIR})

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


# Finding antlr binary.
find_program(ANTLR_EXE NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin ${WINDOWS_DEP_PATH}/tools/antlr/277/bin)

find_package(PythonInterp REQUIRED)

# Enforcing that the version of python being used must have a major version of 2.
# and the minor version be greater than version 6 (this means version 2.7 of python 
# version 2 or newer).
#if(NOT ${PYTHON_VERSION_MAJOR} EQUAL "2")
#    message(FATAL_ERROR "Version of python found is not 2.X.X")
#    if(${PYTHON_VERSION_MINOR} LESS EQUAL 6)
#        message(FATAL_ERROR "Version of python found is not 2.6.X")
#    endif()
#endif()

# The default configuration is to compile in DEBUG mode. These flags can be directly
# overridden by setting the property of a target you wish to change them for.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

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
