##
# @file nsis.cmake
#
# @copyright Copyright (C) 2014 srcML, LLC. (www.srcML.org)
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

# set icons
set(CPACK_NSIS_MUI_ICON ${CMAKE_SOURCE_DIR}/CMake/srcml_icon.ico)
set(CPACK_NSIS_MUI_UNIICON ${CMAKE_SOURCE_DIR}/CMake/srcml_icon.ico)
set(CPACK_NSIS_INSTALLED_ICON_NAME srcml_icon.ico)

set(CPACK_NSIS_MODIFY_PATH ON)
if(WIN32)
    # Checking the correct version of visual studio we are only supporting
    # the last 3 versions of the installer. and ONLY the x86 version (we don't have a
    # 64 bit build of any of the required libraries needed to create a 64 bit version.)
    if (MSVC12)
        set(MSVC_VERSION_NUMBER "12.0")
    elseif(MSVC11)
        set(MSVC_VERSION_NUMBER "11.0")
    elseif(MSVC10)
        set(MSVC_VERSION_NUMBER "10.0")
    endif()
    find_program(MSVC_REDIST NAMES vcredist_x86.exe
      PATHS
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VCExpress\\${MSVC_VERSION_NUMBER};InstallDir]/../../SDK/v${MSVC_VERSION_NUMBER}/BootStrapper/Packages/"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\VisualStudio\\${MSVC_VERSION_NUMBER};InstallDir]/../../SDK/v${MSVC_VERSION_NUMBER}/BootStrapper/Packages/"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\${MSVC_VERSION_NUMBER};InstallDir]/../../SDK/v${MSVC_VERSION_NUMBER}/BootStrapper/Packages/"
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\${MSVC_VERSION_NUMBER};InstallDir]/../../VC/redist/1033"
      REQUIRED
    )
    get_filename_component(vcredist_name "${MSVC_REDIST}" NAME)
    install(PROGRAMS ${MSVC_REDIST} DESTINATION bin)
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\${vcredist_name}\\\" /passive /norestart'")
endif()
# set contact in add/remove programs
set(CPACK_NSIS_CONTACT "Software Developement Laboratories <bugs@srcML.org>")
