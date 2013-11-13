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

subdirs(parser translator libsrcml srcml oldclient)

get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
get_property(LIBXML2_LIBS GLOBAL PROPERTY LIBXML2_LIBS)
get_property(LIBXSLT_LIBS GLOBAL PROPERTY LIBXSLT_LIBS)
get_property(BOOST_PROGRAM_OPTIONS_LIB GLOBAL PROPERTY BOOST_PROGRAM_OPTIONS_LIB)
get_property(ANTLR_LIB GLOBAL PROPERTY ANTLR_LIB)
get_property(LIBARCHIVE_LIBS  GLOBAL PROPERTY LIBARCHIVE_LIBS)

# 
# buildLib
#  Used to help with the creation of the srcML library.
#  - LIB_NAME is the name of the library and target
#  - LIB_TYPE is either STATIC or SHARED.
# 
macro(buildLib LIB_NAME LIB_TYPE)
    add_library(${LIB_NAME} ${LIB_TYPE} $<TARGET_OBJECTS:parser> $<TARGET_OBJECTS:translator> $<TARGET_OBJECTS:libsrcml> srcml/srcmlapps.cpp)
    set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME srcml)
    target_link_libraries(${LIB_NAME} ${LIBXML2_LIBS} ${LIBXSLT_LIBS} ${BOOST_PROGRAM_OPTIONS_LIB} ${ANTLR_LIB} ${LIBARCHIVE_LIBS} dl)
endmacro()

# Building static and dynamic libraries for srcML.
buildLib(srcml_static STATIC)
set_target_properties(srcml_static PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

buildLib(srcml_shared SHARED)
set_target_properties(srcml_shared PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
