/*
  srcMLOutput.h

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

#ifndef SRCMLOUTPUT_H
#define SRCMLOUTPUT_H

#include <iostream>
#include <map>
#include "antlr/Token.hpp"
#include "srcMLParserTokenTypes.hpp"
#include <antlr/MismatchedTokenException.hpp>
#include <antlr/NoViableAltException.hpp>
#include <antlr/TokenStream.hpp>
#include "StreamParser.h"
#include "TokenStream.h"
#include "Options.h"
#include "srcMLException.h"

#include <xmlwriter.h>

class srcMLOutput : public srcMLParserTokenTypes {

 public:
  // constructor
  srcMLOutput(TokenStream* ints, 
	      const char* srcml_filename,
	      const char* language, 
	      const char* encoding,
	      int option,
	      std::map<std::string, std::string>& uri
	      );

  static bool checkEncoding(const char* encoding);

  // same srcml file can be generated from multiple input token streams
  void setTokenStream(TokenStream& ints);

  // start a unit element with the passed metadata
  void startUnit(const char* unit_language,
		 const char* unit_directory, const char* unit_filename, const char* unit_version, bool outer);

  // consume the entire tokenstream with output of srcml
  void consume(const char* unit_directory, const char* unit_filename, const char* unit_version = "");

  // destructor
  ~srcMLOutput();

 private:
  TokenStream* input;

  xmlTextWriter* xout;

  const char* srcml_filename;
  const char* unit_language;
  const char* unit_dir;
  const char* unit_filename;
  const char* unit_version;
  int options;
  const char* xml_encoding;

  static char* prefix_src;
  static char* prefix_cpp;
  static char* prefix_err;

  std::map<std::string, std::string>& uri;

  int consume_next();

  const char* type2name(int token_type) const;

  const char* token2name(const antlr::RefToken& token) const;

  void outputToken(const antlr::RefToken& token);

  bool isoption(int) const;

  // List of element names
  static const char* ElementNames[END_ELEMENT_TOKEN];
  static const char* ElementPrefix[END_ELEMENT_TOKEN];

  // fill the element names array
  static void fillElementNames();

  void processText(const antlr::RefToken& token);

  // standard processing of text
  void processText(const std::string&);

  // handler for optional literal tokens
  void processOptional(const antlr::RefToken& token, const char* attr_name, const char* attr_value);

  // token handlers
  void processUnit(const antlr::RefToken& token);
  void processAccess(const antlr::RefToken& token);
  void processToken(const antlr::RefToken& token);
  void processBlockCommentStart(const antlr::RefToken& token);
  void processLineCommentStart(const antlr::RefToken& token);
  void processEndToken(const antlr::RefToken& token);
  void processMarker(const antlr::RefToken& token);
  void processString(const antlr::RefToken& token);
  void processChar(const antlr::RefToken& token);
  void processLiteral(const antlr::RefToken& token);
  void processBoolean(const antlr::RefToken& token);
  void processInterface(const antlr::RefToken& token);
  void processEscape(const antlr::RefToken& token);

  // method pointer for token processing dispatch
  typedef void (srcMLOutput::*PROCESS_PTR)(const antlr::RefToken & );

  // table of method pointers for token processing dispatch
  static PROCESS_PTR process_table[END_ELEMENT_TOKEN];
};

#endif
