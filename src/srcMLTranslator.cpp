/*
  srcMLTranslator.cpp

  Copyright (C) 2004, 2005  SDML (www.sdml.info)

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
*/

/*
  Class for straightforward translation
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
				 const char* ofilename,
				 int op
				 )
  : Language(language), options(op),
    out(0, getLanguageString(), xml_encoding, options, ofilename, src_encoding) {


   out.startconsumeAll(ofilename);

   if ((options & OPTION_NESTED) > 0)

     out.startUnit(getLanguageString(), "", "", "", true);
}

// translate from input stream to output stream
void srcMLTranslator::translate(const char* src_filename, const char* adir, const char* afilename, const char* version) {

  try {
      std::ifstream srcfile;
      std::istream* pin = &std::cin;
      if (strcmp("-", src_filename) != 0) {
	srcfile.open(src_filename);
	if (!srcfile)
	  throw FileError();
	pin = &srcfile;
      }

      // hack to fix overwriting of parameters
      std::string fdir(adir);
      std::string ffilename(afilename);

      // srcML lexical analyzer from standard input
      KeywordCPPLexer lexer(*pin, getLanguage());

      // base stream parser srcML connected to lexical analyzer
      StreamMLParser<srcMLParser> parser(lexer, getLanguage());

      out.setTokenStream(parser);
      out.consume(fdir.c_str(), ffilename.c_str(), version);

  } catch (FileError e) {
    throw FileError();

  } catch (const std::exception& e) {
      std::cout << "SRCML Exception: " << e.what() << std::endl;
  }
  catch (...) {
      std::cout << "ERROR" << std::endl;
  }
}

// destructor
srcMLTranslator::~srcMLTranslator() {

      out.endconsumeAll();
}
