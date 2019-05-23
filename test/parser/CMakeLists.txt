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
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

set(TESTSUITE *.xml)

# targets to manage parser testing
add_custom_target(run_parser_tests COMMENT "Run all parser test cases"
                         COMMAND /bin/bash -c 'time srcml --parser-test ${TESTSUITE}'
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(c      COMMENT "Run all C parser test cases"
                         COMMAND /bin/bash -c 'time srcml --parser-test --language=C    ${TESTSUITE}'
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(cpp    COMMENT "Run all C++ parser test cases"
                         COMMAND /bin/bash -c 'time srcml --parser-test --language=C++  ${TESTSUITE}'
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(C++    COMMENT "Run all C++ parser test cases"
                         COMMAND /bin/bash -c 'time srcml --parser-test --language=C++  ${TESTSUITE}'
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(java   COMMENT "Run all Java parser test cases"
                         COMMAND /bin/bash -c 'time srcml --parser-test --language=Java ${TESTSUITE}'
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(cs     COMMENT "Run all C\# language parser test cases"
                         COMMAND /bin/bash -c 'time srcml --parser-test --language=C\#  ${TESTSUITE}'
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(CSHARP COMMENT "Run all C\# language parser test cases"
                         COMMAND /bin/bash -c 'time srcml --parser-test --language=C\#  ${TESTSUITE}'
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)

add_subdirectory(testsuite)
