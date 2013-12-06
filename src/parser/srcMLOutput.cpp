/*
  srcMLOutput.cpp

  Copyright (C) 2003-2013  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
  Output of the XML format based on srcMLParser
*/

#include "srcMLOutput.hpp"
#include "srcMLToken.hpp"
#include "project.hpp"
#include "srcmlns.hpp"
#include "srcmlapps.hpp"
#include <boost/preprocessor/iteration/local.hpp>

#include <cstring>
#include <sstream>

enum { SRCML_SRC_NS_URI_POS,
       SRCML_CPP_NS_URI_POS,
       SRCML_ERR_NS_URI_POS,
       SRCML_EXT_LITERAL_NS_URI_POS,
       SRCML_EXT_OPERATOR_NS_URI_POS,
       SRCML_EXT_MODIFIER_NS_URI_POS,
       SRCML_EXT_POSITION_NS_URI_POS,
};

// check if encoding is supported
bool srcMLOutput::checkEncoding(const char* encoding) {

  return xmlFindCharEncodingHandler(encoding) != 0;
}

srcMLOutput::srcMLOutput(TokenStream* ints,
			 const char* filename,
			 const char* language,
			 const char* xml_enc,
			 OPTION_TYPE& op,
			 const char* curi[],
			 int ts,
                         xmlBuffer* output_buffer,
                         xmlTextWriterPtr writer,
                         std::string * string_uri
			 )
  : input(ints), xout(0), srcml_filename(filename), unit_language(language), unit_dir(0), unit_filename(0),
    unit_version(0), options(op), xml_encoding(xml_enc), num2prefix(curi), openelementcount(0), curline(0),
    curcolumn(0), tabsize(ts), depth(0), num2sprefix(string_uri)
{
  // open the output text writer stream
  // "-" filename is standard output
  if (output_buffer == 0 && writer == 0) {
    xout = xmlNewTextWriterFilename(srcml_filename, isoption(OPTION_COMPRESSED));
    if (!xout) {
        fprintf(stderr, "src2srcml: " "Unable to open output file %s\n", srcml_filename);
        exit(2);
    }
  } else if (writer == 0) {
   xout = xmlNewTextWriterMemory(output_buffer, isoption(OPTION_COMPRESSED));
    if (!xout) {
        fprintf(stderr, "src2srcml: " "Unable to open output buffer\n");
        exit(2);
    }
  } else {
    xout = writer;
    if (!xout) {
        fprintf(stderr, "src2srcml: " "Unable to open output buffer\n");
        exit(2);
    }
  }

  // setup attributes names for line/column position if used
  if (isoption(OPTION_POSITION)) {

    lineAttribute = convert_num2prefix(SRCML_EXT_POSITION_NS_URI_POS);
    lineAttribute += ":line";

    line2Attribute = convert_num2prefix(SRCML_EXT_POSITION_NS_URI_POS);
    line2Attribute += ":line2";

    columnAttribute = convert_num2prefix(SRCML_EXT_POSITION_NS_URI_POS);
    columnAttribute += ":column";
  }

  // issue the xml declaration, but only if we want to
  if (!isoption(OPTION_XMLDECL)) xmlTextWriterStartDocument(xout, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);
}

srcMLOutput::~srcMLOutput() {

  close();
}

void srcMLOutput::close() {

  if (xout) {
    xmlTextWriterEndDocument(xout);
    xmlFreeTextWriter(xout);
    xout = 0;
  }
}

bool srcMLOutput::isoption(const OPTION_TYPE& flag) const {
  return (flag & options) > 0;
}

bool srcMLOutput::isoption(const OPTION_TYPE& flag, const OPTION_TYPE& options) {
  return (flag & options) > 0;
}

void srcMLOutput::srcMLTextWriterStartElement(xmlTextWriter* xout, const xmlChar* s) {

  xmlTextWriterStartElement(xout, s);
  ++openelementcount;
}

void srcMLOutput::srcMLTextWriterEndElement(xmlTextWriter* xout) {

  xmlTextWriterEndElement(xout);
  --openelementcount;
}

const char * srcMLOutput::lineAttributeValue(const antlr::RefToken& token) {

  snprintf(out, 20, "%d", token->getLine());

  return out;
}

const char * srcMLOutput::columnAttributeValue(const antlr::RefToken& token) {

  snprintf(out, 20, "%d", token->getColumn());

  return out;
}

const char * srcMLOutput::lineAttributeValue(int aline) {

  snprintf(out, 20, "%d", aline);

  return out;
}

void srcMLOutput::outputNamespaces(xmlTextWriterPtr xout, const OPTION_TYPE& options, int depth, bool outer) {

    // figure out which namespaces are needed
    char const * const ns[] = {

      // main srcML namespace declaration always used
      (depth == 0) ? SRCML_SRC_NS_URI : 0,

      // main cpp namespace declaration
      isoption(OPTION_CPP, options) && (isoption(OPTION_ARCHIVE, options) == !outer) ? SRCML_CPP_NS_URI : 0,

      // optional debugging xml namespace
      (depth == 0) && isoption(OPTION_DEBUG, options)    ? SRCML_ERR_NS_URI : 0,

      // optional literal xml namespace
      (depth == 0) && isoption(OPTION_LITERAL, options)  ? SRCML_EXT_LITERAL_NS_URI : 0,

      // optional operator xml namespace
      (depth == 0) && isoption(OPTION_OPERATOR, options) ? SRCML_EXT_OPERATOR_NS_URI : 0,

      // optional modifier xml namespace
      (depth == 0) && isoption(OPTION_MODIFIER, options) ? SRCML_EXT_MODIFIER_NS_URI : 0,

      // optional position xml namespace
      (depth == 0) && isoption(OPTION_POSITION, options) ? SRCML_EXT_POSITION_NS_URI : 0,

      // optional diff xml namespace
      (depth == 0) && isoption(OPTION_DIFF, options)     ? SRCML_DIFF_NS_URI : 0,
    };

    // output the namespaces
    for (unsigned int i = 0; i < sizeof(ns) / sizeof(ns[0]); ++i) {
      if (!ns[i])
	continue;

      std::string prefix = "xmlns";
      if (convert_num2prefix(i)[0] != '\0') {
	prefix += ':';
	prefix += convert_num2prefix(i);
      }

      xmlTextWriterWriteAttribute(xout, BAD_CAST prefix.c_str(), BAD_CAST ns[i]);
    }
}

void srcMLOutput::startUnit(const char* language, const char* dir, const char* filename, const char* version, bool outer) {

  const char * prefix = convert_num2prefix(0);
  std::string maintag = prefix ? prefix : "";
  if (!maintag.empty())
    maintag += ":";
  maintag += "unit";

  // start of main tag
  srcMLTextWriterStartElement(xout, BAD_CAST /* type2name(SUNIT) */ maintag.c_str());

  // outer units have namespaces
  if (/* outer && */ !isoption(OPTION_NAMESPACEDECL)) {
    outputNamespaces(xout, options, depth, outer);
  }

  // setting up for tabs, even if not used
  std::ostringstream stabs;
  std::string tabattribute;
  if (isoption(OPTION_POSITION)) {
    stabs << tabsize;
    tabattribute = convert_num2prefix(SRCML_EXT_POSITION_NS_URI_POS);
    tabattribute.append(":tabs");
  }

  // list of attributes
  const char* const attrs[][2] = {

    // version attribute
    { UNIT_ATTRIBUTE_REVISION, isoption(OPTION_REVISION) ? srcml_version() : 0 },

    // language attribute
    { UNIT_ATTRIBUTE_LANGUAGE, language },

    // directory attribute
    { UNIT_ATTRIBUTE_DIRECTORY, dir },

    // filename attribute
    { UNIT_ATTRIBUTE_FILENAME, filename },

    // version attribute
    { UNIT_ATTRIBUTE_VERSION, version },

    // position tab setting
    { tabattribute.c_str(), isoption(OPTION_POSITION) ? stabs.str().c_str() : 0 },
  };

  // output attributes
  for (unsigned int i = 0; i < sizeof(attrs) / sizeof(attrs[0]); ++i) {
    if (!attrs[i][1])
      continue;

    xmlTextWriterWriteAttribute(xout, BAD_CAST attrs[i][0], BAD_CAST attrs[i][1]);
  }

  // leave space for nested unit
  if (outer && isoption(OPTION_ARCHIVE))
    processText("\n\n", 2);

  ++depth;
}

void srcMLOutput::processUnit(const antlr::RefToken& token) {

  if (isstart(token)) {

    // keep track of number of open elements
    openelementcount = 0;
    startUnit(unit_language, unit_dir, unit_filename, unit_version, !isoption(OPTION_ARCHIVE));

  } else {

    // end anything still open, including the unit
    while (openelementcount > 0)
      srcMLTextWriterEndElement(xout);

    // leave a blank line before next nested unit even the last one
    if (isoption(OPTION_ARCHIVE))
      processText("\n\n", 2);
  }
}

// output text
void srcMLOutput::processText(const std::string& str) {

  xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) str.data(), str.size());
}

void srcMLOutput::processText(const char* s, int size) {

  xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) s, size);
}

void srcMLOutput::processText(const antlr::RefToken& token) {

  processText(token->getText());
}

void srcMLOutput::processTextPosition(const antlr::RefToken& token) {

  xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST lineAttributeValue(token));

  xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST columnAttributeValue(token));

  processText(token->getText());
}

void srcMLOutput::processTextPositionLine(const antlr::RefToken& token) {

  xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST lineAttributeValue(token->getLine() & 0xFFFF));
  xmlTextWriterWriteAttribute(xout, BAD_CAST line2Attribute.c_str(), BAD_CAST lineAttributeValue(token->getLine() >> 16));

  xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST columnAttributeValue(token));

  processText(token->getText());
}

xmlTextWriter * srcMLOutput::getWriter() {

  return xout;
}

void srcMLOutput::setWriter(xmlTextWriter * writer) {

  xout = writer;
}

int srcMLOutput::getDepth() {

  return depth;
}

void srcMLOutput::setDepth(int thedepth) {

  depth = thedepth;
}

const char * srcMLOutput::convert_num2prefix(unsigned int i) const {

  if(num2sprefix) return num2sprefix[i].c_str();
  else return num2prefix[i];

}
