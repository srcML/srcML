##
# @file CMakeLists.txt
#
# @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
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
include_directories(src/srcml src/libsrcml src/parser src/translator src/SAX2Framework)

# Setting major and minor version numbers (TODO fix this so that it is set using something
# more correct that being input from within the build system).
add_definitions(-DVERSION=\"1\")
add_definitions(-DREVISION=\"1\")

get_property(SVN_ENABLED GLOBAL PROPERTY SVN_ENABLED)
if(${SVN_ENABLED})
    add_definitions(-DSVN)
endif()


get_property(SAX2_ENABLED GLOBAL PROPERTY SAX2_ENABLED)
if(${SAX2_ENABLED})
    add_definitions(-DSAX2)
endif()

# Including extra directories.
subdirs(src)

