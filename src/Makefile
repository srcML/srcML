##
# Makefile for srcML translator, src2srcml and srcml2src
#

# set compiler version to match that of antlr library
CXX=/usr/bin/g++

# library
AR=/usr/bin/ar

# associated text tools
SED=${firstword ${wildcard /opt/local/bin/gsed /bin/sed}}
GREP=${firstword ${wildcard /bin/grep /usr/bin/grep}}
ECHO=/bin/echo
CUT=/usr/bin/cut

VERSION="\"Trunk $(shell svnversion -n)$(shell svn info | ${GREP} 'Last Changed Date:' | ${CUT} -d: -f2-)\""

# project dirs
EXE_DIR = ../bin/
OBJ_DIR = ../obj/

# general ANTLR settings

# antlr tool
ANTLR=${firstword ${wildcard /usr/bin/antlr /usr/bin/runantlr /usr/bin/cantlr /opt/local/bin/antlr}}

# antlr c++ include files
ifndef MINGW32
ANTLR_INC_DIR=${dir ${firstword ${wildcard /usr/include/antlr /opt/local/include/antlr}}}
ANTLR_INC=-I${ANTLR_INC_DIR}
endif

# antlr c++ library
ifndef MINGW32
ANTLR_LIB_DIR=${dir ${firstword ${wildcard /usr/lib64/libantlr* /usr/lib/libantlr* /opt/local/lib/libantlr*}}}
ANTLR_LIB=-L${ANTLR_LIB_DIR}
endif 
ANTLR_LIB += -lantlr

LDFLAGS=$(ANTLR_LIB)

# ANTLR processing flags
SRCMLPARSER_ANTLR_FLAGS =
SRCMLLEXER_ANTLR_FLAGS =

# compiler optimizations
CXXFLAGS = -DVERSION=$(VERSION) -O3 -Wall -march=nocona -minline-all-stringops #-fno-rtti

# compiler gcc linking flags
CXX_LINK_FLAGS = #-s

# libxml
LIBXML_INCLUDE=-I/usr/include/libxml2

ifndef MINGW32
XML_LIB_SRC2SRCML=-lxml2
XML_LIB_SRCML2SRC=-lxml2 -lxslt -lexslt
else
XML_LIB_SRC2SRCML=-lxml2 -lregex -liberty
XML_LIB_SRCML2SRC=-lxml2 -lxslt -lexslt -lregex -liberty
endif

# libarchive
XML_IO_LIB=-larchive
IO_LIB_DEPENDS=libxml_archive_read.hpp libxml_archive_write.hpp
IO_LIB=${OBJ_DIR}libxml_archive_read.o ${OBJ_DIR}libxml_archive_write.o

ifndef MINGW32
SRC2SRCML_BIN=${EXE_DIR}src2srcml
SRCML2SRC_BIN=${EXE_DIR}srcml2src
else
SRC2SRCML_BIN=${EXE_DIR}src2srcml.exe
SRCML2SRC_BIN=${EXE_DIR}srcml2src.exe
endif

# default
all : ${SRC2SRCML_BIN} ${SRCML2SRC_BIN} ${EXE_DIR}srcMLdirect

mingw32 :
	make MINGW32=1 CXX=i686-pc-mingw32-g++ AR=i686-pc-mingw32-ar 

settings :
	@echo "CXX="${CXX}
	@echo "SED="${SED}
	@echo "GREP="${GREP}
	@echo "ECHO="${ECHO}
	@echo "MINGW32=|"${MINGW32}"|"
	@echo "ANTLR_LIB_DIR="${ANTLR_LIB_DIR}
	@echo "ANTLR_LIB="${ANTLR_LIB}
	@echo "ANTLR_INC_DIR="${ANTLR_INC_DIR}
	@echo "ANTLR_INC="${ANTLR_INC}
	@echo "XML_LIB_SRC2SRCML="${XML_LIB_SRC2SRCML}
	@echo "XML_LIB_SRCML2SRC="${XML_LIB_SRCML2SRC}
	@echo "SRC2SRCML_BIN="${SRC2SRCML_BIN}
	@echo "SRCML2SRC_BIN="${SRCML2SRC_BIN}

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
${SRC2SRCML_BIN} : ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcmlapps.o ${EXE_DIR}libsrcml.a
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB} ${EXE_DIR}/libsrcml.a -o $@

${EXE_DIR}src2srcmldyn : ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcmlapps.o ${EXE_DIR}libsrcml.so
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB} -L${EXE_DIR} -lsrcml -o $@

${EXE_DIR}libsrcml.so : ${OBJ_DIR}srcMLParserClean.o ${OBJ_DIR}KeywordCPPLexerClean.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}UTF8CharBuffer.o ${OBJ_DIR}PureCommentLexerClean.o ${OBJ_DIR}Language.o ${OBJ_DIR}URIStream.o ${IO_LIB}
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) -shared -fPIC $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB} -o $@

${EXE_DIR}libsrcml.a : ${OBJ_DIR}srcMLParserClean.o ${OBJ_DIR}KeywordCPPLexerClean.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}UTF8CharBuffer.o ${OBJ_DIR}PureCommentLexerClean.o ${OBJ_DIR}Language.o ${OBJ_DIR}URIStream.o ${IO_LIB}
	${AR} rvs $@ $^ 

# srcML to source code translator program
${SRCML2SRC_BIN} : ${OBJ_DIR}srcml2src.o ${OBJ_DIR}srcMLUtility.o ${OBJ_DIR}SAX2ExtractUnitsSrc.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}SAX2Utilities.o ${OBJ_DIR}SAX2UnitDOM.o ${OBJ_DIR}SAX2UnitDOMXSLT.o ${OBJ_DIR}SAX2UnitDOMXPath.o ${OBJ_DIR}SAX2UnitDOMRelaxNG.o ${OBJ_DIR}srcexfun.o ${IO_LIB}
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRCML2SRC} ${XML_IO_LIB} -o $@

# source code lexical analysis
${EXE_DIR}srcmllexer : ${OBJ_DIR}srcmllexer.o ${OBJ_DIR}KeywordCPPLexer.o ${OBJ_DIR}srcmlapps.o
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) -o $@

# Object files
${OBJ_DIR}src2srcml.o : src2srcml.cpp srcMLTranslator.hpp srcmlapps.hpp Options.hpp srcmlns.hpp ${IO_LIB_DEPENDS}
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcml2src.o : srcml2src.cpp srcmlapps.hpp Options.hpp ${IO_LIB_DEPENDS}
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcmllexer.o : srcmllexer.cpp srcmlapps.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcMLTranslator.o : srcMLTranslator.cpp srcMLTranslator.hpp StreamMLParser.hpp StreamParser.hpp srcMLToken.hpp Language.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

Mode.hpp : srcMLStateStack.hpp State.hpp StateStack.hpp srcMLState.hpp

${OBJ_DIR}srcMLParser.o : $(firstword $(SRCMLPARSER_GENS)) Mode.hpp TokenParser.hpp State.hpp Options.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

srcMLParserClean.cpp : srcMLParser.cpp
	$(SED) -f sedantlr < $< > $@

${OBJ_DIR}srcMLParserClean.o : srcMLParserClean.cpp Mode.hpp TokenParser.hpp State.hpp Options.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcMLParser.S : $(firstword $(SRCMLPARSER_GENS))
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -S $(LIBXML_INCLUDE) $< -o $@

KeywordCPPLexerClean.cpp : KeywordCPPLexer.cpp
	$(SED) -f sedantlrlexer < $< > $@

${OBJ_DIR}KeywordCPPLexerClean.o : KeywordCPPLexerClean.cpp Language.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

PureCommentLexerClean.cpp : PureCommentLexer.cpp
	$(SED) -f sedantlrlexer < $< > $@

${OBJ_DIR}PureCommentLexerClean.o : PureCommentLexerClean.cpp Language.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -fno-jump-tables $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}PureCommentLexer.S : $(firstword $(PURECOMMENTLEXER_GENS))
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -S $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}KeywordLexer.o : $(firstword $(KEYWORDLEXER_GENS))
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

srcMLTranslator.hpp : srcMLParserTokenTypes.hpp

${OBJ_DIR}srcMLOutput.o : srcMLOutput.cpp srcMLOutput.hpp srcMLOutputPR.hpp srcMLParserTokenTypesMacro.hpp srcmlns.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}Mode.o : Mode.cpp Mode.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}Language.o : Language.cpp Language.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}UTF8CharBuffer.o : UTF8CharBuffer.cpp UTF8CharBuffer.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcMLUtility.o : srcMLUtility.cpp srcMLUtility.hpp SAX2CountUnits.hpp SAX2ExtractRootSrc.hpp SAX2ExtractUnitSrc.hpp SAX2ExtractUnitsSrc.hpp SAX2ExtractUnitXML.hpp SAX2Properties.hpp SAX2TextWriter.hpp SAX2Utilities.hpp Options.hpp ExtractUnitsSrc.hpp CountUnits.hpp Properties.hpp ExtractUnitsXML.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}URIStream.o : URIStream.cpp URIStream.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}libxml_curl_io.o : libxml_curl_io.cpp libxml_curl_io.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}libxml_archive_read.o : libxml_archive_read.cpp libxml_archive_read.hpp 
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}libxml_archive_write.o : libxml_archive_write.cpp libxml_archive_write.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}libxml_archive_read_disk.o : libxml_archive_read_disk.cpp libxml_archive_read_disk.hpp 
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2TextWriter.o : SAX2TextWriter.cpp SAX2TextWriter.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2CopyElement.o : SAX2CopyElement.cpp SAX2CopyElement.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2CountUnits.o : SAX2CountUnits.cpp SAX2CountUnits.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2Utilities.o : SAX2Utilities.cpp SAX2Utilities.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2Properties.o : SAX2Properties.cpp SAX2Properties.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractUnitSrc.o : SAX2ExtractUnitSrc.cpp SAX2ExtractUnitSrc.hpp ProcessUnit.hpp ExtractUnitsSrc.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractUnitXML.o : SAX2ExtractUnitXML.cpp SAX2ExtractUnitXML.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractUnitsSrc.o : SAX2ExtractUnitsSrc.cpp SAX2ExtractUnitsSrc.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ListUnits.o : SAX2ListUnits.cpp SAX2ListUnits.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractRootSrc.o : SAX2ExtractRootSrc.cpp SAX2ExtractRootSrc.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srceval.o : srceval.cpp srceval.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcexfun.o : srcexfun.cpp srcexfun.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcpatheval.o : srcpatheval.cpp srcpatheval.hpp srceval.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcxslteval.o : srcxslteval.cpp srcxslteval.hpp srceval.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOM.o : SAX2UnitDOM.cpp SAX2UnitDOM.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOMXPath.o : SAX2UnitDOMXPath.cpp SAX2UnitDOMXPath.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOMXSLT.o : SAX2UnitDOMXSLT.cpp SAX2UnitDOMXSLT.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOMRelaxNG.o : SAX2UnitDOMRelaxNG.cpp SAX2UnitDOMRelaxNG.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcrelaxngeval.o : srcrelaxngeval.cpp srcrelaxngeval.hpp srceval.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcmlapps.o : srcmlapps.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}xmlreader.o : xmlreader.cpp xmlreader.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}xmlwriter.o : xmlwriter.cpp xmlwriter.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcMLdirect.o : srcMLdirect.cpp srcMLTranslator.hpp srcmlapps.hpp Options.hpp srcmlns.hpp ${IO_LIB_DEPENDS}
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${EXE_DIR}srcMLdirect : ${OBJ_DIR}srcMLdirect.o ${OBJ_DIR}srcmlapps.o ${EXE_DIR}libsrcml.a
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB} ${EXE_DIR}/libsrcml.a -o $@

# clean all generated files including program
PHONY : clean
clean :
	make cleanbuild
	rm -f ${EXE_DIR}src2srcml ${EXE_DIR}srcml2src

cleanmingw32: 
	make cleanbuild
	rm -rf ${EXE_DIR}src2srcml.exe ${EXE_DIR}srcml2src.exe
# clean generated files leaving program
PHONY : cleanbuild
cleanbuild :
	make cleanantlr
	make cleanobject

# clean non-generated object files
PHONY : cleanobject
cleanobject :
	rm -f ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLParserClean.o ${OBJ_DIR}KeywordLexer.o ${OBJ_DIR}KeywordCPPLexerClean.o ${OBJ_DIR}srcml2src.o ${OBJ_DIR}srcMLUtility.o ${OBJ_DIR}filetrim.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}srcmllexer.o ${OBJ_DIR}xmlwriter.o ${OBJ_DIR}xmlreader.o ${OBJ_DIR}UTF8CharBuffer.o ${OBJ_DIR}PureCommentLexerClean.o ${OBJ_DIR}Language.o ${OBJ_DIR}SAX2TextWriter.o ${OBJ_DIR}SAX2ExtractUnitsSrc.o ${OBJ_DIR}SAX2ExtractUnitSrc.o ${OBJ_DIR}SAX2ExtractRootSrc.o ${OBJ_DIR}SAX2ExtractUnitXML.o ${OBJ_DIR}SAX2Utilities.o ${OBJ_DIR}SAX2Properties.o ${OBJ_DIR}SAX2CountUnits.o srcMLParserTokenTypesMacro.hpp {OBJ_DIR}PureCommentLexer.o ${OBJ_DIR}srceval.o ${OBJ_DIR}srcpatheval.o ${OBJ_DIR}srcxslteval.o ${OBJ_DIR}SAX2CopyElement.o ${OBJ_DIR}srcexfun.o ${OBJ_DIR}srcrelaxngeval.o ${OBJ_DIR}xmlsavebuf.o ${OBJ_DIR}SAX2UnitDOM.o ${OBJ_DIR}SAX2UnitDOMXSLT.o ${OBJ_DIR}SAX2UnitDOMXPath.o ${OBJ_DIR}SAX2UnitDOMRelaxNG.o ${OBJ_DIR}libxml_archive_ui.o ${OBJ_DIR}SAX2ListUnits.o ${OBJ_DIR}libxml_archive_io.o ${OBJ_DIR}URIStream.o ${OBJ_DIR}libxml_archive_read.o ${OBJ_DIR}libxml_archive_read_disk.o ${OBJ_DIR}libxml_archive_write.o

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

srcMLParserTokenTypesMacro.hpp : srcMLParserTokenTypes.hpp
	$(GREP) '=' srcMLParserTokenTypes.hpp | $(SED) -e 's/,//g' -e 's/= //g' -e 's/^[ \t]*//' -e 's/^/#define TOKEN_/g' > $@

PHONY : install
install :
	cp ${EXE_DIR}src2srcml /usr/bin/.
	cp ${EXE_DIR}srcml2src /usr/bin/.
