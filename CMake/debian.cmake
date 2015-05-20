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

# set Debian package name
set(CPACK_DEBIAN_PACKAGE_NAME "srcML")

# set architecture
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "i686")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
else()
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
endif()

# set dependencies These need to be checked and updated or made to be os specific
#set(CPACK_DEBIAN_PACKAGE_DEPENDS "libxml2 (>= 2.7.8), libxslt1.1 (>= 1.1.26), libarchive12 (>= 3.0.3)")

# set package type
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")

# set the package priority
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

# set recommended package
set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "yes")

# set package suggests
set(CPACK_DEBIAN_PACKAGE_SUGGESTS "yes")

# set extra processing
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "")
