# @copyright
# 
# Copyright (C) 2013-2014  SDML (www.srcML.org)
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
# RunAntlr
# Executes ANTLR executable on the provided files.
# 
# Takes 3 parameters OUTPUT_FILES INPUT_FILES DEPENDENCIES and 
#    1 optional parameter INCLUDE_GRAMMAR
# Use with named arguments.
#
macro(RunAntlr OUTPUT_FILES INPUT_FILES DEPENDENCIES INCLUDE_GRAMMAR)
    if("${INCLUDE_GRAMMAR}" STREQUAL "")
        if(WIN32)
            # Handling windows relative path problem (Not sure why this is an issue but it's given me a headache).
            add_custom_command(OUTPUT  ${OUTPUT_FILES}
                COMMAND echo "${INPUT_FILES}"
                COMMAND echo "${OUTPUT_FILES}"
                COMMAND ${ANTLR_EXE} -o \"${CMAKE_CURRENT_SOURCE_DIR}\" \"${INPUT_FILES}\" DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
            )
            # set(cmdStr "${ANTLR_EXE} -o ${CMAKE_CURRENT_SOURCE_DIR} ${INPUT_FILES}")
            # string(LENGTH  ${cmdStr} cmdLength)
            # message(STATUS "${cmdStr}")
            # message(STATUS "Command String Length: ${cmdLength}")
        else()
            add_custom_command(OUTPUT  ${OUTPUT_FILES}
                COMMAND ${ANTLR_EXE} -o \"${CMAKE_CURRENT_SOURCE_DIR}\" ${INPUT_FILES} DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
                COMMAND touch ${OUTPUT_FILES}
            )
        endif()
    else()
        if(WIN32)
            # C:/antlr/277/bin/antlr.bat -o "C:/Users/bbart_000/Desktop/ProgrammingProjects/GitHub/srcML/src/parser" -glib "C:/Users/bbart_000/Desktop/ProgrammingProjects/GitHub/srcML/src/parser/TextLexer.g" "C:/Users/bbart_000/Desktop/ProgrammingProjects/GitHub/srcML/src/parser/OperatorLexer.g"
            add_custom_command(OUTPUT  ${OUTPUT_FILES}
                COMMAND echo "${INPUT_FILES}"
                COMMAND echo "${OUTPUT_FILES}"
                COMMAND echo "${INCLUDE_GRAMMAR}"
                COMMAND echo "${ANTLR_EXE} -o \"${CMAKE_CURRENT_SOURCE_DIR}\" -glib \"${INCLUDE_GRAMMAR}\" \"${INPUT_FILES}\""
                COMMAND ${ANTLR_EXE} -o \"${CMAKE_CURRENT_SOURCE_DIR}\" -glib \"${INCLUDE_GRAMMAR}\" \"${INPUT_FILES}\" DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
                COMMAND echo "Generated /w Dep: ${OUTPUT_FILES}"
            )
            # set(cmdStr "${ANTLR_EXE} -o ${CMAKE_CURRENT_SOURCE_DIR} -glib ${INCLUDE_GRAMMAR} ${INPUT_FILES}")
            # string(LENGTH  ${cmdStr} cmdLength)
            # message(STATUS "${cmdStr}")
            # message(STATUS "Command String Length: ${cmdLength}")
            #message(STATUS "Stuff Happened Here!")
        else()
            add_custom_command(OUTPUT  ${OUTPUT_FILES}
                COMMAND ${ANTLR_EXE} -o \"${CMAKE_CURRENT_SOURCE_DIR}\" -glib \"${INCLUDE_GRAMMAR}\" ${INPUT_FILES} DEPENDS ${INPUT_FILES} ${DEPENDENCIES}
                COMMAND touch ${OUTPUT_FILES}
            )
        endif()
    endif()
endmacro(RunAntlr)