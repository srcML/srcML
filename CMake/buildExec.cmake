##
# @file buildExec.cmake
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
# This is the function used for building exeucables for srcML.
# This outputs the executable into the bin directory at the top level.
# - EXEC_NAME is the name of the target and the name out of the output by default.
# - EXEC_FILE is the name of the file containing main.
#
# - All subsequent arguments are linked as libraries.
#
macro(srcMLExec EXEC_NAME EXEC_FILE)
    message("HERE: ${EXEC_NAME}")
    add_executable(${EXEC_NAME} ${EXEC_FILE})
    target_link_libraries(${EXEC_NAME} ${ARGN})
    if("x${CMAKE_CXX_COMPILER_ID}" STREQUAL "xMSVC")
		set_target_properties(${EXEC_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
        add_custom_command(TARGET srcml POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/deps/${BUILD_ARCH}/$<CONFIGURATION>/bin"
        $<TARGET_FILE_DIR:srcml>)
    elseif(APPLE)
        # Making the exported_symbols_list an empty file reduces size of executable
        # @TODO Does this work for other UNIX platforms?
        set_target_properties(${EXEC_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin LINK_FLAGS "-exported_symbols_list /dev/null")
    elseif(WIN32)
        set_target_properties(${EXEC_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin LINK_FLAGS "-Wl,--allow-multiple-definition")
    else()
        set_target_properties(${EXEC_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    endif()
endmacro()
