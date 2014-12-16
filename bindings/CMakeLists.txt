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



if(${BUILD_LANGUAGE_BINDINGS})
    # Setting Configuration Variables.
    set(GENERATOR_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(SRCML_H_HEADER_PATH ${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h)
    get_target_property(SHARED_LIBSRCML_OUTPUT_PATH srcml_shared LIBRARY_OUTPUT_DIRECTORY)
    get_target_property(SHARED_LIBSRCML_OUTPUT_NAME srcml_shared OUTPUT_NAME)
    if(WIN32)
        set(DYNAMIC_LIBSRCML_LOCATION ${SHARED_LIBSRCML_OUTPUT_PATH}/${SHARED_LIBSRCML_OUTPUT_NAME}.dll)
    elseif(APPLE)
        set(DYNAMIC_LIBSRCML_LOCATION ${SHARED_LIBSRCML_OUTPUT_PATH}/lib${SHARED_LIBSRCML_OUTPUT_NAME}.dylib)
    else()
        set(DYNAMIC_LIBSRCML_LOCATION ${SHARED_LIBSRCML_OUTPUT_PATH}/lib${SHARED_LIBSRCML_OUTPUT_NAME}.so)
    endif()
    set(SRCML_H_XML_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/srcml.h.xml)

    file(GLOB_RECURSE GENERATOR_LIB_FILES "${CMAKE_CURRENT_SOURCE_DIR}/generator/*")

    # Configuring the file used for generating the srcml.h.xml file
    configure_file(
        srcmlXMLHeaderGeneratorTemplate.py
        genLibsrcmlXMLHeader.py
        NEWLINE_STYLE UNIX
    )

    # Running newly generated python file.
    add_custom_command(
        OUTPUT ${SRCML_H_XML_OUTPUT_PATH}
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/genLibsrcmlXMLHeader.py
        DEPENDS ${GENERATOR_LIB_FILES} ${CMAKE_CURRENT_BINARY_DIR}/genLibsrcmlXMLHeader.py srcml_shared
    )

    # Include individual language Directories
    if(${BUILD_PYTHON_BINDINGS})
        set(SRCML_PY_BINDINGS_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/python/srcml)
        set(SRCML_PY_LIB_SEARCH_PATH_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/python)
        set(PY_BINDINGS_OUTPUT_FILE ${SRCML_PY_BINDINGS_LOCATION}/bindings.py)
        set(PY_ARCHIVE_CLASS_FILE ${SRCML_PY_BINDINGS_LOCATION}/archive.py)
        set(PY_UNIT_CLASS_FILE ${SRCML_PY_BINDINGS_LOCATION}/unit.py)
        set(PY_TEST_SUITE_OUTPUT_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/python/test/srcml_test/test_bindings.py)
        configure_file(
            python/configTemplate.py
            python/genPythonBindings.py
            NEWLINE_STYLE UNIX
        )
        add_custom_command(
            OUTPUT ${PY_BINDINGS_OUTPUT_FILE} ${PY_TEST_SUITE_OUTPUT_FILE_PATH}
            COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/python/genPythonBindings.py
            DEPENDS ${SRCML_H_XML_OUTPUT_PATH} ${CMAKE_CURRENT_BINARY_DIR}/python/genPythonBindings.py

        )

        set(PY_TEST_SOURCE_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/python/test/testRunner.py)
        configure_file(
            python/test/testLauncherConfig.py
            ${CMAKE_CURRENT_SOURCE_DIR}/python/test/testLauncher.py
            NEWLINE_STYLE UNIX
        )

        # Python Target
        add_custom_target(
            build_python_bindings ALL
            DEPENDS ${PY_BINDINGS_OUTPUT_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/python/test/testLauncher.py python/test/testLauncherConfig.py
        )
    endif()
endif()
