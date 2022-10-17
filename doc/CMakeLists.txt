##
# @file CMakeLists.txt
# 
# @copyright Copyright (C) 2013-2019 srcML, LLC. (www.srcML.org)
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
# 
# CMake files for documentation

# Turn ON/OFF building documentation
option(BUILD_CLIENT_DOC "Build client documentation" OFF)
option(INSTALL_CLIENT_DOC "Install (but do not build) client documentation" OFF)
option(BUILD_LIBSRCML_DOC "Build libsrcml documentation" OFF)
if(NOT BUILD_CLIENT_DOC)
    set(INSTALL_CLIENT_DOC ON)
endif()

# Turn ON building all documentation
option(BUILD_ALL_DOCS "Build all documentation" OFF)
if(BUILD_ALL OR BUILD_ALL_DOCS)
    set(BUILD_CLIENT_DOC ON)
    set(BUILD_LIBSRCML_DOC ON)
endif()

if(BUILD_CLIENT_DOC OR INSTALL_CLIENT_DOC)
    add_subdirectory(manpage)
endif()

if(BUILD_LIBSRCML_DOC)
    add_subdirectory(doxygen)
endif()
