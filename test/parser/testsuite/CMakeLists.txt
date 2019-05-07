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

# all individual existing or generated so that variations (formfeed, etc.) can be built
set(ALLGEN)

# Sets the language of the result
macro(setlanguage LANG URL)
	# convert from language to extension
	if(LANG STREQUAL "Java")
		set(LANGEXT "java")
	elseif(LANG STREQUAL "Objective-C")
		set(LANGEXT "m")
	elseif(LANG STREQUAL "C#")
		set(LANGEXT "cs")
	elseif(LANG STREQUAL "C")
		set(LANGEXT "c")
	else()
		set(LANGEXT "cpp")
	endif()

	# assumes a C++ base
	set(BASE "${URL}.cpp.xml")
	set(PRODUCT "${URL}.${LANGEXT}.xml")
	execute_process(COMMAND srcml --language=${LANG} --output-xml ${CMAKE_CURRENT_BINARY_DIR}/${BASE}
				    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT})

	# record product so that variation are generated
	list(APPEND ALLGEN "${PRODUCT}")
endmacro()

# Transforms BASE into PRODUCT with XSLT and applies the given URL
macro(transform BASE PRODUCT URL XSLT)
	execute_process(COMMAND srcml --output-xml --xslt ${CMAKE_CURRENT_SOURCE_DIR}/../xsl/${XSLT} --url=url ${CMAKE_CURRENT_BINARY_DIR}/${BASE}
			    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT})
endmacro()

# Generate special transformations

# class -> struct
transform(class.cpp.xml struct.cpp.xml "struct" "class2struct.xsl")
transform(class_cpp.cpp.xml struct_cpp.cpp.xml "struct" "class2struct.xsl")
transform(class_cs.cs.xml struct_cs.cs.xml "struct" "class2struct.xsl")

# struct -> union
transform(struct.cpp.xml union.cpp.xml "union" "struct2union.xsl")
transform(struct_cpp.cpp.xml union_cpp.cpp.xml "union" "struct2union.xsl")
transform(struct_cs.cs.xml union_cs.cs.xml "union" "struct2union.xsl")
transform(struct_c.c.xml union_c.c.xml "union" "struct2union.xsl")

configure_file(constructor_base.cpp.xml ${CMAKE_CURRENT_BINARY_DIR}/constructor.cpp.xml COPYONLY)

# if -> while
transform(if.cpp.xml while.cpp.xml "while" "if2while.xsl")

# C++ special transformations
transform(function.cpp.xml function_const.cpp.xml "const" "addconst.xsl")
transform(constructor.cpp.xml constructor_explicit.cpp.xml "constructor" "addexplicit.xsl")
transform(if.cpp.xml ifnestcond.cpp.xml "if" "nestcond.xsl")
transform(destructor.cpp.xml destructor_virtual.cpp.xml "destructor.virtual" "addvirtual.xsl")
transform(throw.cpp.xml throw_base.cpp.xml "throw" "insertexpr.xsl")
transform(class_java.java.xml interface_java.java.xml "interface" "class2interface.xsl")
transform(if.cpp.xml ifblock.cpp.xml "if.block" "if2ifblock.xsl")
execute_process(COMMAND srcml --output-xml --xslt ${CMAKE_CURRENT_SOURCE_DIR}/../xsl/insertexpr.xsl --xslt-param=expr_filename="../testsuite/expression.cpp.xml" --url=return ${CMAKE_CURRENT_BINARY_DIR}/return_base.cpp.xml
			    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/return.cpp.xml)
transform(function.cpp.xml function_decl.cpp.xml "function" "defn2decl.xsl")

# Was in Makefile.gen, but cannot find the file
#transform(function_asterisk.cpp.xml function_asterisk_decl.cpp.xml "function" "defn2decl.xsl")

# C# special transformations
transform(class_interface_m.m.xml class_implementation_m.m.xml "function" "interface2implementation.xsl")
transform(try.cpp.xml try_m.m.xml "try" "keyword2mkeyword.xsl")
transform(catch_form.cpp.xml catch_m.m.xml "catch" "keyword2mkeyword.xsl")
transform(finally_cs.cs.xml finally_java.java.xml "finally" "keyword2mkeyword.xsl")

#execute_process(COMMAND srcml --output-xml --set-language xslt ${CMAKE_CURRENT_SOURCE_DIR}/../xsl/${XSLT} --url=url ${CMAKE_CURRENT_BINARY_DIR}/${BASE}
#			    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT})

# C# finally -> Java finally
execute_process(COMMAND srcml --language=Java --output-xml ${CMAKE_CURRENT_BINARY_DIR}/finally_cs.cs.xml
				    -o ${CMAKE_CURRENT_BINARY_DIR}/finally_java.java.xml)

# Objective-C try, catch, finally, throw from C++
transform(try.cpp.xml try_m.m.xml "try" "keyword2mkeyword.xsl")
transform(catch_form.cpp.xml catch_m.m.xml "catch" "keyword2mkeyword.xsl")
transform(finally_cs.cs.xml finally_m.m.xml "finally" "keyword2mkeyword.xsl")
transform(throw.cpp.xml throw_m.m.xml "throw" "keyword2mkeyword.xsl")
transform(throw_base.cpp.xml throw_base_m.m.xml "throw" "keyword2mkeyword.xsl")

# Process all tests in suite.txt according to their categories
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
		list(APPEND ALLGEN "${URL}.m.xml")

	elseif(CATEGORY STREQUAL "LANGUAGE_OO")

		# base is C++, so these just get added
		list(APPEND ALLGEN "${URL}.cpp.xml")
		foreach(LANG "C#" "Java")
			setlanguage("${LANG}" "${URL}")
		endforeach()

	endif()

endforeach()

# Generate variations: formfeed, comment, block, etc.
foreach(GENURL ${ALLGEN})

	string(REGEX MATCH "([a-z_]+)\.([a-z_]+)\.xml" _ "${GENURL}")

	foreach(XSLTYPE "formfeed" "comment" "block" "preproc" "struct")
		set(PRODUCT "${CMAKE_MATCH_1}.${XSLTYPE}.${CMAKE_MATCH_2}.xml")
		execute_process(COMMAND srcml --output-xml --xslt ${CMAKE_CURRENT_SOURCE_DIR}/../xsl/insert${XSLTYPE}.xsl --url=${CMAKE_MATCH_1}.formfeed ${CMAKE_CURRENT_BINARY_DIR}/${GENURL}
				    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT})
	endforeach()

endforeach()
