#
#    CMakeLists.txt
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

get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
if(${SAX2_ENABLED})
    add_definitions(-DSAX2)
endif()

file(GLOB LIBSRCML_HEADERS *.hpp)
file(GLOB LIBSRCML_SOURCE *.cpp)
if(NOT ${SAX2_ENABLED})
    list(REMOVE_ITEM LIBSRCML_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/srcMLReaderHandler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/srcMLSAX2Reader.hpp
    )
    list(REMOVE_ITEM LIBSRCML_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/srcMLSAX2Reader.cpp)

endif()

add_library(libsrcml OBJECT ${LIBSRCML_HEADERS} ${LIBSRCML_SOURCE})

