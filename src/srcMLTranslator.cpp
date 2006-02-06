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

#include "SegException.h"

// constructor
srcMLTranslator::srcMLTranslator(int language,
				 const char* src_encoding,
				 const char* xml_encoding,
				 const char* filename,
				 int op
				 )
  : Language(language), options(op),
    out(0, getLanguageString(), src_encoding, xml_encoding, options) {

   out.startconsumeAll(filename);

   if ((options & OPTION_NESTED) > 0)

     out.startUnit(getLanguageString(), "", "", "", true);
}

// translate from input stream to output stream
void srcMLTranslator::translate(const char* src_filename,
				const char* directory, const char* filename, const char* version) {

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

      // srcML lexical analyzer from standard input
      KeywordCPPLexer lexer(*pin, getLanguage());

      // base stream parser srcML connected to lexical analyzer
      StreamMLParser<srcMLParser> parser(lexer, getLanguage());

      // connect local parser to attribute for output
      out.setTokenStream(parser);

      // parse and form srcML output with unit attributes
      out.consume(directory, filename, version);

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
srcMLTranslator::~srcMLTranslator() {

  out.endconsumeAll();
}
