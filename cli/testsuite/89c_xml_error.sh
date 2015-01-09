#!/bin/bash

# test framework
source $(dirname "$0")/framework_test.sh

# test

# xml error test
info_single = STDOUTxmlns="http://www.sdml.info/srcML/src"
xmlns:cpp="http://www.sdml.info/srcML/cpp"
encoding="UTF-8"
language="C++"
	STDOUT

info_archive = STDOUTxmlns="http://www.sdml.info/srcML/src"
encoding="UTF-8"
	STDOUT
# ok
srcml2src --info 'xml_error/illformed.xml' '' info_single)
srcml2src --info 'xml_error/illformedarchive.xml' '' info_archive)

xml_error = STDOUTsrcml2src: expected '>' in 'xml_error/illformed.xml'
	STDOUT

xml_archive_error = STDOUTsrcml2src: Extra content at the end of the document in 'xml_error/illformedarchive.xml'
	STDOUT

# bad
checkError([srcml2src 'xml_error/illformed.xml' '' xml_error)
checkError([srcml2src --units 'xml_error/illformedarchive.xml' '' xml_archive_error)

