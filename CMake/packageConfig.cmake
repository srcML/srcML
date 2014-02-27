##
# @file packageConfig.cmake
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
 

# Generator settings
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
 set(CPACK_GENERATOR "ZIP")
else()
set(CPACK_GENERATOR "TGZ;TBZ2")
endif()
 
# set package name
set(CPACK_PACKAGE_FILE_NAME "srcML-${CMAKE_SYSTEM_NAME}")

# set summary
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "srcML Toolkit")

# contact
set(CPACK_PACKAGE_CONTACT "Software Developement Laboratories <bugs@srcML.org>")
 
# package version
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

# strip executables
#set(CPACK_STRIP_FILES ON)
 
# set output directory
set(CPACK_PACKAGE_DIRECTORY ${CMAKE_HOME_DIRECTORY}/dist)

# include other specific configurations
include(debian)
include(rpm)
include(packageMaker)

# needs to be last so not overwritten
include(CPack)
