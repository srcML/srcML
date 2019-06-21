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

set(TESTSUITE *.xml)

set(FLAGS "")
if(WIN32)
     set(FLAGS "--no-color")
endif()

# targets to manage parser testing
add_custom_target(run_parser_tests        COMMENT "Run all parser test cases"
                         COMMAND srcml --parser-test ${FLAGS} ${TESTSUITE}
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(run_parser_tests_c      COMMENT "Run C parser test cases"
                         COMMAND srcml --parser-test ${FLAGS} --language=C    ${TESTSUITE}
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(run_parser_tests_cpp    COMMENT "Run C++ parser test cases"
                         COMMAND srcml --parser-test ${FLAGS} --language=C++  ${TESTSUITE}
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(run_parser_tests_C++    COMMENT "Run C++ parser test cases"
                         COMMAND srcml --parser-test ${FLAGS} --language=C++  ${TESTSUITE}
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(run_parser_tests_java   COMMENT "Run Java parser test cases"
                         COMMAND srcml --parser-test ${FLAGS} --language=Java ${TESTSUITE}
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(run_parser_tests_cs     COMMENT "Run C\# language parser test cases"
                         COMMAND srcml --parser-test ${FLAGS} --language=C\#  ${TESTSUITE}
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)
add_custom_target(run_parser_tests_CSHARP COMMENT "Run C\# language parser test cases"
                         COMMAND srcml --parser-test ${FLAGS} --language=C\#  ${TESTSUITE}
                         USES_TERMINAL
                         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testsuite)

add_subdirectory(testsuite)
