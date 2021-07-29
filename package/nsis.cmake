##
# @file nsis.cmake
#
# @copyright Copyright (C) 2014-2019 srcML, LLC. (www.srcML.org)
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

# Remove component LOCAL which is not for packaging
get_cmake_property(CPACK_COMPONENTS_ALL COMPONENTS)
list(REMOVE_ITEM CPACK_COMPONENTS_ALL "LOCAL" "SRCMLDEV")

# set icons
install(FILES ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico DESTINATION . COMPONENT SRCML)
set(CPACK_NSIS_MUI_ICON ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)
set(CPACK_NSIS_MUI_UNIICON ${CMAKE_SOURCE_DIR}/package/srcml_icon.ico)
set(CPACK_NSIS_INSTALLED_ICON_NAME srcml_icon.ico)

set(CPACK_NSIS_MODIFY_PATH ON)
if(WIN32)
    set(MSVC_REDIST ${PROJECT_BINARY_DIR}/deps/tools/VC_redist.x64.exe)
    get_filename_component(vcredist_name "${MSVC_REDIST}" NAME)
    install(PROGRAMS ${MSVC_REDIST} DESTINATION bin COMPONENT SRCML )
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\${vcredist_name}\\\" /passive /norestart'")
endif()
# set contact in add/remove programs
set(CPACK_NSIS_CONTACT "srcML <srcML.org>")
