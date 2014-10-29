##
# @file config.cmake
#
# @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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



if(${BUILD_LANGUAGE_BINDINGS})
    # message(STATUS "Constructing Bindings for A Language")
    set(GENERATOR_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    set(SRCML_H_HEADER_PATH ${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h)

    # Include individual language Directories
    if(${BUILD_PYTHON_BINDINGS})
        get_target_property(DYNAMIC_LIBSRCML_LOCATION srcml_shared LIBRARY_OUTPUT_DIRECTORY)
        set(SRCML_PY_BINDINGS_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/python/srcml)
        configure_file(
            python/config.py
            python/bindingBuilder.py
            NEWLINE_STYLE UNIX
        )

    # set(PYTHON_EXECUTABLE)
    endif()

    if(${BUILD_CSHARP_BINDINGS})
        if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            message(WARNING "C# build hasn't been tested with Non-Visual Studio Projects. This may cause errors or build failures.")
        endif()
    endif()
endif()