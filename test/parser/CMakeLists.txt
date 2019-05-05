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

configure_file(runtest.py ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(python_test_bindings.py ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_subdirectory(testsuite)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/xsl)

file(GLOB XSL ./xsl/*)
foreach(xslfile ${XSL})
    configure_file(${xslfile} xsl COPYONLY)
endforeach()

add_custom_target(gentest COMMAND cd testsuite && make -f Makefile.gen)
add_custom_target(cleantest COMMAND cd testsuite && make -f Makefile.gen clean)

# Collection of targets to manage parser testing and parser test reports
add_custom_target(run COMMAND time ./runtest.py DEPENDS gentest)
add_custom_target(c COMMAND time ./runtest.py C DEPENDS gentest)
add_custom_target(java COMMAND time ./runtest.py Java DEPENDS gentest)
add_custom_target(cpp COMMAND time ./runtest.py C++ DEPENDS gentest)
add_custom_target(cs COMMAND time ./runtest.py CS DEPENDS gentest)
add_custom_target(cleanreport COMMAND rm srcMLTestReport*.txt)
add_custom_target(purge COMMAND ls -t srcMLTestReport*.txt | sed '1d' | xargs rm)
add_custom_target(report COMMAND ls -t srcMLTestReport*.txt | head -1 | xargs cat)

set(CMAKE_CXX_STANDARD 11)

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

add_executable(runtest runtest.cpp)
target_include_directories(runtest PRIVATE ${CMAKE_SOURCE_DIR}/src/libsrcml ${LibArchive_INCLUDE_DIRS})
target_link_libraries(runtest srcml_shared ${SRCML_LIBRARIES} ${LibArchive_LIBRARIES})

