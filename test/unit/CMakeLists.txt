#
#    CMakeLists.txt
# @copyright
# 
# Copyright (C) 2013  SDML (www.srcML.org)
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

# cmake_minimum_required(VERSION 2.8)
# cmake_policy(SET CMP0015 NEW)
# 
# # set includes and link locations
find_package(LibXml2)
if(LIBXML2_FOUND)
    include_directories(${LIBXML2_INCLUDE_DIR})
endif()



macro(add_unit_test TEST_NAME FILE_NAME)
    add_executable(${TEST_NAME} ${FILE_NAME})
    target_link_libraries(${TEST_NAME} srcml ${ARGN})
    add_test(${TEST_NAME} ${TEST_NAME})
endmacro(add_unit_test)

include_directories(../../src/parser ../../src/translator ../../src/libsrcml)
link_directories(../../bin)
# 
# # set compile and linker flags
# add_definitions(-g)
# add_definitions(--coverage)
# add_definitions(-O0)
# set(CMAKE_EXE_LINKER_FLAGS -fprofile-arcs)

# ctest
#include(CTest)

set(CTEST_DROP_SITE_CDASH FALSE)
set(MEMORYCHECK_COMMAND /usr/local/bin/valgrind)
set(COVERAGE_COMMAND /usr/bin/gcov)


add_unit_test(test_Language test_Language.cpp)
add_unit_test(test_Options test_Options.cpp)
add_unit_test(test_State test_State.cpp)
add_unit_test(test_UTF8CharBuffer test_UTF8CharBuffer.cpp ${LIBXML2_LIBRARIES})
add_unit_test(test_srcMLState test_srcMLState.cpp)
add_unit_test(test_srcMLStateStack test_srcMLStateStack.cpp)
add_unit_test(test_srcMLTranslatorCore test_srcMLTranslatorCore.cpp ${LIBXML2_LIBRARIES})

