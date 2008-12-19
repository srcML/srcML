/*
  srcMLTranslator.cpp

  Copyright (C) 2004-2006  SDML (www.sdml.info)

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

  Class for straightforward translation from source code to srcML
*/

#include <iostream>
#include <fstream>
#include "srcMLTranslator.h"
#include "KeywordCPPLexer.hpp"
#include "srcMLParser.hpp"
#include "StreamMLParser.h"
#include "srcMLOutput.h"
#include "srcmlns.h"
#include "PureCommentLexer.hpp"

#include "SegException.h"
#include "antlr/TokenStreamSelector.hpp"

// constructor
srcMLTranslator::srcMLTranslator(int language,                // programming language of source code
				 const char* src_encoding,    // text encoding of source code
				 const char* xml_encoding,    // xml encoding of result srcML file
				 const char* srcml_filename,  // filename of result srcML file
				 int op,
				 const char* directory,       // 
				 const char* filename,        // 
				 const char* version,         //
				 std::map<std::string, std::string>& uri
				 )
  : Language(language), encoding(src_encoding), options(op),
    out(0, srcml_filename, getLanguageString(), xml_encoding, options, uri) {

  // record the uri for the src namespace since we need it in the
  // lexer for formfeed elements in strings
  srcuri = uri[SRCML_SRC_NS_URI];

  // root unit for compound srcML documents
  if ((options & OPTION_NESTED) > 0)
    out.startUnit(getLanguageString(), directory, filename, version, true);
}

// translate from input stream to output stream
void srcMLTranslator::translate(const char* src_filename, const char* unit_directory,
				const char* unit_filename, const char* unit_version) {

  try {
      // assume standard input
      std::istream* pin = &std::cin;

      // file input
      std::ifstream srcfile;
      if (strcmp("-", src_filename) != 0) {
	srcfile.open(src_filename);
	if (!srcfile)
	  throw FileError();
	pin = &srcfile;
      }

      // master lexer with multiple streams
      antlr::TokenStreamSelector selector;

      // srcML lexical analyzer from standard input
      KeywordCPPLexer lexer(*pin, encoding, srcuri, getLanguage());
      lexer.setSelector(&selector);

      // pure comment lexer
      PureCommentLexer commentlexer(lexer.getInputState());
      commentlexer.setSelector(&selector);

      // switching between lexers
      selector.addInputStream(&lexer, "main");
      selector.addInputStream(&commentlexer, "comment");
      selector.select(&lexer);

      // base stream parser srcML connected to lexical analyzer
      StreamMLParser<srcMLParser> parser(selector, getLanguage(), options);

      // connect local parser to attribute for output
      out.setTokenStream(parser);

      // parse and form srcML output with unit attributes
      out.consume(unit_directory, unit_filename, unit_version);

  } catch (FileError) {
      throw FileError();

  } catch (const std::exception& e) {
      std::cerr << "SRCML Exception: " << e.what() << std::endl;
  }
  catch (...) {
      std::cerr << "ERROR" << std::endl;
  }
}

// destructor
srcMLTranslator::~srcMLTranslator() {}
