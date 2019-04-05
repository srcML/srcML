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

file(COPY runtest.py python_test_bindings.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(testsuite)

add_custom_target(gentest COMMAND cd testsuite && make -f Makefile.gen)
add_custom_target(cleantest COMMAND cd testsuite && make -f Makefile.gen clean)

# Collection of targest to manage parser testing, and parser test reports
add_custom_target(run COMMAND time ./runtest.py DEPENDS gentest)
add_custom_target(c COMMAND time ./runtest.py C DEPENDS gentest)
add_custom_target(java COMMAND time ./runtest.py Java DEPENDS gentest)
add_custom_target(cpp COMMAND time ./runtest.py C++ DEPENDS gentest)
add_custom_target(cs COMMAND time ./runtest.py CS DEPENDS gentest)
add_custom_target(cleanreport COMMAND rm srcMLTestReport*.txt)
add_custom_target(purge COMMAND ls -t srcMLTestReport*.txt | sed '1d' | xargs rm)
add_custom_target(report COMMAND ls -t srcMLTestReport*.txt | head -1 | xargs cat)
