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

set(TESTSUITE ${CMAKE_CURRENT_BINARY_DIR}/testsuite)

# targets to manage parser testing
add_custom_target(run    COMMENT "Runs all parser test cases"
                         COMMAND time srcml --parser-test ${CMAKE_CURRENT_BINARY_DIR}/testsuite VERBATIM)
add_custom_target(c      COMMENT "Runs all C parser test cases"
                         COMMAND time srcml --parser-test --language=C    ${TESTSUITE} VERBATIM)
add_custom_target(cpp    COMMENT "Runs all C++ parser test cases"
                         COMMAND time srcml --parser-test --language=C++  ${TESTSUITE} VERBATIM)
add_custom_target(C++    COMMENT "Runs all C++ parser test cases"
                         COMMAND time srcml --parser-test --language=C++  ${TESTSUITE} VERBATIM)
add_custom_target(java   COMMENT "Runs all C++ parser test cases"
                         COMMAND time srcml --parser-test --language=Java ${TESTSUITE} VERBATIM)
add_custom_target(cs     COMMENT "Runs all C language parser test cases"
                         COMMAND time srcml --parser-test --language=C\#  ${TESTSUITE} VERBATIM)
add_custom_target(CSHARP COMMENT "Runs all C language parser test cases"
                         COMMAND time srcml --parser-test --language=C\#  ${TESTSUITE} VERBATIM)

add_subdirectory(testsuite)
