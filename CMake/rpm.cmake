##
# @file rpm.cmake
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

# RPM package name
set(CPACK_RPM_PACKAGE_NAME "srcML")

# architecture
#set(CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

# package release number (as oppossed to srcML release)
set(CPACK_RPM_PACKAGE_RELEASE 1)

# post install script for ldconfig
file(WRITE ${CPACK_PACKAGE_DIRECTORY}/post.sh "ldconfig")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE ${CPACK_PACKAGE_DIRECTORY}/post.sh)

# cpack puts directories leading to the installed files into the RPM. Since they are already
# created, this leads to a conflict with other packages. This excludes these (which are mostly documentation)
# Note that there is no problem with this list including directories that are not needed
set (CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION ${CMAKE_INSTALL_MANDIR}/man1 ${CMAKE_INSTALL_MANDIR} /usr/local /usr/local/man /usr/local/man/man1 /usr/local/share /usr/local/share/man /usr/local/share/man/man1 /usr/share/man /usr/share/man/man1 /usr/share/man1)
