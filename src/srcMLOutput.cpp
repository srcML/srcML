/*
  srcMLOutput.cpp

  Copyright (C) 2002-2006  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  XML output
*/

#include "srcMLOutput.h"
#include "srcMLToken.h"
#include "project.h"
#include "srcmlns.h"

#include "srcMLOutputPR.h"

const char* XML_DECLARATION_STANDALONE = "yes";
const char* XML_VERSION = "1.0";

#define SRC_PREFIX ""
#define CPP_PREFIX "cpp:"

// encoding handler
xmlCharEncodingHandlerPtr handler = 0;

// check if encoding is supported
bool srcMLOutput::checkEncoding(const char* encoding) {

  return xmlFindCharEncodingHandler(encoding) != 0;
}

srcMLOutput::srcMLOutput(TokenStream* ints, 
			 const char* filename,
			 const char* language, 
			 const char* src_encoding,
			 const char* xml_enc,
			 int op)
  : input(ints), xout(0), srcml_filename(filename), unit_language(language), unit_dir(""), unit_filename(""),
    unit_version(""), options(op), xml_encoding(xml_enc)
{
  // setup an output handler
  handler = xmlFindCharEncodingHandler(src_encoding);
  if (!handler)
    throw srcEncodingException();

#ifdef LIBXML_ENABLED
  // if the source encoding is to UTF-8, skip any encoding
  if (strcmp(handler->name, "UTF-8") == 0)
    op |= OPTION_SKIP_ENCODING;
#endif

  // fill the elements
  fillElementNames();

  if (isoption(OPTION_LITERAL)) {
    // literal values
    ElementNames[SSTRING] = "literal";
    ElementNames[SCHAR] = "literal";
    ElementNames[SLITERAL] = "literal";
  }

  // only allow debug tags in debug
  if (!isoption(OPTION_DEBUG)) {
    ElementNames[SMARKER] = "";
    ElementNames[SERROR_PARSE]  = "";
    ElementNames[SERROR_MODE] = "";
  }

  // assign for special processing
  //  process_table[SUNIT] = &srcMLOutput::processUnit;
  process_table[BLOCKCOMMENT] = &srcMLOutput::processBlockComment;
  process_table[LINECOMMENT] = &srcMLOutput::processLineComment;
  process_table[SMARKER] = &srcMLOutput::processMarker;
  process_table[SPUBLIC_ACCESS_DEFAULT] = &srcMLOutput::processAccess;
  process_table[SPRIVATE_ACCESS_DEFAULT] = &srcMLOutput::processAccess;
  process_table[SSTRING] = &srcMLOutput::processString;
  process_table[SCHAR] = &srcMLOutput::processChar;
  process_table[SLITERAL] = &srcMLOutput::processLiteral;
  process_table[FORMFEED] = &srcMLOutput::processFormFeed;
  process_table[SINTERFACE] = &srcMLOutput::processInterface;

  // open the output text writer stream
  // "-" filename is standard output
  xout = xmlNewTextWriterFilename(srcml_filename, isoption(OPTION_COMPRESSED));

  // issue the xml declaration
  xmlTextWriterStartDocument(xout, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);
}

srcMLOutput::~srcMLOutput() {

  xmlTextWriterEndDocument(xout);

  xmlFreeTextWriter(xout);
}

void srcMLOutput::setTokenStream(TokenStream& ints) {

  input = &ints;
}

void srcMLOutput::consume(const char* directory, const char* filename, const char* version) {

  // store attributes so that first occurrence of unit element will be correct
  unit_dir = directory;
  unit_filename = filename;
  unit_version = version;

  // consume all input until EOF
  while (consume_next() != antlr::Token::EOF_TYPE) {

    // in interactive mode flush after each token is discovered
    if (isoption(OPTION_INTERACTIVE))
      xmlTextWriterFlush(xout);
  }
}

bool srcMLOutput::isoption(int flag) const {
  return (flag & options) > 0;
}

int srcMLOutput::consume_next() {


  const antlr::RefToken& token = input->nextToken();

  outputToken(token);

  return token->getType();
}

const char* srcMLOutput::token2name(const antlr::RefToken& token) const {

  return ElementNames[token->getType()];
}

const char* srcMLOutput::type2name(int token_type) const {

  return ElementNames[token_type];
}

#ifdef LIBXML_ENABLED
// buffer of output utf8 characters
const int UTF8BUFFER_MAXSIZE = 512;

xmlBufferPtr poutbuffer = xmlBufferCreateSize(UTF8BUFFER_MAXSIZE);

// amount of space for expanded characters.  assume a maximum of four bytes for every original single byte
const int UTF8BUFFER_SPACE = UTF8BUFFER_MAXSIZE / 4;
#endif

// output text
void srcMLOutput::processText(const std::string& str) {

  xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) str.c_str(), str.size());
}

// output encoded text
void srcMLOutput::processEncodedText(const std::string& str) {

  // no encoding needed for conversion from UTF-8
#ifdef LIBXML_ENABLED
  if (isoption(OPTION_SKIP_ENCODING)) {
#endif
    processText(str);
    return;
#ifdef LIBXML_ENABLED
  }

  // input buffer created from C++ string
  xmlBufferPtr pinbuffer = xmlBufferCreateStatic((char*) str.c_str(), str.size());
  // convert all of the input buffer to UTF-8 in chunks
  // conversion from libxml internal UTF-8 to output encoding is handled automatically
  unsigned int pos = 0;
  while (pos < str.size()) {

    // reset resusable output buffer
    poutbuffer->use = 0;

    int buffer_left = pinbuffer->size - pos;
    int partialinputbuffer_size = buffer_left < UTF8BUFFER_SPACE ? buffer_left : UTF8BUFFER_SPACE;

    pinbuffer->content += pos;
    pinbuffer->size -= pos;

    xmlCharEncInFunc(handler, poutbuffer, pinbuffer);
    
    xmlTextWriterWriteRawLen(xout, poutbuffer->content, poutbuffer->use);

    pos += partialinputbuffer_size;
  }
#endif
}

void srcMLOutput::processText(const antlr::RefToken& token) {
  processText(token->getText());
}

void srcMLOutput::processEncodedText(const antlr::RefToken& token) {
  processEncodedText(token->getText());
}

void srcMLOutput::processFormFeed(const antlr::RefToken& token) {

  // start the element
  xmlTextWriterStartElement(xout, BAD_CAST token2name(token));

  xmlTextWriterEndElement(xout);
}

void srcMLOutput::startUnit(const char* language, const char* dir, const char* filename, const char* version, bool outer) {

    // start of main tag
    xmlTextWriterStartElement(xout, BAD_CAST "unit");

    // outer units have namespaces
    if (outer) {

      // main srcML namespace declaration
      xmlTextWriterWriteAttribute(xout, BAD_CAST "xmlns", BAD_CAST SRCML_SRC_NS_URI);

      // main cpp namespace declaration
      xmlTextWriterWriteAttribute(xout, BAD_CAST "xmlns:cpp", BAD_CAST SRCML_CPP_NS_URI);

      // optional debugging xml namespace
      if (isoption(OPTION_DEBUG))
	xmlTextWriterWriteAttribute(xout, BAD_CAST "xmlns:srcerr", BAD_CAST SRCML_ERR_NS_URI);
    }

    // language attribute
    if (language[0])
      xmlTextWriterWriteAttribute(xout, BAD_CAST "language", BAD_CAST language);

    // directory attribute
    if (dir[0])
      xmlTextWriterWriteAttribute(xout, BAD_CAST "dir", BAD_CAST dir);

    // filename attribute
    if (filename[0])
      xmlTextWriterWriteAttribute(xout, BAD_CAST "filename", BAD_CAST filename);

    // version attribute
    if (version[0])
      xmlTextWriterWriteAttribute(xout, BAD_CAST "version", BAD_CAST version);

    // leave space for nested unit
    if (outer && isoption(OPTION_NESTED))
      processText("\n\n");
}

void srcMLOutput::processUnit(const antlr::RefToken& token) {

  if (isstart(token)) {

    startUnit(unit_language, unit_dir, unit_filename, unit_version, !isoption(OPTION_NESTED));

  } else {

    // end the unit
    xmlTextWriterEndElement(xout);

    // leave a blank line before next nested unit even the last one
    if (isoption(OPTION_NESTED))
      processText("\n\n");
  }
}

void srcMLOutput::processAccess(const antlr::RefToken& token) {
  static const char* type_default = "default";

  if (!isstart(token)) {
    processToken(token);
    return;
  }

  // start the element
  xmlTextWriterStartElement(xout, BAD_CAST token2name(token));

  xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST type_default);

  if (isempty(token))
    xmlTextWriterEndElement(xout);
}

void srcMLOutput::processToken(const antlr::RefToken& token) {

  const char* s = token2name(token);

  if (s[0] == 0)
    return;

  if (isstart(token) || isempty(token))
    xmlTextWriterStartElement(xout, BAD_CAST s);

  if (!isstart(token) || isempty(token))
    xmlTextWriterEndElement(xout);
}

void srcMLOutput::processLineComment(const antlr::RefToken& token) {

  const char* s = token2name(token);

  if (s[0] == 0)
    return;

  xmlTextWriterStartElement(xout, BAD_CAST s);

  xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST "line");

  processEncodedText(token);

  xmlTextWriterEndElement(xout);
}

void srcMLOutput::processBlockComment(const antlr::RefToken& token) {
  static const char* BLOCK_COMMENT_ATTR = "block";
  static const char* JAVADOC_COMMENT_ATTR = "javadoc";

  const char* s = token2name(token);

  if (s[0] == 0)
    return;

  xmlTextWriterStartElement(xout, BAD_CAST s);

  xmlTextWriterWriteAttribute(xout, BAD_CAST "type",
     BAD_CAST (strcmp(unit_language, "Java") == 0 && token->getText().substr(0, 3) == "/**" ? JAVADOC_COMMENT_ATTR : BLOCK_COMMENT_ATTR));

  processEncodedText(token);

  xmlTextWriterEndElement(xout);
}

void srcMLOutput::processOptional(const antlr::RefToken& token, const char* attr_name, const char* attr_value) {

  const char* s = token2name(token);

  if (s[0] == 0)
    return;

  if (isstart(token)) {
    xmlTextWriterStartElement(xout, BAD_CAST s);
    xmlTextWriterWriteAttribute(xout, BAD_CAST attr_name, BAD_CAST attr_value);
  } else
    xmlTextWriterEndElement(xout);
}

void srcMLOutput::processString(const antlr::RefToken& token) {

  processOptional(token, "type", "string");
}

void srcMLOutput::processChar(const antlr::RefToken& token) {

  processOptional(token, "type", "char");
}

void srcMLOutput::processLiteral(const antlr::RefToken& token) {

  processOptional(token, "type", "number");
}

void srcMLOutput::processMarker(const antlr::RefToken& token) {

  const char* s = token2name(token);

  if (s[0] == 0)
    return;

  xmlTextWriterStartElement(xout, BAD_CAST s);

  xmlTextWriterWriteAttribute(xout, BAD_CAST "location", BAD_CAST token->getText().c_str());

  xmlTextWriterEndElement(xout);
}

void srcMLOutput::processInterface(const antlr::RefToken& token) {
  static const char* INTERFACE_ATTR = "interface";

  const char* s = token2name(token);

  if (s[0] == 0)
    return;

  if (isstart(token) || isempty(token)) {
    xmlTextWriterStartElement(xout, BAD_CAST s);

    xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST INTERFACE_ATTR);
  }

  if (!isstart(token) || isempty(token))
    xmlTextWriterEndElement(xout);

}

inline void srcMLOutput::outputToken(const antlr::RefToken& token) {

  // use the array of pointers to methods to call the correct output routine
  ((*this).*(process_table[token->getType()]))(token);
}

// element names array
char* srcMLOutput::ElementNames[];

// fill the element names array
void srcMLOutput::fillElementNames() {

  ElementNames[SUNIT] = SRC_PREFIX "unit";
  ElementNames[LINECOMMENT] = SRC_PREFIX "comment";
  ElementNames[BLOCKCOMMENT] = ElementNames[LINECOMMENT];

  // No op
  ElementNames[SNOP] = "";

  // literal values
  ElementNames[SSTRING] = "";
  ElementNames[SCHAR] = "";
  ElementNames[SLITERAL] = "";

  // sub-statement elements
  ElementNames[SNAME] = SRC_PREFIX "name";
  ElementNames[SONAME] = "";
  ElementNames[SCNAME] = SRC_PREFIX "name";
  ElementNames[STYPE] = SRC_PREFIX "type";
  ElementNames[SCONDITION] = SRC_PREFIX "condition";
  ElementNames[SBLOCK] = SRC_PREFIX "block";
  ElementNames[SINDEX] = "";

  ElementNames[SEXPRESSION_STATEMENT] = SRC_PREFIX "expr_stmt";
  ElementNames[SEXPRESSION] = SRC_PREFIX "expr";

  ElementNames[SDECLARATION_STATEMENT] = SRC_PREFIX "decl_stmt";
  ElementNames[SDECLARATION] = SRC_PREFIX "decl";
  ElementNames[SDECLARATION_INITIALIZATION] = SRC_PREFIX "init";

  ElementNames[SBREAK_STATEMENT] = SRC_PREFIX "break";
  ElementNames[SCONTINUE_STATEMENT] = SRC_PREFIX "continue";
  ElementNames[SGOTO_STATEMENT] = SRC_PREFIX "goto";
  ElementNames[SLABEL_STATEMENT] = SRC_PREFIX "label";

  ElementNames[STYPEDEF] = SRC_PREFIX "typedef";
  ElementNames[SASM] = SRC_PREFIX "asm";
  ElementNames[SMACRO_CALL] = SRC_PREFIX "macro";
  ElementNames[SENUM] = SRC_PREFIX "enum";

  ElementNames[SIF_STATEMENT] = SRC_PREFIX "if";
  ElementNames[STHEN] = SRC_PREFIX "then";
  ElementNames[SELSE] = SRC_PREFIX "else";

  ElementNames[SWHILE_STATEMENT] = SRC_PREFIX "while";
  ElementNames[SDO_STATEMENT] = SRC_PREFIX "do";

  ElementNames[SSWITCH] = SRC_PREFIX "switch";
  ElementNames[SCASE] = SRC_PREFIX "case";
  ElementNames[SDEFAULT] = SRC_PREFIX "default";

  ElementNames[SFOR_STATEMENT] = SRC_PREFIX "for";
  ElementNames[SFOR_GROUP] = "";
  ElementNames[SFOR_INITIALIZATION] = SRC_PREFIX "init";
  ElementNames[SFOR_CONDITION] = ElementNames[SCONDITION];
  ElementNames[SFOR_INCREMENT] = SRC_PREFIX "incr";

  // functions
  ElementNames[SFUNCTION_DEFINITION]  = SRC_PREFIX "function";
  ElementNames[SFUNCTION_DECLARATION] = SRC_PREFIX "function_decl";
  ElementNames[SFUNCTION_SPECIFIER]   = SRC_PREFIX "specifier";
  ElementNames[SRETURN_STATEMENT]     = SRC_PREFIX "return";
  ElementNames[SFUNCTION_CALL]        = SRC_PREFIX "call";
  ElementNames[SPARAMETER_LIST]       = SRC_PREFIX "parameter_list";
  ElementNames[SPARAMETER]            = SRC_PREFIX "param";
  ElementNames[SARGUMENT_LIST]        = SRC_PREFIX "argument_list";
  ElementNames[SARGUMENT]             = SRC_PREFIX "argument";

  // struct, union
  ElementNames[SSTRUCT] = SRC_PREFIX "struct";
  ElementNames[SSTRUCT_DECLARATION]   = SRC_PREFIX "struct_decl";
  ElementNames[SUNION]  = SRC_PREFIX "union";
  ElementNames[SUNION_DECLARATION]    = SRC_PREFIX "union_decl";

  // class
  ElementNames[SCLASS]                   = SRC_PREFIX "class";
  ElementNames[SCLASS_DECLARATION]       = SRC_PREFIX "class_decl";
  ElementNames[SPUBLIC_ACCESS]           = SRC_PREFIX "public";
  ElementNames[SPUBLIC_ACCESS_DEFAULT]   = SRC_PREFIX "public";
  ElementNames[SPRIVATE_ACCESS]          = SRC_PREFIX "private";
  ElementNames[SPRIVATE_ACCESS_DEFAULT]  = SRC_PREFIX "private";
  ElementNames[SPROTECTED_ACCESS]        = SRC_PREFIX "protected";

  ElementNames[SMEMBER_INITIALIZATION_LIST]  = SRC_PREFIX "member_list";
  ElementNames[SCONSTRUCTOR_DEFINITION]  = SRC_PREFIX "constructor";
  ElementNames[SCONSTRUCTOR_DECLARATION] = SRC_PREFIX "constructor_decl";
  ElementNames[SDESTRUCTOR_DEFINITION]   = SRC_PREFIX "destructor";
  ElementNames[SDESTRUCTOR_DECLARATION]  = SRC_PREFIX "destructor_decl";
  ElementNames[SDERIVATION_LIST]         = SRC_PREFIX "super";
  ElementNames[SFRIEND]                  = SRC_PREFIX "friend";
  ElementNames[SCLASS_SPECIFIER]         = SRC_PREFIX "specifier";

  // extern definition
  ElementNames[SEXTERN] = SRC_PREFIX "extern";

  // namespaces
  ElementNames[SNAMESPACE] = SRC_PREFIX "namespace";
  ElementNames[SUSING_DIRECTIVE] = SRC_PREFIX "using";

  // exception handling
  ElementNames[STRY_BLOCK]       = SRC_PREFIX "try";
  ElementNames[SCATCH_BLOCK]     = SRC_PREFIX "catch";
  ElementNames[STHROW_STATEMENT] = SRC_PREFIX "throw";
  ElementNames[STHROW_SPECIFIER] = SRC_PREFIX "throw";
  ElementNames[STHROW_SPECIFIER_JAVA] = SRC_PREFIX "throws";

  // template
  ElementNames[STEMPLATE] = SRC_PREFIX "template";
  ElementNames[STEMPLATE_ARGUMENT] = ElementNames[SARGUMENT];
  ElementNames[STEMPLATE_ARGUMENT_LIST] = ElementNames[SARGUMENT_LIST];
  ElementNames[STEMPLATE_PARAMETER] = ElementNames[SPARAMETER];
  ElementNames[STEMPLATE_PARAMETER_LIST] = ElementNames[SPARAMETER_LIST];

  // cpp
  ElementNames[SCPP_DIRECTIVE] = CPP_PREFIX "directive";
  ElementNames[SCPP_FILENAME]  = CPP_PREFIX "file";
  ElementNames[SCPP_INCLUDE]   = CPP_PREFIX "include";
  ElementNames[SCPP_DEFINE]    = CPP_PREFIX "define";
  ElementNames[SCPP_UNDEF]     = CPP_PREFIX "undef";
  ElementNames[SCPP_LINE]      = CPP_PREFIX "line";
  ElementNames[SCPP_IF]        = CPP_PREFIX "if";
  ElementNames[SCPP_IFDEF]     = CPP_PREFIX "ifdef";
  ElementNames[SCPP_IFNDEF]    = CPP_PREFIX "ifndef";
  ElementNames[SCPP_ELSE]      = CPP_PREFIX "else";
  ElementNames[SCPP_ELIF]      = CPP_PREFIX "elif";
  ElementNames[SCPP_ENDIF]     = CPP_PREFIX "endif";
  ElementNames[SCPP_THEN]      = CPP_PREFIX "then";
  ElementNames[SCPP_PRAGMA]    = CPP_PREFIX "pragma";
  ElementNames[SCPP_ERROR]     = CPP_PREFIX "error";

  ElementNames[SMARKER]        = "srcerr:marker";
  ElementNames[SERROR_PARSE]   = "srcerr:parse";
  ElementNames[SERROR_MODE]    = "srcerr:mode";

  // Java elements
  ElementNames[SEXTENDS]       = SRC_PREFIX "extends";
  ElementNames[SIMPLEMENTS]    = SRC_PREFIX "implements";
  ElementNames[SIMPORT]        = SRC_PREFIX "import";
  ElementNames[SPACKAGE]       = SRC_PREFIX "package";
  ElementNames[SINTERFACE]     = SRC_PREFIX "class";

  // special characters
  ElementNames[FORMFEED]       = SRC_PREFIX "formfeed";
}
