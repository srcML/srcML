##
# Makefile for testing srcML translator, src2srcml and srcml2src

# Uncomment to generate a static build
#STATIC

# detect OS
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

# 'linux' is output for Linux system, 'darwin' for OS X
DARWIN = $(strip $(findstring DARWIN, $(OSUPPER)))

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

# general ANTLR settings
include Makefile.antlr

# ANTLR processing flags
SRCMLPARSER_ANTLR_FLAGS =
SRCMLLEXER_ANTLR_FLAGS =

# set compiler version to match that of antlr library
CXX = /usr/bin/g++

# compiler optimizations
CXXFLAGS = -O3 -Wall -march=nocona -minline-all-stringops #-fno-rtti

# compiler gcc linking flags
CXX_LINK_FLAGS = #-s

# object code for xml
LIBXML_INCLUDE=-I/usr/include/libxml2
XML_OBJ=
XML_LIB_SRC2SRCML=-lxml2
XML_LIB_SRCML2SRC=-lxml2 -lxslt -lexslt

ifdef ARCHIVE
XML_IO_LIB_SRC2SRCML_DEFINE=-DLIBARCHIVE
XML_IO_LIB_SRC2SRCML=-larchive
XML_IO_LIB_SRCML2SRC=
IO_LIB_SRC2SRCML=${OBJ_DIR}libxml_archive_io.o
else
XML_IO_LIB_SRC2SRCML_DEFINE=
XML_IO_LIB_SRC2SRCML=
XML_IO_LIB_SRCML2SRC=
IO_LIB_SRC2SRCML=
endif

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

ifneq ($(DARWIN),) 
SED=/opt/local/bin/gsed
GREP=/usr/bin/grep
else
SED=/bin/sed
GREP=/bin/grep
endif
ECHO=/bin/echo

# default
all : ${EXE_DIR}src2srcml ${EXE_DIR}srcml2src

# setup
setup : version.cpp

# ANTLR generated files
SRCMLPARSER_GENS = \
	srcMLParser.cpp srcMLParser.hpp \
	srcMLParserTokenTypes.txt srcMLParserTokenTypes.hpp

KEYWORDCPPLEXER_GENS = \
	KeywordCPPLexer.cpp KeywordCPPLexer.hpp \
	KeywordCPPLexerTokenTypes.txt KeywordCPPLexerTokenTypes.hpp

KEYWORDLEXER_GENS = \
	KeywordLexer.cpp KeywordLexer.hpp \
	KeywordLexerTokenTypes.txt KeywordLexerTokenTypes.hpp

TEXTLEXER_GENS = \
	TextLexer.cpp TextLexer.hpp \
	TextLexerTokenTypes.txt TextLexerTokenTypes.hpp

OPERATORLEXER_GENS = \
	OperatorLexer.cpp OperatorLexer.hpp \
	OperatorLexerTokenTypes.txt OperatorLexerTokenTypes.hpp

COMMENTLEXER_GENS = \
	CommentLexer.cpp CommentLexer.hpp \
	CommentLexerTokenTypes.txt CommentLexerTokenTypes.hpp

PURECOMMENTLEXER_GENS = \
	PureCommentLexer.cpp PureCommentLexer.hpp \
	PureCommentLexerTokenTypes.txt PureCommentLexerTokenTypes.hpp

WSLEXER_GENS = 	\
	WSLexer.cpp WSLexer.hpp \
	WSLexerTokenTypes.txt WSLexerTokenTypes.hpp

# ANTLR generated cpp, hpp and txt files
$(SRCMLPARSER_GENS): srcMLParser.g KeywordCPPLexerTokenTypes.txt PureCommentLexerTokenTypes.txt
	$(ANTLR) ${SRCMLPARSER_ANTLR_FLAGS} -glib "WSLexer.g;CommentLexer.g;OperatorLexer.g;TextLexer.g;KeywordLexer.g;KeywordCPPLexer.g;PureCommentLexer.g" $<

$(KEYWORDCPPLEXER_GENS): KeywordCPPLexer.g KeywordLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g;CommentLexer.g;OperatorLexer.g;TextLexer.g;KeywordLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<

$(KEYWORDLEXER_GENS): KeywordLexer.g TextLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g;CommentLexer.g;OperatorLexer.g;TextLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<

$(TEXTLEXER_GENS): TextLexer.g OperatorLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g;CommentLexer.g;OperatorLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<

$(OPERATORLEXER_GENS): OperatorLexer.g CommentLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g;CommentLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<

$(COMMENTLEXER_GENS): CommentLexer.g WSLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<

$(PURECOMMENTLEXER_GENS): PureCommentLexer.g
	$(ANTLR) ${SRCMLLEXER_ANTLR_FLAGS} $<

$(WSLEXER_GENS): WSLexer.g PureCommentLexerTokenTypes.txt
	$(ANTLR) -glib "PureCommentLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $< 

# User created grammars
CREATED_GRAMMARS = WSLexer.g CommentLexer.g OperatorLexer.g TextLexer.g KeywordLexer.g KeywordCPPLexer.g PureCommentLexer.g

# ANTLR generated grammars (name "expanded" at the front)
GENERATED_GRAMMARS = $(patsubst %.g, expanded%.g, $(CREATED_GRAMMARS))

# source code to srcML translator program
${EXE_DIR}src2srcml : ${OBJ_DIR}srcMLParserClean.o ${OBJ_DIR}KeywordCPPLexerClean.o ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}version.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}UTF8CharBuffer.o ${OBJ_DIR}PureCommentLexerClean.o ${OBJ_DIR}Language.o ${OBJ_DIR}URIStream.o ${IO_LIB_SRC2SRCML}
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB_SRC2SRCML} -o $@

# srcML to source code translator program
${EXE_DIR}srcml2src : ${OBJ_DIR}srcml2src.o ${OBJ_DIR}srcMLUtility.o ${OBJ_DIR}SAX2TextWriter.o ${OBJ_DIR}SAX2ExtractUnitsSrc.o ${OBJ_DIR}SAX2ExtractUnitSrc.o ${OBJ_DIR}SAX2ExtractRootSrc.o ${OBJ_DIR}SAX2ExtractUnitXML.o ${OBJ_DIR}version.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}SAX2Utilities.o ${OBJ_DIR}SAX2Properties.o ${OBJ_DIR}SAX2CountUnits.o ${XML_OBJ} ${OBJ_DIR}SAX2UnitDOM.o ${OBJ_DIR}SAX2UnitDOMXSLT.o ${OBJ_DIR}SAX2UnitDOMXPath.o ${OBJ_DIR}SAX2UnitDOMRelaxNG.o ${OBJ_DIR}srcexfun.o
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRCML2SRC} ${XML_IO_LIB_SRCML2SRC} -o $@

# source code lexical analysis
${EXE_DIR}srcmllexer : ${OBJ_DIR}srcmllexer.o ${OBJ_DIR}KeywordCPPLexer.o ${OBJ_DIR}version.o ${OBJ_DIR}srcmlapps.o
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) -o $@

# Object files
${OBJ_DIR}src2srcml.o : src2srcml.cpp srcMLTranslator.h version.h srcmlapps.h Options.h srcmlns.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(XML_IO_LIB_SRC2SRCML_DEFINE) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcml2src.o : srcml2src.cpp version.h srcmlapps.h Options.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcmllexer.o : srcmllexer.cpp version.h srcmlapps.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcMLTranslator.o : srcMLTranslator.cpp srcMLTranslator.h StreamMLParser.h StreamParser.h srcMLToken.h Language.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

Mode.h : srcMLStateStack.h State.h StateStack.h srcMLState.h

${OBJ_DIR}srcMLParser.o : $(firstword $(SRCMLPARSER_GENS)) Mode.h TokenParser.h State.h Options.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

srcMLParserClean.cpp : srcMLParser.cpp
	$(SED) -f sedantlr < $< > $@

${OBJ_DIR}srcMLParserClean.o : srcMLParserClean.cpp Mode.h TokenParser.h State.h Options.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcMLParser.S : $(firstword $(SRCMLPARSER_GENS))
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -S $(LIBXML_INCLUDE) $< -o $@

KeywordCPPLexerClean.cpp : KeywordCPPLexer.cpp
	$(SED) -f sedantlrlexer < $< > $@

${OBJ_DIR}KeywordCPPLexerClean.o : KeywordCPPLexerClean.cpp Language.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

PureCommentLexerClean.cpp : PureCommentLexer.cpp
	$(SED) -f sedantlrlexer < $< > $@

${OBJ_DIR}PureCommentLexerClean.o : PureCommentLexerClean.cpp Language.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -fno-jump-tables $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}PureCommentLexer.S : $(firstword $(PURECOMMENTLEXER_GENS))
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -S $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}KeywordLexer.o : $(firstword $(KEYWORDLEXER_GENS))
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcMLOutput.o : srcMLOutput.cpp srcMLOutput.h srcMLOutputPR.h srcMLParserTokenTypesMacro.hpp srcmlns.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}Mode.o : Mode.cpp Mode.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}Language.o : Language.cpp Language.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}UTF8CharBuffer.o : UTF8CharBuffer.cpp UTF8CharBuffer.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcMLUtility.o : srcMLUtility.cpp srcMLUtility.h SAX2CountUnits.h SAX2ExtractRootSrc.h SAX2ExtractUnitSrc.h SAX2ExtractUnitsSrc.h SAX2ExtractUnitXML.h SAX2Properties.h SAX2TextWriter.h SAX2Utilities.h Options.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}URIStream.o : URIStream.cpp URIStream.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}libxml_curl_io.o : libxml_curl_io.cpp libxml_curl_io.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}libxml_archive_io.o : libxml_archive_io.cpp libxml_archive_io.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2TextWriter.o : SAX2TextWriter.cpp SAX2TextWriter.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2CopyElement.o : SAX2CopyElement.cpp SAX2CopyElement.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2CountUnits.o : SAX2CountUnits.cpp SAX2CountUnits.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2Utilities.o : SAX2Utilities.cpp SAX2Utilities.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2Properties.o : SAX2Properties.cpp SAX2Properties.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractUnitSrc.o : SAX2ExtractUnitSrc.cpp SAX2ExtractUnitSrc.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractUnitXML.o : SAX2ExtractUnitXML.cpp SAX2ExtractUnitXML.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractUnitsSrc.o : SAX2ExtractUnitsSrc.cpp SAX2ExtractUnitsSrc.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractRootSrc.o : SAX2ExtractRootSrc.cpp SAX2ExtractRootSrc.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srceval.o : srceval.cpp srceval.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcexfun.o : srcexfun.cpp srcexfun.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcpatheval.o : srcpatheval.cpp srcpatheval.h srceval.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcxslteval.o : srcxslteval.cpp srcxslteval.h srceval.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOM.o : SAX2UnitDOM.cpp SAX2UnitDOM.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOMXPath.o : SAX2UnitDOMXPath.cpp SAX2UnitDOMXPath.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOMXSLT.o : SAX2UnitDOMXSLT.cpp SAX2UnitDOMXSLT.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOMRelaxNG.o : SAX2UnitDOMRelaxNG.cpp SAX2UnitDOMRelaxNG.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcrelaxngeval.o : srcrelaxngeval.cpp srcrelaxngeval.h srceval.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}version.o : version.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcmlapps.o : srcmlapps.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}xmlreader.o : xmlreader.cpp xmlreader.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}xmlwriter.o : xmlwriter.cpp xmlwriter.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# clean all generated files including program
PHONY : clean
clean :
	make cleanbuild
	rm -f ${EXE_DIR}src2srcml ${EXE_DIR}srcml2src version.cpp

# clean generated files leaving program
PHONY : cleanbuild
cleanbuild :
	make cleanantlr
	make cleanobject

# clean non-generated object files
PHONY : cleanobject
cleanobject :
	rm -f ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLParserClean.o ${OBJ_DIR}KeywordLexer.o ${OBJ_DIR}KeywordCPPLexerClean.o ${OBJ_DIR}srcml2src.o ${OBJ_DIR}srcMLUtility.o ${OBJ_DIR}version.o ${OBJ_DIR}filetrim.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}srcmllexer.o ${OBJ_DIR}xmlwriter.o ${OBJ_DIR}xmlreader.o ${OBJ_DIR}UTF8CharBuffer.o ${OBJ_DIR}PureCommentLexerClean.o ${OBJ_DIR}Language.o ${OBJ_DIR}SAX2TextWriter.o ${OBJ_DIR}SAX2ExtractUnitsSrc.o ${OBJ_DIR}SAX2ExtractUnitSrc.o ${OBJ_DIR}SAX2ExtractRootSrc.o ${OBJ_DIR}SAX2ExtractUnitXML.o ${OBJ_DIR}SAX2Utilities.o ${OBJ_DIR}SAX2Properties.o ${OBJ_DIR}SAX2CountUnits.o version.cpp srcMLParserTokenTypesMacro.hpp {OBJ_DIR}PureCommentLexer.o ${OBJ_DIR}srceval.o ${OBJ_DIR}srcpatheval.o ${OBJ_DIR}srcxslteval.o ${OBJ_DIR}SAX2CopyElement.o ${OBJ_DIR}srcexfun.o ${OBJ_DIR}srcrelaxngeval.o ${OBJ_DIR}xmlsavebuf.o ${OBJ_DIR}SAX2UnitDOM.o ${OBJ_DIR}SAX2UnitDOMXSLT.o ${OBJ_DIR}SAX2UnitDOMXPath.o ${OBJ_DIR}SAX2UnitDOMRelaxNG.o ${OBJ_DIR}libxml_archive_ui.o

# clean ANTLR generated files
PHONY : cleanantlr
cleanantlr :
	rm -f $(SRCMLPARSER_GENS)
	rm -f $(COMMENTLEXER_GENS)
	rm -f $(OPERATORLEXER_GENS)
	rm -f $(KEYWORDCPPLEXER_GENS)
	rm -f $(KEYWORDLEXER_GENS)
	rm -f $(TEXTLEXER_GENS)
	rm -f $(WSLEXER_GENS)
	rm -f $(GENERATED_GRAMMARS)
	rm -f $(PURECOMMENTLEXER_GENS)
	rm -f PureCommentLexerClean.cpp
	rm -f KeywordCPPLexerClean.cpp
	rm -f srcMLParserClean.cpp


# on every build, record the version number
version.cpp : VERSION
	$(ECHO) 'const char* version() { const char* Version = "$(shell cat VERSION)"; return Version; }'   > $@

# update trunk version
PHONY : trunkversion
trunkversion :
	$(ECHO) -n "Trunk " > VERSION
	$(ECHO) -n `date` >> VERSION

srcMLParserTokenTypesMacro.hpp : srcMLParserTokenTypes.hpp
	$(GREP) '=' srcMLParserTokenTypes.hpp | $(SED) -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/#define TOKEN_/g' > $@

PHONY : install
install :
	cp ../bin/src2srcml /usr/bin/.
	cp ../bin/srcml2src /usr/bin/.
