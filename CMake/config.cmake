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

option(RUN_TIMING_TESTS "Run timing tests with ctest" OFF)
option(BUILD_UNIT_TESTS "Build unit tests for srcML/libsrcml" OFF)
option(BUILD_CLI_TESTS "Build cli tests" OFF)
option(BUILD_LARGE_SYSTEMS_TESTS "Build tests run on large systems" OFF)
option(BUILD_EXAMPLES "Build examples usage files for libsrcml" OFF)
option(BUILD_PYTHON_BINDINGS "Build Python language bindings/wrapper" OFF)

if(${BUILD_PYTHON_BINDINGS})
    set(BUILD_LANGUAGE_BINDINGS true)
else()
    set(BUILD_LANGUAGE_BINDINGS false)
endif()

# Setting some windows only properties.
# @todo this breaks mingw32 build.
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-DWITH_LIBXSLT)
    # Adding suspected windows include directory for ANTRL
    include_directories("C:/antlr/277/include/antlr")
    set(WINDOWS_DEP_PATH ${PROJECT_SOURCE_DIR}/dep)
    include_directories(${WINDOWS_DEP_PATH}/include)
    link_directories(${WINDOWS_DEP_PATH}/lib)
    if(ENABLE_SVN_INTEGRATION)
        message(FATAL_ERROR "SVN integration not tested on windows.")
    endif()
    # FIXME
    set(LIBXSLT_LIBRARIES libxslt.lib)
    set(LIBXSLT_EXSLT_LIBRARY libexslt.lib)
    set(LibArchive_LIBRARIES archive.lib)
    set(LIBXML2_LIBRARIES libxml2.lib iconv.lib)
    set(CURL_LIBRARIES libcurl.lib)
    include_directories(C:/antlr/277/include)
    set(BOOST_DIR $ENV{BOOST_ROOT})
    include_directories(${BOOST_DIR})
    link_directories(${BOOST_DIR}/stage/lib)
else()

    set(WINDOWS_DEP_PATH "")
    
    # Locating packages.
    find_package(LibArchive REQUIRED)
    find_package(LibXml2 REQUIRED)
    find_package(CURL REQUIRED)
    set(Boost_NO_BOOST_CMAKE ON)
    set(Boost_USE_STATIC_LIBS ON)
    find_package(Boost COMPONENTS program_options filesystem system thread regex date_time REQUIRED)

    # add include directories
    include_directories(${LibArchive_INCLUDE_DIRS} ${LIBXML2_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

    if(DYNAMIC_LOAD_ENABLED)
        find_package(LibXslt)
    else()
        find_package(LibXslt REQUIRED)
    endif()

    if(LIBXSLT_FOUND)
        include_directories(${LIBXSLT_INCLUDE_DIR})
        add_definitions(-DWITH_LIBXSLT)
    endif()


endif()

# Locating the antlr library.
find_library(ANTLR_LIBRARY NAMES libantlr-pic.a libantlr.a libantlr2-0.dll antlr.lib PATHS /usr/lib /usr/local/lib ${WINDOWS_DEP_PATH}/lib)

if(DYNAMIC_LOAD_ENABLED)
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} dl crypto pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
elseif(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" AND NOT WIN32)
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} crypto pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
elseif(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${ANTLR_LIBRARY} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} crypto pthread
                CACHE INTERNAL "Libraries needed to build libsrcml")
else()
    set(LIBSRCML_LIBRARIES ${LIBXML2_LIBRARIES} ${LIBXSLT_LIBRARIES} ${LIBXSLT_EXSLT_LIBRARY} ${Boost_LIBRARIES} ${ANTLR_LIBRARY}
                CACHE INTERNAL "Libraries needed to build libsrcml")
endif()


if(NOT WIN32 AND NOT APPLE)
list(APPEND LIBSRCML_LIBRARIES rt)
endif()

if(NOT WIN32)
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} crypto pthread CACHE INTERNAL "Libraries needed to build srcml")
elseif(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} ws2_32 crypto CACHE INTERNAL "Libraries needed to build srcml")
else()
    set(SRCML_LIBRARIES ${LibArchive_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} ws2_32 ${LIBSRCML_LIBRARIES} CACHE INTERNAL "Libraries needed to build srcml")
endif()


# Finding antlr library.
find_program(ANTLR_EXE NAMES antlr runantlr cantlr antlr2 antlr.bat PATHS /usr/bin /opt/local/bin /usr/local/bin C:/antlr/277/bin)

# Finding SED
find_program(SED_EXE NAMES gsed sed PATHS /opt/local/bin /usr/local /bin ${WINDOWS_DEP_PATH}/bin)

# Finding GREP
find_program(GREP_EXE grep PATHS /bin /usr/bin ${WINDOWS_DEP_PATH}/bin)

find_package(PythonInterp REQUIRED)

# Enforcing that the version of python being used must have a major version of 2.
# and the minor version be greater than version 6 (this means version 2.7 of python 
# version 2 or newer).
if(NOT ${PYTHON_VERSION_MAJOR} EQUAL "2")
    message(FATAL_ERROR "Version of python found is not 2.X.X")
    if(${PYTHON_VERSION_MINOR} LESS EQUAL 6)
        message(FATAL_ERROR "Version of python found is not 2.6.X")
    endif()
endif()

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

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # Configuring the Clang compiler
    set(CLANG_WARNINGS "-Wno-long-long -Wall -Wextra -Wshorten-64-to-32 -Wno-unknown-pragmas -Wno-int-to-void-pointer-cast")
    set(CMAKE_CXX_FLAGS "-fPIC --std=c++11 ${CLANG_WARNINGS}")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DSTATIC_GLOBALS")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -DDEBUG")
    
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    message(FATAL_ERROR "Configuration Not Implemented: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
    
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # message(STATUS "MSVC Compiler not completely configured yet")
    set(MSVC_WARNINGS "/W3 /wd4068 /wd4101 /D_CRT_SECURE_NO_WARNINGS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${MSVC_WARNINGS} /DSTATIC_GLOBALS  /Od /ZI /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_WARNINGS} /Ox")
else()
    message(FATAL_ERROR "Unknown compiler: ${CMAKE_CXX_COMPILER_ID}. Build not configured for selected compiler.")
endif()
