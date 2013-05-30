cmake_minimum_required(VERSION 2.8)
project(srcML)

execute_process(COMMAND cat VERSION COMMAND tr -d "\n" OUTPUT_VARIABLE VERSION)
execute_process(COMMAND cat REVISION COMMAND tr -d "\n" OUTPUT_VARIABLE REVISION)
add_definitions(-DVERSION=\"${VERSION}\")
add_definitions(-DREVISION=\"${REVISION}\")

include_directories(/usr/include/libxml2)

add_library(srcml srcMLParserClean.cpp KeywordCPPLexerClean.cpp srcMLTranslator.cpp srcMLTranslatorCore.cpp srcMLOutput.cpp srcMLTranslatorOutput.cpp Mode.cpp UTF8CharBuffer.cpp PureCommentLexerClean.cpp Language.cpp URIStream.cpp srcmlapps.cpp libxml_archive_read.cpp libxml_archive_write.cpp)


add_executable(src2srcml src2srcml.cpp srcmlapps.cpp makeargv.cpp)
target_link_libraries(src2srcml archive xml2 antlr srcml)

add_executable(srcml2src srcml2src.cpp srcMLUtility.cpp SAX2ExtractUnitsSrc.cpp srcmlapps.cpp SAX2Utilities.cpp SAX2UnitDOM.cpp SAX2UnitDOMRelaxNG.cpp srcexfun.cpp libxml_archive_read.cpp libxml_archive_write.cpp URIStream.cpp makeargv.cpp)
target_link_libraries(srcml2src archive xml2 xslt exslt)

set_target_properties(src2srcml srcml2src PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)
