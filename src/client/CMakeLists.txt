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


file(GLOB SRCML_HEADERS *.hpp)
file(GLOB SRCML_SOURCE *.cpp)
list(REMOVE_ITEM SRCML_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/srcml.cpp)

srcMLExec(srcml "srcml.cpp;${SRCML_HEADERS};${SRCML_SOURCE}" srcml_shared ${SRCML_LIBRARIES})

if(APPLE)							     
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\" \"-u\" \"-r\" \"${CMAKE_BINARY_DIR}/bin/srcml\")")
elseif(NOT WIN32)
    install(CODE "execute_process(COMMAND \"/usr/bin/strip\" \"${CMAKE_BINARY_DIR}/bin/srcml\")")
endif()

install(TARGETS srcml RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
