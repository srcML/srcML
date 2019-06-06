##
# @file debian.cmake
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

# Debian package name
set(CPACK_DEBIAN_PACKAGE_NAME "srcML")

set(CPACK_DEBIAN_PACKAGE_HOMEPAGE https://srcMl.org)

# architecture
# if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
#     set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
# else()
#     set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
# endif()

# autogenerate dependency information
set(CPACK_DEBIAN_PACKAGE_DEBUG ON)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# package type
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")

# package priority
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

# recommended package
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "libxslt, zip, unzip, cpio, tar, man")

# package suggests
#set(CPACK_DEBIAN_PACKAGE_SUGGESTS "yes")
