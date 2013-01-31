##
# Makefile for srcML translator, src2srcml and srcml2src
#

# set compiler version to match that of antlr library
CXX=/usr/bin/g++

# library
AR=/usr/bin/ar

ifdef MINGW32

CXX=i686-pc-mingw32-g++
AR=i686-pc-mingw32-ar

ifneq (${wildcard /etc/fedora-release},)

ifneq ($(shell cat /etc/fedora-release | grep "17"),)
CXX=i686-w64-mingw32-g++
AR=i686-w64-mingw32-ar
endif

endif

endif

# associated text tools
SED=${firstword ${wildcard /opt/local/bin/gsed /usr/local/bin/gsed /bin/sed}}
GREP=${firstword ${wildcard /bin/grep /usr/bin/grep}}
ECHO=/bin/echo
CUT=/usr/bin/cut
SVNVERSION=${firstword ${wildcard /usr/local/bin/svnversion /usr/bin/svnversion }}

ifeq ($(SVNVERSION),)
VERSION="\"$(shell cat VERSION)\""
REVISION="\"$(shell cat REVISION)\""
else

ifeq ($(shell ${SVNVERSION} -n),Unversioned directory)
VERSION="\"$(shell cat VERSION)\""
REVISION="\"$(shell cat REVISION)\""
else
ifeq ($(shell ${SVNVERSION} -n),exported)
VERSION="\"$(shell cat VERSION)\""
REVISION="\"$(shell cat REVISION)\""
else
VERSION="\"Trunk $(shell ${SVNVERSION} -n)$(shell svn info | ${GREP} 'Last Changed Date:' | ${CUT} -d: -f2-)\""
$(shell echo ${VERSION} | ${CUT} -d \" -f2 > VERSION)
REVISION="\"$(shell ${SVNVERSION} -n)\""
endif

endif

endif

# project dirs
EXE_DIR = ../bin/
OBJ_DIR = ../obj/
DLL_DIR = ../dlls

# general ANTLR settings

# antlr tool
ANTLR=${firstword ${wildcard /usr/bin/antlr /usr/bin/runantlr /usr/bin/cantlr /opt/local/bin/antlr /usr/local/bin/antlr2}}

# antlr c++ include files
ifndef MINGW32
ANTLR_INC_DIR=${dir ${firstword ${wildcard /usr/include/antlr /opt/local/include/antlr /usr/local/include/antlr}}}
ANTLR_INC=-I${ANTLR_INC_DIR}
endif

# antlr c++ library
ifndef MINGW32
ANTLR_LIB_DIR=${dir ${firstword ${wildcard /usr/lib64/libantlr* /usr/lib/libantlr* /opt/local/lib/libantlr* /usr/local/lib/libantlr*}}}
ANTLR_LIB=-L${ANTLR_LIB_DIR}
endif 
ANTLR_LIB += -lantlr

LDFLAGS=$(ANTLR_LIB)

# ANTLR processing flags
SRCMLPARSER_ANTLR_FLAGS =
SRCMLLEXER_ANTLR_FLAGS =

# compiler optimizations
CXXFLAGS = -DVERSION=$(VERSION) -DREVISION=$(REVISION) -O3 -Wall -march=nocona -minline-all-stringops $(if $(DYNAMIC), -fPIC,) $(if $(DEBUG), -g,) $(if $(DYNAMIC), -fPIC,) $(if $(SAXFRAMEWORK), -DSAXFRAMEWORK,) #-fno-rtti 

# compiler gcc linking flags
CXX_LINK_FLAGS = #-s

# libxml
LIBXML_INC_DIR=${firstword ${wildcard /opt/local/include/libxml2 /usr/include/libxml2}}
LIBXML_INCLUDE=-I$(LIBXML_INC_DIR)

# libapr
LIBAPR_INC_DIR=${firstword ${wildcard /usr/include/apr-1/ /usr/include/apr}}
LIBAPR_INCLUDE=-I$(LIBAPR_INC_DIR)

# libsvn
LIBSVN_INC_DIR=${firstword ${wildcard /usr/local/include/subversion-1 /usr/include/subversion}}
LIBSVN_INCLUDE=-I$(LIBSVN_INC_DIR)

ifndef MINGW32
XML_LIB_SRC2SRCML=$(if $(DEBUG),-L /usr/lib/debug/usr/lib/,-lxml2)
XML_LIB_SRCML2SRC=$(if $(DEBUG),-L /usr/lib/debug/usr/lib/,-lxml2) -lxslt -lexslt
else
XML_LIB_SRC2SRCML=${DLL_DIR}/libxml2.dll ${DLL_DIR}/bz2-1.dll -lregex
XML_LIB_SRCML2SRC=${DLL_DIR}/libxml2.dll ${DLL_DIR}/libxslt.dll ${DLL_DIR}/libexslt.dll -lregex -liberty
#XML_LIB_SRCML2SRC=-lxml2 -lxslt -lexslt -lregex -liberty
endif

# libarchive
XML_IO_LIB=-larchive
IO_LIB_DEPENDS=libxml_archive_read.hpp libxml_archive_write.hpp
IO_LIB=${OBJ_DIR}libxml_archive_read.o ${OBJ_DIR}libxml_archive_write.o

ifndef MINGW32
SRC2SRCML_BIN=${EXE_DIR}src2srcml
SRCML2SRC_BIN=${EXE_DIR}srcml2src
SRCMLDIRECT_BIN=${EXE_DIR}srcmldirect
LIBSRCML_SO=${EXE_DIR}libsrcml.so
else
SRC2SRCML_BIN=${EXE_DIR}src2srcml.exe
SRCML2SRC_BIN=${EXE_DIR}srcml2src.exe
LIBSRCML_SO=${EXE_DIR}libsrcml.dll
SRCMLDIRECT_BIN=${EXE_DIR}srcmldirect.exe
endif

LIBSRCML_INCLUDE=${OBJ_DIR}srcMLParserClean.o ${OBJ_DIR}KeywordCPPLexerClean.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLTranslatorCore.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}srcMLTranslatorOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}UTF8CharBuffer.o ${OBJ_DIR}PureCommentLexerClean.o ${OBJ_DIR}Language.o ${OBJ_DIR}URIStream.o ${OBJ_DIR}srcmlapps.o ${IO_LIB}

# default
.PHONY: all
all : ${SRC2SRCML_BIN} ${SRCML2SRC_BIN}  ${SRCMLDIRECT_BIN}

.PHONY: mingw32
mingw32 :
	make MINGW32=1

.PHONY: settings
settings :
	@echo "CXX="${CXX}
	@echo "CXXFLAGS="${CXXFLAGS}
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
.PHONY: setup
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
	touch ${SRCMLPARSER_GENS}

$(KEYWORDCPPLEXER_GENS): KeywordCPPLexer.g KeywordLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g;CommentLexer.g;OperatorLexer.g;TextLexer.g;KeywordLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<
	touch ${KEYWORDCPPLEXER_GENS}

$(KEYWORDLEXER_GENS): KeywordLexer.g OperatorLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g;CommentLexer.g;TextLexer.g;OperatorLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<
	touch ${KEYWORDLEXER_GENS}

$(OPERATORLEXER_GENS): OperatorLexer.g TextLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g;CommentLexer.g;TextLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<
	touch ${OPERATORLEXER_GENS}

$(TEXTLEXER_GENS): TextLexer.g CommentLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g;CommentLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<
	touch ${TEXTLEXER_GENS}

$(COMMENTLEXER_GENS): CommentLexer.g WSLexerTokenTypes.txt
	$(ANTLR) -glib "WSLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $<
	touch ${COMMENTLEXER_GENS}

$(PURECOMMENTLEXER_GENS): PureCommentLexer.g
	$(ANTLR) ${SRCMLLEXER_ANTLR_FLAGS} $<
	touch ${PURECOMMENTLEXER_GENS}

$(WSLEXER_GENS): WSLexer.g PureCommentLexerTokenTypes.txt
	$(ANTLR) -glib "PureCommentLexer.g" ${SRCMLLEXER_ANTLR_FLAGS} $< 
	touch ${WSLEXER_GENS}

# User created grammars
CREATED_GRAMMARS = WSLexer.g CommentLexer.g OperatorLexer.g TextLexer.g KeywordLexer.g KeywordCPPLexer.g PureCommentLexer.g

# ANTLR generated grammars (name "expanded" at the front)
GENERATED_GRAMMARS = $(patsubst %.g, expanded%.g, $(CREATED_GRAMMARS))

# source code to srcML translator program
${SRC2SRCML_BIN} : ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}makeargv.o $(if $(DYNAMIC), ${LIBSRCML_SO}, ${EXE_DIR}libsrcml.a)
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB} $(if $(DYNAMIC), -L${EXE_DIR} -lsrcml, ) -o $@

# shared library for srcML translation
${LIBSRCML_SO}: $(LIBSRCML_INCLUDE)
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) -shared -fPIC $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB} -o $@

# static library for srcML translation
${EXE_DIR}libsrcml.a : $(LIBSRCML_INCLUDE)
	rm -f $@
	${AR} rvs $@ $^ 

# srcML to source code translator program
${SRCML2SRC_BIN} : ${OBJ_DIR}srcml2src.o ${OBJ_DIR}srcMLUtility.o ${OBJ_DIR}SAX2ExtractUnitsSrc.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}SAX2Utilities.o ${OBJ_DIR}SAX2UnitDOM.o ${OBJ_DIR}SAX2UnitDOMRelaxNG.o ${OBJ_DIR}srcexfun.o ${IO_LIB} ${OBJ_DIR}URIStream.o ${OBJ_DIR}makeargv.o
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRCML2SRC} ${XML_IO_LIB} -o $@

# Object files
${OBJ_DIR}src2srcml.o : src2srcml.cpp srcMLTranslator.hpp srcmlapps.hpp Options.hpp srcmlns.hpp makeargv.hpp ${IO_LIB_DEPENDS}
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcml2src.o : srcml2src.cpp srcmlapps.hpp Options.hpp URIStream.hpp makeargv.hpp ${IO_LIB_DEPENDS}
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcMLTranslator.o : srcMLTranslator.cpp ${OBJ_DIR}srcMLTranslatorCore.o srcMLTranslator.hpp srcMLTranslatorCore.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcMLTranslatorCore.o : srcMLTranslatorCore.cpp srcMLTranslatorCore.hpp StreamMLParser.hpp StreamParser.hpp srcMLToken.hpp Language.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

Mode.hpp : srcMLStateStack.hpp State.hpp StateStack.hpp srcMLState.hpp

${OBJ_DIR}srcMLParser.o : $(SRCMLPARSER_GENS) Mode.hpp TokenParser.hpp State.hpp Options.hpp
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

${OBJ_DIR}srcMLTranslatorOutput.o : srcMLTranslatorOutput.cpp srcMLTranslatorOutput.hpp srcMLOutputPR.hpp srcMLParserTokenTypesMacro.hpp srcmlns.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}Mode.o : Mode.cpp Mode.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}Language.o : Language.cpp Language.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}UTF8CharBuffer.o : UTF8CharBuffer.cpp UTF8CharBuffer.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcMLUtility.o : srcMLUtility.cpp srcMLUtility.hpp SAX2ExtractUnitsSrc.hpp SAX2TextWriter.hpp SAX2Utilities.hpp Options.hpp ExtractUnitsSrc.hpp CountUnits.hpp Properties.hpp ExtractUnitsXML.hpp UnitDOM.hpp XPathQueryUnits.hpp XSLTUnits.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}URIStream.o : URIStream.cpp URIStream.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}libxml_archive_read.o : libxml_archive_read.cpp libxml_archive_read.hpp 
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}libxml_archive_write.o : libxml_archive_write.cpp libxml_archive_write.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}process_svn.o : process_svn.cpp process_svn.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBSVN_INCLUDE) $(LIBAPR_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2TextWriter.o : SAX2TextWriter.cpp SAX2TextWriter.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2CopyElement.o : SAX2CopyElement.cpp SAX2CopyElement.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2Utilities.o : SAX2Utilities.cpp SAX2Utilities.hpp UnitDOM.hpp XPathQueryUnits.hpp 
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2ExtractUnitsSrc.o : SAX2ExtractUnitsSrc.cpp SAX2ExtractUnitsSrc.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcexfun.o : srcexfun.cpp srcexfun.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOM.o : SAX2UnitDOM.cpp SAX2UnitDOM.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}SAX2UnitDOMRelaxNG.o : SAX2UnitDOMRelaxNG.cpp SAX2UnitDOMRelaxNG.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcmlapps.o : srcmlapps.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}xmlreader.o : xmlreader.cpp xmlreader.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}xmlwriter.o : xmlwriter.cpp xmlwriter.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}makeargv.o : makeargv.cpp makeargv.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcmldirect.o : srcmldirect.cpp srcMLTranslator.hpp srcmlapps.hpp Options.hpp srcmlns.hpp
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

ifndef MINGW32
${SRCMLDIRECT_BIN} : ${OBJ_DIR}srcmldirect.o ${OBJ_DIR}srcmlapps.o ${EXE_DIR}libsrcml.a
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB} -o $@
else
${SRCMLDIRECT_BIN} : ${OBJ_DIR}srcmldirect.o ${OBJ_DIR}srcmlapps.o ${LIBSRCML_SO}
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB_SRC2SRCML} ${XML_IO_LIB} -o $@
endif

# clean all generated files including program
PHONY : clean
clean :
	make cleanbuild
	rm -f ${EXE_DIR}src2srcml ${EXE_DIR}srcml2src ${EXE_DIR}srcmldirect ${EXE_DIR}libsrcml.a ${EXE_DIR}libsrcml.so

PHONY : cleanmingw32
cleanmingw32: 
	make cleanbuild
	rm -rf ${EXE_DIR}src2srcml.exe ${EXE_DIR}srcml2src.exe ${EXE_DIR}srcmldirect.exe

# clean generated files leaving program
PHONY : cleanbuild
cleanbuild :
	make cleanantlr
	make cleanobject

# clean non-generated object files
PHONY : cleanobject
cleanobject :
	rm -f ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}srcMLTranslatorOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLParserClean.o ${OBJ_DIR}KeywordLexer.o ${OBJ_DIR}KeywordCPPLexerClean.o ${OBJ_DIR}srcml2src.o ${OBJ_DIR}srcMLUtility.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}srcmllexer.o ${OBJ_DIR}xmlwriter.o ${OBJ_DIR}xmlreader.o ${OBJ_DIR}UTF8CharBuffer.o ${OBJ_DIR}PureCommentLexerClean.o ${OBJ_DIR}Language.o ${OBJ_DIR}SAX2TextWriter.o ${OBJ_DIR}SAX2ExtractUnitsSrc.o ${OBJ_DIR}SAX2ExtractUnitXML.o ${OBJ_DIR}SAX2Utilities.o srcMLParserTokenTypesMacro.hpp {OBJ_DIR}PureCommentLexer.o ${OBJ_DIR}srceval.o ${OBJ_DIR}srcxslteval.o ${OBJ_DIR}SAX2CopyElement.o ${OBJ_DIR}srcexfun.o ${OBJ_DIR}srcrelaxngeval.o ${OBJ_DIR}xmlsavebuf.o ${OBJ_DIR}SAX2UnitDOM.o ${OBJ_DIR}SAX2UnitDOMRelaxNG.o ${OBJ_DIR}libxml_archive_ui.o ${OBJ_DIR}libxml_archive_io.o ${OBJ_DIR}URIStream.o ${OBJ_DIR}libxml_archive_read.o ${OBJ_DIR}libxml_archive_write.o ${OBJ_DIR}srcmldirect.o ${OBJ_DIR}makeargv.o ${OBJ_DIR}srcMLTranslatorCore.o

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
