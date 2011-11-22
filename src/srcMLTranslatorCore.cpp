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

#include "srcMLTranslatorCore.hpp"
#include "srcMLTranslator.hpp"
#include "KeywordCPPLexer.hpp"
#include "srcMLParser.hpp"
#include "StreamMLParser.hpp"
#include "srcMLOutput.hpp"
#include "srcmlapps.hpp"
#include "srcmlns.hpp"

// constructor
srcMLTranslatorCore::srcMLTranslatorCore(int language, const char* srcml_filename, OPTION_TYPE& op)
  : Language(language), first(true),
    root_directory(""), root_filename(""), root_version(""),
    encoding(DEFAULT_TEXT_ENCODING), options(op),
    out(0, srcml_filename, getLanguageString(), DEFAULT_XML_ENCODING, options, (const char**)DEFAULT_URI_PREFIX, 8), tabsize(8) {
}

// constructor
srcMLTranslatorCore::srcMLTranslatorCore(int language, xmlBuffer* output_buffer, OPTION_TYPE& op)
  : Language(language), first(true),
    root_directory(""), root_filename(""), root_version(""),
    encoding(DEFAULT_TEXT_ENCODING), options(op),
    out(0, 0, getLanguageString(), DEFAULT_XML_ENCODING, options, (const char**)DEFAULT_URI_PREFIX, 8, output_buffer), tabsize(8) {
}

// constructor
srcMLTranslatorCore::srcMLTranslatorCore(int language,                // programming language of source code
				 const char* src_encoding,    // text encoding of source code
				 const char* xml_encoding,    // xml encoding of result srcML file
				 const char* srcml_filename,  // filename of result srcML file
				 OPTION_TYPE& op,             // many and varied options
				 const char* directory,       // root unit directory
				 const char* filename,        // root unit filename
				 const char* version,         // root unit version
				 const char* uri[],           // uri prefixes
				 int tabsize                  // size of tabs
				 )
  : Language(language), pinput(0), first(true),
    root_directory(directory), root_filename(filename), root_version(version),
    encoding(src_encoding), options(op),
    out(0, srcml_filename, getLanguageString(), xml_encoding, options, uri, tabsize), tabsize(tabsize) {

// constructor
srcMLTranslatorCore::srcMLTranslatorCore(int language,                // programming language of source code
				 const char* src_encoding,    // text encoding of source code
				 const char* xml_encoding,    // xml encoding of result srcML file
                                 xmlBuffer * output_buffer,
				 OPTION_TYPE& op,             // many and varied options
				 const char* directory,       // root unit directory
				 const char* filename,        // root unit filename
				 const char* version,         // root unit version
				 const char* uri[],           // uri prefixes
				 int tabsize                  // size of tabs
				 )
  : Language(language), pinput(0), first(true),
    root_directory(directory), root_filename(filename), root_version(version),
    encoding(src_encoding), options(op),
  out(0, 0, getLanguageString(), xml_encoding, options, uri, tabsize, output_buffer), tabsize(tabsize) {

}

// translate from input stream to output stream
void* srcMLTranslatorCore::setInput(const char* path) {

  try {
    pinput = new UTF8CharBuffer(path, encoding);

    // return the libxml context of the file
    if (pinput)
      return pinput->getContext();

  } catch (const std::exception& e) {
    fprintf(stderr, "SRCML Exception: %s\n", e.what());
  }
  catch (UTF8FileError) {
    throw FileError();
  }
  catch (...) {
    fprintf(stderr, "ERROR\n");
  }

  return 0;
}

// close the output
void srcMLTranslatorCore::close() {

  out.close();
}

// translate from input stream to output stream
void srcMLTranslatorCore::translate(const char* path, const char* unit_directory,
				const char* unit_filename, const char* unit_version,
				int language) {

  // root unit for compound srcML documents
  if (first && ((options & OPTION_NESTED) > 0))
    out.startUnit(0, root_directory, root_filename, root_version, true);

  first = false;

  try {

      // master lexer with multiple streams
      antlr::TokenStreamSelector selector;

      // srcML lexical analyzer from standard input
      KeywordCPPLexer lexer(pinput, encoding, language);
      lexer.setSelector(&selector);
      lexer.setTabsize(tabsize);

      // pure block comment lexer
      PureCommentLexer textlexer(lexer.getInputState());
      textlexer.setSelector(&selector);

      // switching between lexers
      selector.addInputStream(&lexer, "main");
      selector.addInputStream(&textlexer, "text");
      selector.select(&lexer);

      // base stream parser srcML connected to lexical analyzer
      StreamMLParser<srcMLParser> parser(selector, language, options);

      // connect local parser to attribute for output
      out.setTokenStream(parser);

      // parse and form srcML output with unit attributes
      Language l(language);
      out.consume(l.getLanguageString(), unit_directory, unit_filename, unit_version);

  } catch (const std::exception& e) {
    fprintf(stderr, "SRCML Exception: %s\n", e.what());
  }
  catch (UTF8FileError) {
    throw FileError();
  }
  catch (...) {
    fprintf(stderr, "ERROR\n");
  }
}

// destructor
srcMLTranslatorCore::~srcMLTranslatorCore() {
}
