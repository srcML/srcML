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
    # Setting Configuration Variables.
    set(GENERATOR_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(SRCML_H_HEADER_PATH ${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h)
    get_target_property(SHARED_LIBSRCML_OUTPUT_PATH srcml_shared LIBRARY_OUTPUT_DIRECTORY)
    get_target_property(SHARED_LIBSRCML_OUTPUT_NAME srcml_shared OUTPUT_NAME)
    if(WIN32)
        set(DYNAMIC_LIBSRCML_LOCATION ${SHARED_LIBSRCML_OUTPUT_PATH}/lib${SHARED_LIBSRCML_OUTPUT_NAME}.dll)
    elseif(APPLE)
        set(DYNAMIC_LIBSRCML_LOCATION ${SHARED_LIBSRCML_OUTPUT_PATH}/lib${SHARED_LIBSRCML_OUTPUT_NAME}.dylib)
    else()
        set(DYNAMIC_LIBSRCML_LOCATION ${SHARED_LIBSRCML_OUTPUT_PATH}/lib${SHARED_LIBSRCML_OUTPUT_NAME}.so)
    endif()
    set(SRCML_H_XML_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/srcml.h.xml)


    # Configuring the file used for generating the srcml.h.xml file
    configure_file(
        srcmlXMLHeaderGeneratorTemplate.py
        genLibsrcmlXMLHeader.py
        NEWLINE_STYLE UNIX
    )
    file(GLOB_RECURSE GENERATOR_LIB_FILES "${CMAKE_CURRENT_SOURCE_DIR}/generator/*")

    # Running newly generated python file.
    add_custom_command(
        OUTPUT ${SRCML_H_XML_OUTPUT_PATH}
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/genLibsrcmlXMLHeader.py
        DEPENDS ${GENERATOR_LIB_FILES} ${CMAKE_CURRENT_BINARY_DIR}/genLibsrcmlXMLHeader.py srcml_shared

    )
    # Adding Target so that I can manually invoke this part of the build for testing.
    add_custom_target(
        build_srcml_h_xml
        DEPENDS ${SRCML_H_XML_OUTPUT_PATH}
        COMMAND echo "Created srcml.h.xml from srcml.h"
    )
    # add_custom_command(
    #     OUTPUT 
    # )
    
    # if(NOT ${CMAKE_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
        # add_custom_target(copy_test_data_target ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/xpath_test_data" "${CMAKE_BINARY_DIR}/bin/xpath_test_data")
        # add_dependencies(test_srcexfun_impl copy_test_data_target)
        # file(GLOB_RECURSE xpath_test_files "${CMAKE_CURRENT_SOURCE_DIR}/xpath_test_data/*")
        # add_dependencies(test_srcexfun_impl ${xpath_test_files})
    # endif()


    # Include individual language Directories
    if(${BUILD_PYTHON_BINDINGS})
        set(SRCML_PY_BINDINGS_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/python/srcml)
        set(PY_BINDINGS_OUTPUT_FILE ${SRCML_PY_BINDINGS_LOCATION}/bindings.py)
        configure_file(
            python/configTemplate.py
            python/genPythonBindings.py
            NEWLINE_STYLE UNIX
        )
        add_custom_command(
            OUTPUT ${PY_BINDINGS_OUTPUT_FILE}
            COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/python/genPythonBindings.py
            DEPENDS build_srcml_h_xml ${SRCML_H_XML_OUTPUT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/python/genPythonBindings.py

        )
        # Python Target
        add_custom_target(
            build_python_bindings
            DEPENDS ${PY_BINDINGS_OUTPUT_FILE}
            COMMAND echo "Generated Python C Bindings"
        )
    endif()

    if(${BUILD_CSHARP_BINDINGS})
        if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
            message(WARNING "C# build hasn't been tested with Non-Visual Studio Projects. This may cause errors or build failures.")
        endif()
    endif()
endif()