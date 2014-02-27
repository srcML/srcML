##
# @file testing.cmake
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
# Testing macros/functions and additional variables. These functions will also allow
# for the specification of additional resource locations.
#

#
# addUnitTest
# Creates a unit test from a given file with a given name.
# - TEST_NAME the name of the test.
# - FILE_NAME the name of the unit test file.
# All arguments after the file name are considered to be linker arguments.
# By default all tests are linked against the srcml_static library.
#
#
macro(addUnitTest TEST_NAME FILE_NAME)
    add_executable(${TEST_NAME} ${FILE_NAME})
    target_link_libraries(${TEST_NAME} srcml_static ${ARGN})
    add_test(${TEST_NAME} ${TEST_NAME})
endmacro()


# 
# copyDependentFile 
# This copies a file given as TARGET_NAME to the current bunary directory
# of the build.
# 
macro(copyDependentFile TARGET_NAME)
    file(COPY ${TARGET_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endmacro()

# 
# Process string list of files
# This takes a list of files and creates the generated names for each file.
# The string list is not a list but a space separated string of files.
# 
macro(stringListTofileNames INPUT FILE_SUFFIX OUTPUT)
    string(REPLACE " " ";" OUTPUT_LIST ${INPUT})
    set(outputTemp "")
    foreach(temp ${OUTPUT_LIST})
        set(outputTemp "${outputTemp}${temp}${FILE_SUFFIX} ")
    endforeach()
    set(${OUTPUT} ${outputTemp})
endmacro()


# 
# Removes the file extension from a current file, this is used while generating files
# duing testing when we need to convert a file from for example .cpp.xml to .cpp.
# 
# NOTICE: This makes the assumption that there IS a file extension present it 
# will fail if there isn't.
# 
# 
macro(stripFileExt FILE_NAME OUTPUT)
    string(FIND ${FILE_NAME} "." LAST_POINT REVERSE)
    string(SUBSTRING ${FILE_NAME} 0 ${LAST_POINT} ${OUTPUT})
endmacro()

# 
# Locates the first dot and creates a sub string from
# upto that point in the provided FILE_NAME string
# 
macro(stripAllFileExts FILE_NAME OUTPUT)
    string(FIND ${FILE_NAME} "." LAST_POINT)
    string(SUBSTRING ${FILE_NAME} 0 ${LAST_POINT} ${OUTPUT})
endmacro()


# 
# appendFileExtToList
# Takes a list of files as the extra arguments and appends a suffix to each one
# and stores them in OUTPUT.
# 
macro(appendFileExtToList FILE_SUFFIX OUTPUT)
    set(${OUTPUT} "")
    foreach(temp ${ARGN})
        list(APPEND ${OUTPUT} ${temp}${FILE_SUFFIX} )
    endforeach()
endmacro()

# 
# Creates a temporary target which is used 
# for debugging the build system.
# 
# 
macro(tempTarget TARGET)
    string(REPLACE ";" " " DISPLAYED_ARGN "${ARGN}")
    add_custom_target(${TARGET}
        COMMAND echo "Build ${TARGET}: dpenedencies: ${DISPLAYED_ARGN}"
        DEPENDS ${ARGN}
    )
endmacro()


# 
# This function assists with the creation of dependencies with targets being built
# by other functions within this file. This allows individual targets to be invoked
# with make using the file name as the target name.
# 
macro(createProbDep PROBLEM_TYPE LANGUAGE_EXT)
    add_custom_target(problem.${PROBLEM_TYPE}.${LANGUAGE_EXT}.xml
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/problem.${PROBLEM_TYPE}.${LANGUAGE_EXT}.xml
        COMMAND echo "Built problem.${PROBLEM_TYPE}.${LANGUAGE_EXT}.xml"
    )    
endmacro()

#
# Creates a list of unique items with a given name.
#
macro(createUniqueList OUTPUT)
    set(${OUTPUT} "")
    foreach(ITEM ${ARGN})
        list(REMOVE_ITEM ${OUTPUT} ${ITEM})
        list(APPEND ${OUTPUT} ${ITEM})
    endforeach()
endmacro()

# 
# Check to see if an element is a match for one of the elements within
# the list given to ARGN and returns true if so otherwise false.
# 
macro(matchesOneInList OUTPUT INPUT)
    set(${OUTPUT} OFF)
    foreach(ELEMENT ${ARGN})
        if(${INPUT} MATCHES ${ELEMENT})
            set(${OUTPUT} ON)
            break()
        endif()
    endforeach()
endmacro()
