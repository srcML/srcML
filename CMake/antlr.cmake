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

# 
# RunAntlr
# Executes ANTLR executable on the provided files.
# 
# Takes 3 parameters OUTPUT_FILES INPUT_FILES DEPENDENCIES and 
#    1 optional parameter INCLUDE_GRAMMAR
# Use with named arguments.
#
macro(RunAntlr OUTPUT_FILES INPUT_FILES DEPENDENCIES INCLUDE_GRAMMAR)
	    message("HERE OUTPUT: " ${OUTPUT_FILES})
	    message("HERE  INPUT: " ${INPUT_FILES})
	    message("HERE  DEPEN: " ${DEPENDENCIES})
	    message("HERE  INCLU: " ${INCLUDE_GRAMMAR})
    	    message("\n")

	    add_custom_command(OUTPUT  ${OUTPUT_FILES}
            COMMAND ${ANTLR_EXE} -o \"${CMAKE_CURRENT_SOURCE_DIR}\" -glib \"${INCLUDE_GRAMMAR}\" ${INPUT_FILES} DEPENDS ${INPUT_FILES} ${DEPENDENCIES} ${INCLUDE_GRAMMAR}
            COMMAND touch ${OUTPUT_FILES}
        )
endmacro(RunAntlr)