##
# @file packageConfig.cmake
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

##
#
#
# CMake directives and config for CPack.

# set package name
set(CPACK_PACKAGE_NAME "srcML") 

# set package name
#set(CPACK_PACKAGE_FILE_NAME "srcML-${CMAKE_SYSTEM_NAME}")

# set executables
set(CPACK_PACKAGE_EXECUTABLES srcml;srcML)

# set Desktop links
set(CPACK_CREATE_DESKTOP_LINKS srcml)

# Generator settings
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CPACK_PACKAGE_FILE_NAME "srcML-Win")
    # set(CPACK_GENERATOR "ZIP")
    # set(CPACK_BINARY_NS
else()
    set(CPACK_PACKAGE_FILE_NAME "srcML")
    set(CPACK_GENERATOR "TGZ;TBZ2")
endif()
 
# set summary
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "srcML Toolkit")

# description
#set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/doc/src2srcml.xml)

# vendor
set(CPACK_PACKAGE_VENDOR "srcML, LLC.")

# contact
set(CPACK_PACKAGE_CONTACT "Software Developement Laboratories <bugs@srcML.org>")
 
# package version
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "9")
set(CPACK_PACKAGE_VERSION_PATCH "5")

# license
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING.txt)

# README
configure_file(${CMAKE_SOURCE_DIR}/README.md ${CMAKE_BINARY_DIR}/README.txt COPYONLY)
set(CPACK_RESOURCE_FILE_README ${CMAKE_BINARY_DIR}/README.txt)

# welcome message
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/CMake/welcome.txt)

if(WIN32)
    install(FILES ${CMAKE_SOURCE_DIR}/CMake/srcml_icon.ico DESTINATION .)
else()
    # icon
    set(CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/CMake/srcml_icon.ico)
endif()

# strip executables
#set(CPACK_STRIP_FILES ON)
 
# set output directory
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

# include other specific configurations
include(debian)
include(rpm)
include(packageMaker)
include(nsis)

# needs to be last so not overwritten
include(CPack)
