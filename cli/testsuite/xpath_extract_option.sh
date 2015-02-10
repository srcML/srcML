#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

# escaped xml test

extract_option_xpath_simple ="concat('&lt;' string(//src:decl[string(src:name)='TO_DIR_FLAG'/src:name) '&gt;')"

extract_option_xpath = "concat('&lt;&#33;ENTITY ' string(//src:decl[substring(src:name string-length(src:name) - 4)='_FLAG'/src:name) ' \"--' substring(string(//src:decl[substring(src:name string-length(src:name) - 4)='_FLAG'/src:init/src:expr) 2 string-length(string(//src:decl[substring(src:name string-length(src:name) - 4)='_FLAG'/src:init/src:expr)) - 2) '\"&gt;')"

extract_option_xpath_output_simple = STDOUT<TO_DIR_FLAG>
	STDOUT

extract_option_xpath_output = STDOUT<!ENTITY TO_DIR_FLAG "--to-dir">
	STDOUT

extract_options_output = STDOUT<!ENTITY TO_DIR_FLAG "--to-dir">
<!ENTITY TO_DIR_FLAG_SHORT '-a'>
<!ENTITY UNIT_FLAG "--unit">
<!ENTITY UNIT_FLAG_SHORT '-U'>
<!ENTITY XML_FLAG "--xml">
<!ENTITY XML_FLAG_SHORT '-X'>
<!ENTITY INFO_FLAG "--info">
<!ENTITY INFO_FLAG_SHORT '-i'>
<!ENTITY LONG_INFO_FLAG "--longinfo">
<!ENTITY LONG_INFO_FLAG_SHORT '-L'>
<!ENTITY NAMESPACE_FLAG "--prefix">
<!ENTITY NAMESPACE_FLAG_SHORT '-p'>
<!ENTITY OMIT_FLAG "--omit">
<!ENTITY OMIT_FLAG_SHORT -o>
<!ENTITY XPATH_FLAG "--xpath">
<!ENTITY XSLT_FLAG "--xslt">
<!ENTITY PARAM_FLAG "--xpathparam">
<!ENTITY STRING_PARAM_FLAG "--param">
<!ENTITY APPLY_ROOT_FLAG "--apply-root">
<!ENTITY RELAXNG_FLAG "--relaxng">
<!ENTITY CONTEXT_FLAG "--context">
<!ENTITY LIST_FLAG "--list">
<!ENTITY REGISTER_EXTENSION_FLAG "--register-ext">
<!ENTITY REGISTER_EXTENSION_FILE_FLAG "--register-ext-file">
<!ENTITY REGISTER_EXTENSION_FUNCTION_FLAG "--register-xpath-func">
<!ENTITY REGISTER_EXTENSION_FUNCTION_FILE_FLAG "--register-xpath-func-file">
<!ENTITY EOL_FLAG "--eol">
	STDOUT

srcml2src --xpath extract_option_xpath_simple 'extract_options/extract_options_test.cpp.xml' '' extract_option_xpath_output_simple)
srcml2src --xpath extract_option_xpath 'extract_options/extract_options_test.cpp.xml' '' extract_option_xpath_output)
srcml2src --xslt 'extract_options/extract_options.xsl' 'extract_options/extract_options_test.cpp.xml' '' extract_options_output)
