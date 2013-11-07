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

#
#    The CMake project file for srcML.
#
cmake_minimum_required(VERSION 2.8)
project(srcML)
enable_testing()
# Setting up Project includes.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
include (srcml)

# Setting up source include directories.
include_directories(src/srcml src/libsrcml src/parser src/translator)

# Reading Major and minor version numbers and adding them as definitions.

# add_definitions(-DVERSION=\"${VERSION_NUMBER}\")
add_definitions(-DVERSION=\"1\")
add_definitions(-DREVISION=\"1\")

#MESSAGE(STATUS "${VERSION} ${REVISION}")
# This is being moved to be more compiler independent.
add_definitions(-fPIC)

# Adding global compiler definitions.
# This allows for compilation of a re-locatable execuatable on GCC I need to be sure that I
# can make this portable to compilers other than GCC.
if(CMAKE_COMPILER_IS_GNUXX)
    add_definitions(-fPIC)
endif()
get_property(SVN_ENABLED GLOBAL PROPERTY SVN_ENABLED)
if(${SVN_ENABLED})
    add_definitions(-DSVN)
endif()

get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
if(${SAX2_ENABLED})
    add_definitions(-DSAX2)
endif()


# Including extra directories.
subdirs(src test timing)

# Remove this later.
MESSAGE(STATUS "")
MESSAGE(STATUS "")
MESSAGE(STATUS "----------- Testing global properties --------------")

get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
get_property(LIBXML2_LIBS GLOBAL PROPERTY LIBXML2_LIBS)
get_property(LIBXSLT_LIBS GLOBAL PROPERTY LIBXSLT_LIBS)
get_property(BOOST_PROGRAM_OPTIONS_LIB GLOBAL PROPERTY BOOST_PROGRAM_OPTIONS_LIB)
get_property(WINDOWS_DEP_PATH GLOBAL PROPERTY WINDOWS_DEP_PATH)
get_property(ANTLR_LIB GLOBAL PROPERTY ANTLR_LIB)
get_property(ANTLR_EXE GLOBAL PROPERTY ANTLR_EXE)
get_property(SED_EXE GLOBAL PROPERTY SED_EXE)
get_property(GREP_EXE GLOBAL PROPERTY GREP_EXE)
get_property(LIBARCHIVE_LIBS  GLOBAL PROPERTY LIBARCHIVE_LIBS)

#  Printing global property values.
MESSAGE(STATUS "SVN_ENABLED ${SVN_ENABLED}")
MESSAGE(STATUS "SAX2_ENABLED ${SAX2_ENABLED}")
MESSAGE(STATUS "LIBXML2_LIBS: ${LIBXML2_LIBS}")
MESSAGE(STATUS "LIBXSLT_LIBS: ${LIBXSLT_LIBS}")
MESSAGE(STATUS "BOOST_PROGRAM_OPTIONS_LIB: ${BOOST_PROGRAM_OPTIONS_LIB}")
MESSAGE(STATUS "WINDOWS_DEP_PATH: ${WINDOWS_DEP_PATH}")
MESSAGE(STATUS "ANTLR_LIB: ${ANTLR_LIB}")
MESSAGE(STATUS "ANTLR_EXE: ${ANTLR_EXE}")
MESSAGE(STATUS "SED_EXE: ${SED_EXE}")
MESSAGE(STATUS "GREP_EXE: ${GREP_EXE}")
MESSAGE(STATUS "LIBARCHIVE_LIBS: ${LIBARCHIVE_LIBS}")
MESSAGE(STATUS "")
MESSAGE(STATUS "")




