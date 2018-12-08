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

find_program(RONN_EXE REQUIRED NAMES ronn ronn.exe PATHS /bin /usr/bin ${WINDOWS_DEP_PATH}/bin)

# Configure file properties used to configure the file srcml.cfg
set(HELP_FLAG_LONG "help")
set(HELP_FLAG_SHORT "h")
set(VERSION_FLAG_LONG "version")
set(VERSION_FLAG_SHORT "V")
set(VERBOSE_FLAG_LONG "verbose")
set(VERBOSE_FLAG_SHORT "v")
set(QUIET_FLAG_LONG "quiet")
set(QUIET_FLAG_SHORT "q")
set(OUTPUT_FLAG_LONG "output")
set(OUTPUT_FLAG_SHORT "o")
# @todo Missing in doc
set(JOBS_FLAG_LONG "jobs")
set(JOBS_FLAG_SHORT "j")

set(FILES_FROM_LONG "files-from")
set(TEXT_FLAG_LONG "text")
set(TEXT_FLAG_SHORT "t")
set(LANGUAGE_FLAG_LONG "language")
set(LANGUAGE_FLAG_SHORT "l")
set(REGISTER_EXTENSION_FLAG_LONG "register-ext")
set(SRC_ENCODING_FLAG_LONG "src-encoding")
set(ARCHIVE_FLAG_SHORT "r")
set(ARCHIVE_FLAG_LONG "archive")
set(OUTPUT_XML_FLAG_LONG "output-xml")
set(OUTPUT_XML_FLAG_SHORT "X")
set(OUTPUT_XML_FRAGMENT_FLAG_LONG "output-xml-fragment")
set(OUTPUT_XML_FRAGMENT_FLAG_SHORT "F")
set(OUTPUT_XML_RAW_FLAG_LONG "output-xml-raw")
set(OUTPUT_XML_RAW_FLAG_SHORT "R")
set(POSITION_FLAG_LONG "position")
set(TABS_FLAG "tabs")
set(CPP_FLAG_LONG "cpp")
set(CPP_MARKUP_IF0_FLAG_LONG "cpp-markup-if0")
set(CPP_NO_MARKUP_ELSE_FLAG_LONG "cpp-no-markup-else")
set(XML_ENCODING_FLAG_LONG "xml-encoding")
set(XML_ENCODING_FLAG_SHORT "x")
set(NO_XML_DECL_LONG "no-xml-declaration")
set(XMLNS_FLAG "xmlns")
set(LIST_FLAG_LONG "list")
set(LIST_FLAG_SHORT "L")
set(INFO_FLAG_LONG "info")
set(INFO_FLAG_SHORT "i")
set(FULL_INFO_FLAG_LONG "full-info")
set(FULL_INFO_FLAG_SHORT "I")
set(SHOW_LANGUAGE_FLAG_LONG "show-language")
set(SHOW_URL_FLAG_LONG "show-url")
set(SHOW_FILENAME_FLAG_LONG "show-filename")
set(SHOW_SRC_VERSION_FLAG_LONG "show-src-version")
set(SHOW_TIMESTAMP_FLAG_LONG "show-timestamp")
set(SHOW_HASH_FLAG_LONG "show-hash")
set(SHOW_ENCODING_FLAG_LONG "show-encoding")
set(SHOW_UNIT_COUNT_FLAG_LONG "show-unit-count")
set(SHOW_PREFIX_FLAG_LONG "show-prefix")
set(FILENAME_FLAG_LONG "filename")
set(FILENAME_FLAG_SHORT "f")
set(URL_FLAG_LONG "url")
set(SRCVERSION_FLAG_LONG "src-version")
set(SRCVERSION_FLAG_SHORT "s")
set(HASH_FLAG_LONG "hash")
set(TIMESTAMP_FLAG_LONG "timestamp")
set(UNIT_OPTION_LONG "unit")
set(UNIT_OPTION_SHORT "U")
set(OUTPUT_SRC_FLAG_LONG "output-src")
set(OUTPUT_SRC_FLAG_SHORT "S")
set(TO_DIR_FLAG_LONG "to-dir")
set(XPATH_OPTION_LONG "xpath")
set(ATTRIBUTE_LONG "attribute")
set(ELEMENT_LONG "element")
set(XSLT_LONG "xslt")
# @todo Rename constant
set(XPATH_PARAM_LONG "xslt-param")
set(RELAXNG_OPTION_LONG "relaxng")

#set(LONG_INFO_FLAG_SHORT "L")
#set(LONG_INFO_FLAG_LONG "longinfo")
set(PREFIX_FLAG_LONG "show-prefix")
set(LINE_ENDING_FLAG_LONG "output-src-eol")
set(UPDATE_FLAG_LONG "update")
set(OUTPUT_FORMAT_FLAG_LONG "output-format")
#set(NO_NAMESPACE_DECL_LONG "no-namespace-decl")
#set(INTERACTIVE_FLAG_SHORT "c")
#set(INTERACTIVE_FLAG_LONG "interactive")
set(DEBUG_FLAG_SHORT "g")
set(DEBUG_FLAG_LONG "debug")
set(SRCML_ERR_NS_PREFIX_DEFAULT "err")
set(SRCML_CPP_NS_PREFIX_DEFAULT "cpp")
set(SRCML_EXT_POSITION_NS_PREFIX_DEFAULT "pos")
set(SRCML_ERR_NS_URI "http://www.srcML.org/srcML/srcerr")
set(SRCML_SRC_NS_URI "http://www.srcML.org/srcML/src")
set(SRCML_CPP_NS_URI "http://www.srcML.org/srcML/cpp")
set(SRCML_EXT_POSITION_NS_URI "http://www.srcML.org/srcML/position")
set(LITERAL_FLAG "idk")
set(MAX_THREADS_FLAG_LONG "max-threads")
set(EXTERNAL_LONG "external")
set(XML_PROCESSING_FLAG_LONG "xml-processing")
set(APPLY_ROOT_FLAG_LONG "apply-root")
set(STATUS_SUCCESS "0")
set(STATUS_ERROR "1")
set(STATUS_INVALID_ARGUMENT "2")
set(STATUS_INVALID_OPTION "3")
set(SRCML_STATUS_UNKNOWN_ENCODING "7")
#set(SRCML_STATUS_IO_ERROR "5")
set(SRCML_STATUS_INVALID_LANGUAGE "6")
#set(SRCML_STATUS_NO_TRANSFORMATION "8")
set(PRETTY_FLAG_LONG "pretty")

# Custom commands for creating things using the ronn executable.
configure_file(${CMAKE_SOURCE_DIR}/doc/manpage/srcml.cfg ${CMAKE_BINARY_DIR}/srcml.md)
add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/srcml ${CMAKE_BINARY_DIR}/srcml.html 
    COMMAND ${RONN_EXE} ${CMAKE_BINARY_DIR}/srcml.md --manual=srcml
    DEPENDS ${CMAKE_BINARY_DIR}/srcml.md
)
ADD_CUSTOM_TARGET(doc_manpage ALL DEPENDS ${CMAKE_BINARY_DIR}/srcml ${CMAKE_BINARY_DIR}/srcml.html)
install(FILES ${CMAKE_BINARY_DIR}/srcml ${CMAKE_BINARY_DIR}/srcml.html DESTINATION doc/man)
