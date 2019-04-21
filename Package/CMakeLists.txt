##
# @file package.cmake
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

# package name
set(CPACK_PACKAGE_NAME "srcML") 

# executables and text label
set(CPACK_PACKAGE_EXECUTABLES srcml;srcML)

# desktop links
set(CPACK_CREATE_DESKTOP_LINKS srcml)

# Generator settings
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CPACK_PACKAGE_FILE_NAME "srcML-Win")
else()
    set(CPACK_PACKAGE_FILE_NAME "srcML")
    set(CPACK_GENERATOR "TGZ;TBZ2")
endif()
 
# summary
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "srcML Toolkit")

# description
#set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/doc/src2srcml.xml)

# vendor
set(CPACK_PACKAGE_VENDOR "srcML, LLC.")

# contact
set(CPACK_PACKAGE_CONTACT "Software Development Laboratories <bugs@srcML.org>")
 
# package version
# set as part of project()

# license
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING.txt)

# README
set(CPACK_RESOURCE_FILE_README ${CMAKE_SOURCE_DIR}/README.md)

# welcome message
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/Package/welcome.txt)

# icon
if(WIN32)
    install(FILES ${CMAKE_SOURCE_DIR}/Package/srcml_icon.ico DESTINATION .)
else()
    # icon
    set(CPACK_PACKAGE_ICON ${CMAKE_SOURCE_DIR}/Package/srcml_icon.ico)
endif()

# strip executables
#set(CPACK_STRIP_FILES ON)
 
# output directory
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_BINARY_DIR}/dist)

# specific configurations
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Package)
include(debian)
include(rpm)
include(nsis)

# needs to be last so not overwritten
include(CPack)
