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


# if(LIBSRCML_SAX2_ENABLED)
#     file(GLOB LIBSRCML_SRC src/libsrcml/*.cpp)
#     add_definitions(-DSAX2)
# else()
#     file(GLOB LIBSRCML_SRC src/libsrcml/srcml_*.cpp src/libsrcml/libsrcml.cpp src/libsrcml/srcMLReader.cpp)
# endif()
get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
if(${SAX2_ENABLED})
    # TODO: Figure out what needs to be added/excluded in the event that the "SAX2" is enabled/disabled.
    MESSAGE(WARNING "FIXME: Need to be told which headers/source files are NOT part of the \"SAX2\" interface. Only assuming that the current SAX2 stuff begins with SAX2 in the name of the file.")
endif()

file(GLOB LIBSRCML_HEADERS *.hpp)
file(GLOB LIBSRCML_SOURCE *.cpp)
list(REMOVE_ITEM LIBSRCML_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/srcMLReaderHandler.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/srcMLSAX2Reader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/srcml_archive.hpp
)

list(REMOVE_ITEM LIBSRCML_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/srcMLSAX2Reader.cpp)
add_library(libsrcml STATIC ${LIBSRCML_HEADERS} ${LIBSRCML_SOURCE})

