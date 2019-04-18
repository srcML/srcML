##
# @file CMakeLists.txt
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

file(GLOB LIBSRCML_SOURCE *.h *.hpp *.cpp)

add_library(libsrcml OBJECT ${LIBSRCML_SOURCE})
add_dependencies(libsrcml parser)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/srcml.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    file(GLOB SRCML_DLLS ${WINDOWS_DEP_PATH}/${BUILD_ARCH}/release/bin/*.dll)
    install(FILES ${SRCML_DLLS} DESTINATION bin)
endif()
