/*
  srcMLTranslator.cpp

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

  Class for straightforward translation from source code to srcML
*/

#include "srcMLTranslator.hpp"
#include "srcMLTranslatorCore.hpp"

// constructor
srcMLTranslator::srcMLTranslator(int language, const char* srcml_filename, OPTION_TYPE& op) {

  translator = new srcMLTranslatorCore(language, srcml_filename, op);
}

// constructor
srcMLTranslator::srcMLTranslator(int language,                // programming language of source code
				 const char* src_encoding,    // text encoding of source code
				 const char* xml_encoding,    // xml encoding of result srcML file
				 const char* srcml_filename,  // filename of result srcML file
				 OPTION_TYPE& op,             // many and varied options
				 const char* directory,       // root unit directory
				 const char* filename,        // root unit filename
				 const char* version,         // root unit version
				 const char* uri[],           // uri prefixes
				 int tabsize                  // size of tabs
				 ) {

  translator = new srcMLTranslatorCore(language, src_encoding, xml_encoding, srcml_filename, op, directory, filename, version, uri, tabsize);
}

// translate from input stream to output stream
void* srcMLTranslator::setInput(const char* path) {

  return translator->setInput(path);
}

// close the output
void srcMLTranslator::close() {

  translator->close();
}

// translate from input stream to output stream
void srcMLTranslator::translate(const char* path, const char* unit_directory,
				const char* unit_filename, const char* unit_version,
				int language) {
  translator->translate(path, unit_directory, unit_filename, unit_version, language);
}

// destructor
srcMLTranslator::~srcMLTranslator() {

  delete translator;
}
