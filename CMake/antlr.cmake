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

MESSAGE(STATUS "Fix file: antlr.cmake")


# RunAntlr
# Executes ANTLR executable on the provided files.
# 
# Takes 3 parameters OUTPUT_FILES INPUT_FILES DEPENDENCIES and 
#    1 optional parameter INCLUDE_GRAMMAR
# Use with named arguments.
#
get_property(ANTLR_EXEC GLOBAL PROPERTY ANTLR_EXE)
macro(RunAntlr)
    set(multiValueArgs OUTPUT_FILES INPUT_FILES DEPENDENCIES INCLUDE_GRAMMAR)
    cmake_parse_arguments(RunAntlr "" "" "${multiValueArgs}" ${ARGN})
    list(LENGTH RunAntlr_INCLUDE_GRAMMAR incGrammarLength)
    if(${incGrammarLength} GREATER 0)
        math(EXPR incGrammarLength "${incGrammarLength} - 1")
        set(glibs "")
        FOREACH(index RANGE 0 ${incGrammarLength})
            list(GET RunAntlr_INCLUDE_GRAMMAR ${index} temp)
            if(index LESS ${incGrammarLength})    
                set(glibs "${glibs}${temp}\;")
            else()
                set(glibs "${glibs}${temp}")
            endif()
        ENDFOREACH()
        add_custom_command(
            OUTPUT  ${RunAntlr_OUTPUT_FILES}
            COMMAND ${ANTLR_EXEC} -o \"${CMAKE_CURRENT_SOURCE_DIR}\" -glib \"${glibs}\" ${RunAntlr_INPUT_FILES} DEPENDS ${RunAntlr_INPUT_FILES} ${RunAntlr_DEPENDENCIES} ${RunAntlr_INCLUDE_GRAMMAR}
            COMMAND touch ${RunAntlr_OUTPUT_FILES}
        )
    else()
        add_custom_command(
            OUTPUT  ${RunAntlr_OUTPUT_FILES}
            COMMAND ${ANTLR_EXEC} -o \"${CMAKE_CURRENT_SOURCE_DIR}\" ${RunAntlr_INPUT_FILES} DEPENDS ${RunAntlr_INPUT_FILES} ${RunAntlr_DEPENDENCIES}
            COMMAND touch ${RunAntlr_OUTPUT_FILES}
        )
    endif()
endmacro(RunAntlr)