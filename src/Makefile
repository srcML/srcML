##
# Makefile for testing srcML translator, src2srcml and srcml2src

# Uncomment to generate non-libxml version
#LIBXML_DISABLED=a

# Uncomment to generate a static build
#STATIC

EXE_DIR = ../bin/
OBJ_DIR = ../obj/

# general ANTLR settings
include Makefile.antlr

# ANTLR processing flags
SRCMLPARSER_ANTLR_FLAGS =
SRCMLLEXER_ANTLR_FLAGS =

# compiler processing flags
SRCMLPARSER_CXX_FLAGS = 

# set compiler version to match that of antlr library
CXX = /usr/bin/g++

# compiler optimizations
CXXFLAGS = -O3 -Wall -march=nocona -minline-all-stringops -fno-rtti
#-fno-enforce-eh-specs

# compiler optimizations
ifdef STATIC
CXXFLAGS = -O3 -Wall -march=nocona -minline-all-stringops -fno-rtti -static-libgcc -L ../lib
else
CXXFLAGS = -O3 -Wall -march=nocona -minline-all-stringops -fno-rtti
endif

# compiler gcc linking flags
CXX_LINK_FLAGS = -s

# object code for xml
ifdef LIBXML_DISABLED
LIBXML_INCLUDE=-I .
XML_OBJ=${OBJ_DIR}xmlwriter.o ${OBJ_DIR}xmlreader.o
XML_LIB=
XML_PRE=
LIBXML_VERSION=
else
LIBXML_INCLUDE=-I/usr/include/libxml2 -I/usr/include/libxml2/libxml
XML_OBJ=
XML_LIB=-lxml2
XML_PRE=-DLIBXML_ENABLED
LIBXML_VERSION= Libxml2 enabled
endif

SRC2SRCML_BIN=../bin/src2srcml
SRCML2SRC_BIN=../bin/srcml2src

TIMING_MED=timing_med.h
TIMING_JAVA=timing_java.java

# default
all : ${EXE_DIR}src2srcml ${EXE_DIR}srcml2src

# setup
setup : version.cpp

# EOF check for open tags
asserteof :
	make cleanobject
	make SRCMLPARSER_CXX_FLAGS="-DASSERT_EOF -DASSERT_ENDMODE -DODEBUG"

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

WSLEXER_GENS = 	\
	WSLexer.cpp WSLexer.hpp \
	WSLexerTokenTypes.txt WSLexerTokenTypes.hpp

# ANTLR generated cpp, hpp and txt files
$(SRCMLPARSER_GENS): srcMLParser.g KeywordCPPLexerTokenTypes.txt
	$(ANTLR) ${SRCMLPARSER_ANTLR_FLAGS} -glib "WSLexer.g;CommentLexer.g;OperatorLexer.g;TextLexer.g;KeywordLexer.g;KeywordCPPLexer.g" $<

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

$(WSLEXER_GENS): WSLexer.g
	$(ANTLR) ${SRCMLLEXER_ANTLR_FLAGS} $< 

# User created grammars
CREATED_GRAMMARS = WSLexer.g CommentLexer.g OperatorLexer.g TextLexer.g KeywordLexer.g KeywordCPPLexer.g

# ANTLR generated grammars (name "expanded" at the front)
GENERATED_GRAMMARS = $(patsubst %.g, expanded%.g, $(CREATED_GRAMMARS))

# source code to srcML translator program
${EXE_DIR}src2srcml : ${OBJ_DIR}srcMLParser.o ${OBJ_DIR}KeywordCPPLexer.o ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}version.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}UTF8CharBuffer.o ${XML_OBJ}
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB} -o $@

# srcML to source code translator program
${EXE_DIR}srcml2src : ${OBJ_DIR}srcml2src.o ${OBJ_DIR}srcMLUtility.o ${OBJ_DIR}xmloutput.o ${OBJ_DIR}version.o ${OBJ_DIR}srcmlapps.o ${XML_OBJ}
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) ${XML_LIB} -o $@

# source code lexical analysis
${EXE_DIR}srcmllexer : ${OBJ_DIR}srcmllexer.o ${OBJ_DIR}KeywordCPPLexer.o ${OBJ_DIR}version.o ${OBJ_DIR}srcmlapps.o
	$(CXX) $(CXXFLAGS) $(CXX_LINK_FLAGS) $^ $(LDFLAGS) -o $@

# Object files
${OBJ_DIR}src2srcml.o : src2srcml.cpp srcMLTranslator.h version.h srcmlapps.h Options.h srcmlns.h
	$(CXX) $(XML_PRE) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcml2src.o : srcml2src.cpp version.h srcmlapps.h Options.h
	$(CXX) $(XML_PRE) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}srcmllexer.o : srcmllexer.cpp version.h srcmlapps.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}srcMLTranslator.o : srcMLTranslator.cpp srcMLTranslator.h StreamMLParser.h StreamParser.h srcMLToken.h Language.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

Mode.h : srcMLStateStack.h State.h StateStack.h srcMLState.h

${OBJ_DIR}srcMLParser.o : $(firstword $(SRCMLPARSER_GENS)) Mode.h TokenParser.h State.h Options.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(SRCMLPARSER_CXX_FLAGS) -c $< -o $@

# hack for antlr bug when it generate unintentional multi-line comment
${OBJ_DIR}KeywordCPPLexer.o : $(firstword $(KEYWORDCPPLEXER_GENS)) Language.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@

${OBJ_DIR}KeywordLexer.o : $(firstword $(KEYWORDLEXER_GENS))
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) -c $< -o $@
#	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(SRCMLPARSER_CXX_FLAGS) -c $< -o $@

${OBJ_DIR}srcMLOutput.o : srcMLOutput.cpp srcMLOutput.h srcMLOutputPR.h srcmlns.h
	$(CXX) ${XML_PRE} $(ANTLR_INC) $(CXXFLAGS) $(SRCMLPARSER_CXX_FLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}Mode.o : Mode.cpp Mode.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(SRCMLPARSER_CXX_FLAGS) -c $< -o $@

${OBJ_DIR}UTF8CharBuffer.o : UTF8CharBuffer.cpp UTF8CharBuffer.h
	$(CXX) $(ANTLR_INC) $(CXXFLAGS) $(SRCMLPARSER_CXX_FLAGS) -c $< -o $@

${OBJ_DIR}srcMLUtility.o : srcMLUtility.cpp srcMLUtility.h xmloutput.h Options.h
	$(CXX) $(XML_PRE) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

${OBJ_DIR}xmloutput.o : xmloutput.cpp xmloutput.h
	$(CXX) $(XML_PRE) $(ANTLR_INC) $(CXXFLAGS) $(LIBXML_INCLUDE) -c $< -o $@

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
	rm -f ${OBJ_DIR}src2srcml.o ${OBJ_DIR}srcMLOutput.o ${OBJ_DIR}Mode.o ${OBJ_DIR}srcMLTranslator.o ${OBJ_DIR}srcMLParser.o ${OBJ_DIR}KeywordLexer.o ${OBJ_DIR}KeywordCPPLexer.o ${OBJ_DIR}srcml2src.o ${OBJ_DIR}srcMLUtility.o ${OBJ_DIR}xmloutput.o ${OBJ_DIR}version.o ${OBJ_DIR}filetrim.o ${OBJ_DIR}srcmlapps.o ${OBJ_DIR}srcmllexer.o ${OBJ_DIR}xmlwriter.o ${OBJ_DIR}xmlreader.o ${OBJ_DIR}UTF8CharBuffer.o version.cpp

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

# on every build, record the version number
version.cpp : VERSION
	echo -n 'const char* version() { const char* Version = "' > $@
	cat VERSION	              >> $@
	echo -n " ${LIBXML_VERSION}"   >> $@
	echo '"; return Version; }'   >> $@

# changes use of 64 bit integer using transformation
visual :
	perl -pi -e "s|ULL;|i64;|g" Mode.cpp


# update trunk version
PHONY : trunkversion
trunkversion :
	echo -n "Trunk " > VERSION
	echo -n `date` >> VERSION

PHONY : install
install :
	cp ../bin/src2srcml /usr/bin/.
	cp ../bin/srcml2src /usr/bin/.
