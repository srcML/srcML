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
    string(REGEX MATCH "Fedora" OS_NAME ${REDHAT_RELEASE})

elseif(EXISTS /etc/os-release)

    file(READ /etc/os-release OS_RELEASE)
    string(REGEX MATCH "^NAME=([a-zA-Z]*)" OS_NAME ${OS_RELEASE})
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

# Turn ON/OFF tests
option(BUILD_PARSER_TESTS "Include tests for parser" ON)
option(BUILD_CLIENT_TESTS "Build srcml client tests" ON)
option(BUILD_UNIT_TESTS "Build unit tests for libsrcml" OFF)

option(BUILD_EXAMPLES "Build examples usage files for libsrcml" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python language bindings/wrapper" OFF)

if(${BUILD_PYTHON_BINDINGS})
    set(BUILD_LANGUAGE_BINDINGS true)
else()
    set(BUILD_LANGUAGE_BINDINGS false)
endif()

# Setting some windows only properties.
# @todo this breaks mingw32 build.
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
    
    if(ENABLE_SVN_INTEGRATION)
        message(FATAL_ERROR "SVN integration not tested on windows.")
    endif()
    
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
    find_package(Boost COMPONENTS program_options filesystem system thread date_time REQUIRED)

else()

    set(WINDOWS_DEP_PATH "")
    
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

    if (EXISTS /usr/local/opt/libarchive)
        set(LibArchive_INCLUDE_DIRS /usr/local/opt/libarchive/include)
        set(LibArchive_LIBRARIES /usr/local/opt/libarchive/lib/libarchive.dylib)
    else()
        find_package(LibArchive 3 REQUIRED)
    endif()

    # Locating packages.e
    find_package(LibXml2 REQUIRED)
    find_package(CURL REQUIRED)
    find_package(Iconv REQUIRED)
    set(Boost_NO_BOOST_CMAKE ON)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost COMPONENTS program_options filesystem system date_time REQUIRED)

    # add include directories
    include_directories(${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS} ${ICONV_INCLUDE_DIRS})

    if(DYNAMIC_LOAD_ENABLED)
        find_package(LibXslt)
    else()
        find_package(LibXslt REQUIRED)
    endif()

    if(LIBXSLT_FOUND)
        include_directories(${LIBXSLT_INCLUDE_DIR})
        add_definitions(-DWITH_LIBXSLT)
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

if(EXISTS ${Boost_INCLUDE_DIR}/boost/mpl/vector/vector150_c.hpp)
    add_definitions(-DSRCML_BOOST_MPL_LARGE)
endif()

# The default configuration is to compile in DEBUG mode. These flags can be directly
# overridden by setting the property of a target you wish to change them for.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif(NOT CMAKE_BUILD_TYPE)

if(${CMAKE_COMPILER_IS_GNUCXX})

    string(FIND ${CMAKE_CXX_COMPILER} "mingw32" IS_MINGW32)
    if(IS_MINGW32 EQUAL -1)
      set(USE_FPIC -fPIC)
    endif()

    set(GCC_WARNINGS "-Wno-long-long -Wall -Wextra  -Wall -pedantic -Wempty-body -Wignored-qualifiers -Wsign-compare -Wtype-limits -Wuninitialized -Wno-pragmas -Wno-variadic-macros")
    # Adding global compiler definitions.                                                                                      
    set(CMAKE_CXX_FLAGS "${USE_FPIC} --std=c++0x ${GCC_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DSTATIC_GLOBALS")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG --coverage -fprofile-arcs")

elseif("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xClang")
    # Configuring the Clang compiler
    set(CLANG_WARNINGS "-Wno-long-long -Wall -Wextra -Wshorten-64-to-32 -Wno-unknown-pragmas -Wno-int-to-void-pointer-cast")
    set(CMAKE_CXX_FLAGS "-fPIC --std=c++11 ${CLANG_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DSTATIC_GLOBALS")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG")
    
elseif("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xIntel")
    message(FATAL_ERROR "Configuration Not Implemented: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
    
elseif("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC")
    # message(STATUS "MSVC Compiler not completely configured yet")
    set(MSVC_WARNINGS "/W3 /wd4068 /wd4101 /D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${MSVC_WARNINGS} /DSTATIC_GLOBALS  /Od /ZI /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_WARNINGS} /Ox")
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
endif()
