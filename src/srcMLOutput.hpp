/*
  srcMLOutput.hpp

  Copyright (C) 2003-2011  SDML (www.sdml.info)

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

#ifndef SRCMLOUTPUT_HPP
#define SRCMLOUTPUT_HPP

#include <iostream>
#include "antlr/Token.hpp"
#include "srcMLParserTokenTypes.hpp"
#include <antlr/TokenStream.hpp>
#include "StreamParser.hpp"
#include "TokenStream.hpp"
#include "Options.hpp"
#include "srcMLException.hpp"

#include <libxml/xmlwriter.h>

class srcMLOutput : public srcMLParserTokenTypes {

 public:
  // constructor
  srcMLOutput(TokenStream* ints,
	      const char* srcml_filename,
	      const char* language,
	      const char* encoding,
	      OPTION_TYPE& option,
	      const char* uri[],
	      int tabsize,
              xmlBuffer* output_buffer = 0
	      );

  static bool checkEncoding(const char* encoding);

  // start a unit element with the passed metadata
  void startUnit(const char* unit_language,
		 const char* unit_directory, const char* unit_filename, const char* unit_version, bool outer);

  // close the output
  void close();

  // standard processing of text
  void processText(const antlr::RefToken& token);
  void processText(const std::string&);
  void processText(const char* s, int size);

  const char * lineAttributeValue(const antlr::RefToken& token);
  const char * columnAttributeValue(const antlr::RefToken& token);

  // destructor
  ~srcMLOutput();

public:
  TokenStream* input;

  xmlTextWriter* xout;

  const char* srcml_filename;
  const char* unit_language;
  const char* unit_dir;
  const char* unit_filename;
  const char* unit_version;
  OPTION_TYPE& options;
  const char* xml_encoding;
  const char** num2prefix;
  int openelementcount;

  int curline;
  int curcolumn;
  int tabsize;

  int depth;

  // output line attribute content
  std::string lineAttribute;
  std::string columnAttribute;
  char out[21];

  int consume_next();

  void outputToken(const antlr::RefToken& token);

  void processUnit(const antlr::RefToken& token);

  static void outputNamespaces(xmlTextWriterPtr xout, const OPTION_TYPE& options, int depth, bool outer, const char** num2prefix);

  bool isoption(const OPTION_TYPE& options) const;
  static bool isoption(const OPTION_TYPE& flag, const OPTION_TYPE& options);

  void srcMLTextWriterStartElement(xmlTextWriter*, const xmlChar* s);
  void srcMLTextWriterEndElement(xmlTextWriter*);
};

#endif
