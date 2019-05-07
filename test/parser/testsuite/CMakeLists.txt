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

configure_file(Makefile.gen ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(suite.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

file(GLOB PARSE_TESTS *.xml)
foreach(PARSE_TEST ${PARSE_TESTS})
	configure_file(${PARSE_TEST} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()

file(GLOB TEMPLATE_TESTS *.template)
foreach(TEMPLATE_TEST ${TEMPLATE_TESTS})
	configure_file(${TEMPLATE_TEST} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()

set(LANG_EXTENSIONS ";Java:java;C#:cs;C++:cpp;Objective-C:m;C:c;")

set(ALLGEN)

macro(setlanguage LANG URL)
	set(BASE "${URL}.cpp.xml")
	string(REGEX MATCH ";${LANG}:([a-z]+);" _ "${LANG_EXTENSIONS}")
	set(LANGEXT ${CMAKE_MATCH_1})
	set(PRODUCT "${URL}.${LANGEXT}.xml")
	execute_process(COMMAND srcml --language=${LANG} --output-xml ${CMAKE_CURRENT_SOURCE_DIR}/${BASE}
				    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT})
	list(APPEND ALLGEN "${PRODUCT}")
endmacro()

macro(applyxslt BASE PRODUCT URL XSLT)

	execute_process(COMMAND srcml --output-xml --xslt ${CMAKE_CURRENT_SOURCE_DIR}/../xsl/${XSLT} --url=url ${CMAKE_CURRENT_BINARY_DIR}/${BASE}
			    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT})
endmacro()

# generate special cases
applyxslt(class.cpp.xml struct.cpp.xml "struct" "class2struct.xsl")
applyxslt(class_cpp.cpp.xml struct_cpp.cpp.xml "struct" "class2struct.xsl")
applyxslt(class_cs.cs.xml struct_cs.cs.xml "struct" "class2struct.xsl")

applyxslt(struct.cpp.xml union.cpp.xml "union" "struct2union.xsl")
applyxslt(struct_cpp.cpp.xml union_cpp.cpp.xml "union" "struct2union.xsl")
applyxslt(struct_cs.cs.xml union_cs.cs.xml "union" "struct2union.xsl")
applyxslt(struct_c.c.xml union_c.c.xml "union" "struct2union.xsl")

file(COPY constructor_base.cpp.xml DESTINATION constructor.cpp.xml)

# keep track of all individual existing or created
file(STRINGS suite.txt SUITE REGEX "LANGUAGE_")
foreach(GEN ${SUITE})

	# split into category and url
	string(FIND "${GEN}" " " MID)
	string(SUBSTRING "${GEN}" 0 "${MID}" CATEGORY)
	math(EXPR MIDINC "${MID} + 1")
	string(SUBSTRING "${GEN}" "${MIDINC}" "-1" URL)

	if(CATEGORY STREQUAL "LANGUAGE_ALL_BASE")

		# base is C++, so these just get added
		list(APPEND ALLGEN "${URL}.cpp.xml")
		foreach(LANG "Java" "C#" "Objective-C" "C")
			setlanguage("${LANG}" "${URL}")
		endforeach()

	elseif(CATEGORY STREQUAL "LANGUAGE_ALL_GEN")

		# explicitly generated
		list(APPEND ALLGEN "${URL}.cpp.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_CSHARP")

		# exist, just add to list
		list(APPEND ALLGEN "${URL}.cs.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_CSHARP_GEN")

		# explicitly generated
		list(APPEND ALLGEN "${URL}.cs.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_CXX")

		# exist, just add to list
		list(APPEND ALLGEN "${URL}.cpp.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_CXX_FAMILY")

		list(APPEND ALLGEN "${URL}.cpp.xml")
		foreach(LANG "C#")
			setlanguage("${LANG}" "${URL}")
		endforeach()

	elseif(CATEGORY STREQUAL "LANGUAGE_CXX_GEN")

		# explicitly generated
		list(APPEND ALLGEN "${URL}.cpp.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_C_FAMILY")

		list(APPEND ALLGEN "${URL}.cpp.xml")
		foreach(LANG "C#" "C")
			setlanguage("${LANG}" "${URL}")
		endforeach()

	elseif(CATEGORY STREQUAL "LANGUAGE_C_ONLY")

		# exist, just add to list
		list(APPEND ALLGEN "${URL}.c.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_JAVA")

		# exist, just add to list
		list(APPEND ALLGEN "${URL}.java.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_JAVA_GEN")

		# explicitly generated
		list(APPEND ALLGEN "${URL}.java.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_OBJECTIVE_C")

		# exist, just add to list
		list(APPEND ALLGEN "${URL}.m.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_OBJECTIVE_C_GEN")

		# explicitly generated
		list(APPEND ALLGEN "${URL}.c.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_OO")

		# base is C++, so these just get added
		list(APPEND ALLGEN "${URL}.cpp.xml")
		foreach(LANG "C#" "Java")
			setlanguage("${LANG}" "${URL}")
		endforeach()

	endif()

endforeach()

foreach(GENURL ${ALLGEN})

#	message("GENURL |${GENURL}|")

	# formfeed
	string(REGEX MATCH "([a-z_]+)\.([a-z_]+)\.xml" _ "${GENURL}")
	set(BASE "${GENURL}")

	foreach(XSLTYPE "formfeed" "comment" "block" "preproc" "struct")
		set(PRODUCT "${CMAKE_MATCH_1}.${XSLTYPE}.${CMAKE_MATCH_2}.xml")
		execute_process(COMMAND srcml --output-xml --xslt ${CMAKE_CURRENT_SOURCE_DIR}/../xsl/insert${XSLTYPE}.xsl --url=${CMAKE_MATCH_1}.formfeed ${CMAKE_CURRENT_BINARY_DIR}/${BASE}
				    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT})
	endforeach()

endforeach()
