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
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

# Debian package name
set(CPACK_DEBIAN_PACKAGE_NAME "srcML")


# architecture
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
else()
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
endif()

# autogenerate dependency information
set(CPACK_DEBIAN_PACKAGE_DEBUG ON)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

# package type
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")

# package priority
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

# recommended package
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "yes")

# package suggests
set(CPACK_DEBIAN_PACKAGE_SUGGESTS "yes")

# set install for manpage (Run cpack -G DEB)
# @TODO Have this called higher up
include(GNUInstallDirs)

message("CMAKE_INSTALL_MANDIR: ${CMAKE_INSTALL_MANDIR}")

# @TODO This may not be working
install(FILES "${CMAKE_SOURCE_DIR}/doc/manpage/srcml.1" DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
