##
# @file nsis.cmake
#
# @copyright Copyright (C) 2014 SDML (www.srcML.org)
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
# D:\ProgrammingProjects\srcMLGitHubRepo\srcML
# set add to path variable and ask for shortcut
set(CPACK_NSIS_MODIFY_PATH ON)
if(WIN32)
    set(CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "!include LogicLib.nsh
!macro IfKeyExists ROOT MAIN_KEY KEY
  Push $R0
  Push $R1
  Push $R2
 
  StrCpy $R1 \\\"0\\\"
  StrCpy $R2 \\\"0\\\"
 
  \\\${Do}
    EnumRegKey $R0 \\\${ROOT} \\\"\\\${MAIN_KEY}\\\" \\\"$R1\\\"
    \\\${If} $R0 == \\\"\\\${KEY}\\\"
      StrCpy $R2 \\\"1\\\"
      \\\${Break}
    \\\${EndIf}
    IntOp $R1 $R1 + 1
  \\\${LoopWhile} $R0 != \\\"\\\"
 
  ClearErrors
 
  Exch 2
  Pop $R0
  Pop $R1
  Exch $R2
!macroend")
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "

#!insertmacro IfKeyExists \\\"HKEY_LOCAL_MACHINE\\\" \\\"SOFTWARE\\\\Wow6432Node\\\\Microsoft\\\\DevDiv\\\\vc\\\\Servicing\\\\12.0\\\\RuntimeMinimum\\\" \\\"Version\\\"
#Pop \$R0
#!include LogicLib.nsh
#\\\${If} \$R0 == 1
File \\\"D:\\\\OtherProgramFiles\\\\Visual Studio 12.0\\\\VC\\\\redist\\\\1033\\\\vcredist_x86.exe\\\" 	
ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\"  /passive /norestart'
#\\\${EndIf}

")
# 
    message(STATUS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")
endif()
# set contact in add/remove programs
set(CPACK_NSIS_CONTACT "Software Developement Laboratories <bugs@srcML.org>")
