##
# @file doc.cmake
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

option(GENERATE_DOCUMENTATION "Enables the generation of documentation during the build and installation process." off)
if(${GENERATE_DOCUMENTATION})

    find_program(RONN_EXE REQUIRED NAMES ronn ronn.exe PATHS /bin /usr/bin ${WINDOWS_DEP_PATH}/bin)
    find_package(Doxygen REQUIRED)
    configure_file("${CMAKE_SOURCE_DIR}/doc/doxygen/config" "${CMAKE_BINARY_DIR}/config")
    SET(DOXYGEN_INPUT "${CMAKE_BINARY_DIR}/config")
    SET(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}")
    ADD_CUSTOM_COMMAND(
        OUTPUT ${DOXYGEN_OUTPUT_DIR}/html ${DOXYGEN_OUTPUT_DIR}/latex
        COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
        COMMAND ${CMAKE_COMMAND} -E echo "Done."
        WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR}
        DEPENDS ${DOXYGEN_INPUT} ${CMAKE_SOURCE_DIR}/src/libsrcml/srcml.h
    )

    # Configure file properties used to configure the
    # srcml_input.cfg_file

    set(SHOW_FILENAME_FLAG_LONG "show-filename")
    set(SHOW_LANGUAGE_FLAG_LONG "show-language")
    set(SHOW_URL_FLAG_LONG "show-url")
    set(SHOW_SRC_VERSION_FLAG_LONG "show-src-version")
    set(SHOW_TIMESTAMP_FLAG_LONG "show-timestamp")
    set(SHOW_HASH_FLAG_LONG "show-hash")
    set(SHOW_ENCODING_FLAG_LONG "show-encoding")
    set(SHOW_UNIT_COUNT_FLAG_LONG "show-unit-count")
    set(LANGUAGE_FLAG_SHORT "l")
    set(LANGUAGE_FLAG_LONG "language")
    set(URL_FLAG_LONG "url")
    set(FILENAME_FLAG_SHORT "f")
    set(FILENAME_FLAG_LONG "filename")
    set(SRCVERSION_FLAG_SHORT "s")
    set(SRCVERSION_FLAG_LONG "src-version")
    set(TEXT_FLAG_SHORT "t")
    set(TEXT_FLAG_LONG "text")
    set(SRC_ENCODING_FLAG_LONG "src-encoding")
    set(HASH_FLAG_LONG "hash")
    set(XML_ENCODING_FLAG_SHORT "x")
    set(XML_ENCODING_FLAG_LONG "xml-encoding")
    set(LONG_INFO_FLAG_SHORT "L")
    set(LONG_INFO_FLAG_LONG "longinfo")
    set(INFO_FLAG_SHORT "i")
    set(INFO_FLAG_LONG "info")
    set(PREFIX_FLAG_SHORT "p")
    set(PREFIX_FLAG_LONG "prefix")
    set(LINE_ENDING_FLAG_LONG "output-src-eol")
    set(LIST_FLAG_LONG "list")
    set(UPDATE_FLAG_LONG "update")
    set(OUTPUT_FLAG_SHORT "o")
    set(OUTPUT_FLAG_LONG "output")
    set(OUTPUT_XML_FLAG_SHORT "X")
    set(OUTPUT_XML_FLAG_LONG "output-xml")
    set(OUTPUT_SRC_FLAG_SHORT "S")
    set(OUTPUT_SRC_FLAG_LONG "output-src")
    set(OUTPUT_FORMAT_FLAG_LONG "output-format")
    set(TO_DIR_FLAG_LONG "to-dir")
    set(UNIT_OPTION_LONG "unit")
    set(UNIT_OPTION_SHORT "U")
    set(ARCHIVE_FLAG_SHORT "r")
    set(ARCHIVE_FLAG_LONG "archive")
    set(NO_XML_DECL_LONG "no-xml-declaration")
    set(NO_NAMESPACE_DECL_LONG "no-namespace-decl")
    set(INTERACTIVE_FLAG_SHORT "c")
    set(INTERACTIVE_FLAG_LONG "interactive")
    set(FILES_FROM_LONG "files-from")
    set(DEBUG_FLAG_SHORT "g")
    set(DEBUG_FLAG_LONG "debug")
    set(SRCML_ERR_NS_PREFIX_DEFAULT "err")
    set(SRCML_CPP_NS_PREFIX_DEFAULT "cpp")
    set(SRCML_EXT_POSITION_NS_PREFIX_DEFAULT "pos")
    set(SRCML_ERR_NS_URI "http://www.srcML.org/srcML/srcerr")
    set(SRCML_SRC_NS_URI "http://www.srcML.org/srcML/src")
    set(SRCML_CPP_NS_URI "http://www.srcML.org//srcML//cpp")
    set(SRCML_EXT_POSITION_NS_URI "http://www.srcML.org//srcML//position")
    set(POSITION_FLAG_LONG "position")
    set(XMLNS_FLAG "xmlns")
    set(VERBOSE_FLAG_SHORT "v")
    set(VERBOSE_FLAG_LONG "verbose")
    set(REGISTER_EXTENSION_FLAG_LONG "register-ext")
    set(LITERAL_FLAG "idk")
    set(MAX_THREADS_FLAG_LONG "max-threads")
    set(IN_ORDER_FLAG_LONG "in-order")
    set(EXTERNAL_LONG "external")
    set(TABS_FLAG "tabs")
    set(TIMESTAMP_FLAG_LONG "timestamp")
    set(CPP_FLAG_LONG "cpp")
    set(CPP_MARKUP_IF0_FLAG_LONG "cpp-markup-if0")
    set(CPP_NO_MARKUP_ELSE_FLAG_LONG "cpp-no-markup-else")
    set(XML_PROCESSING_FLAG_LONG "xml-processing")
    set(APPLY_ROOT_FLAG_LONG "apply-root")
    set(RELAXNG_OPTION_LONG "relaxng")
    set(XPATH_OPTION_LONG "xpath")
    set(XPATH_PARAM_LONG "xpathparam")
    set(XSLT_LONG "xslt")
    set(ATTRIBUTE_LONG "attribute")
    set(ELEMENT_LONG "element")
    set(STATUS_SUCCESS "0")
    set(STATUS_ERROR "1")
    set(STATUS_INVALID_ARGUMENT "2")
    set(STATUS_INPUTFILE_PROBLEM "3")
    set(SRCML_STATUS_INVALID_IO_OPERATION "4")
    set(SRCML_STATUS_IO_ERROR "5")
    set(SRCML_STATUS_UNINITIALIZED_UNIT "6")
    set(SRCML_STATUS_UNSET_LANGUAGE "7")
    set(SRCML_STATUS_NO_TRANSFORMATION "8")
    set(QUIET_FLAG_SHORT "q")
    set(QUIET_FLAG_LONG "quiet")
    set(VERSION_FLAG_LONG "version")
    set(VERSION_FLAG_SHORT "V")
    set(HELP_FLAG_SHORT "h")
    set(HELP_FLAG_LONG "help")
    set(PRETTY_FLAG_LONG "pretty")

    # Custom commands for creating things using the ronn executable.
    configure_file(${CMAKE_SOURCE_DIR}/doc/manpage/srcml_input.cfg ${CMAKE_BINARY_DIR}/srcml.md)
    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/srcml ${CMAKE_BINARY_DIR}/srcml.html 
        COMMAND ${RONN_EXE} ${CMAKE_BINARY_DIR}/srcml.md --manual=srcml
        DEPENDS ${CMAKE_BINARY_DIR}/srcml.md
    )
    ADD_CUSTOM_TARGET(doc ALL DEPENDS ${DOXYGEN_OUTPUT_DIR}/html ${DOXYGEN_OUTPUT_DIR}/latex ${CMAKE_BINARY_DIR}/srcml ${CMAKE_BINARY_DIR}/srcml.html)

    # man1 for srcml manpage
    install(FILES ${CMAKE_BINARY_DIR}/srcml RENAME srcml.1 DESTINATION /usr/local/share/man/man1)

    # share for srcml manpage in html, and libsrcml doxygen documentation
    install(FILES ${CMAKE_BINARY_DIR}/srcml.html DESTINATION /usr/local/share/srcml/doc)
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html DESTINATION /usr/local/share/srcml/doc/doxygen)

endif()

