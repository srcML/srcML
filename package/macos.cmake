##
# @file macos.cmake
#
# @copyright Copyright (C) 2019 srcML, LLC. (www.srcML.org)
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

# Custom template CPack.distribution.dist.in
file(WRITE ${CMAKE_BINARY_DIR}/pkg_resources/CPack.distribution.dist.in
'<?xml version="1.0" encoding="UTF-8"?>
<installer-gui-script minSpecVersion="1.0">
    <title>@CPACK_PACKAGE_NAME@</title>
    <welcome file="@CPACK_RESOURCE_FILE_WELCOME_NOPATH@"/>
    <license file="@CPACK_RESOURCE_FILE_LICENSE_NOPATH@"/>
    <background file="BACKGROUND.png"/>
    <options allow-external-scripts="no" customize="allow" rootVolumeOnly="false"></options>
    @CPACK_PACKAGEMAKER_CHOICES@
</installer-gui-script>
')
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}/pkg_resources")

set(CPACK_COMPONENTS_ONE_PACKAGE_PER_GROUP ON)

#set(CPACK_PRODUCTBUILD_COMPONENT_INSTALL ON)
set(CPACK_PACKAGING_INSTALL_PREFIX /usr/local)
set(CPACK_RESOURCE_FILE_WELCOME ${CMAKE_SOURCE_DIR}/package/welcome.txt)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING.txt)
configure_file(${CMAKE_SOURCE_DIR}/README.md ${CMAKE_BINARY_DIR}/README.txt COPYONLY)
set(CPACK_RESOURCE_FILE_README ${CMAKE_BINARY_DIR}/README.txt)
configure_file(${CMAKE_SOURCE_DIR}/package/background.png ${CMAKE_BINARY_DIR}/pkg_resources/BACKGROUND.png COPYONLY)
set(CPACK_PRODUCTBUILD_RESOURCE_DIR ${CMAKE_BINARY_DIR}/pkg_resources)
