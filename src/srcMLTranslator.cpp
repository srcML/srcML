/*
  srcMLTranslator.cpp

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
  Class for straightforward translation from source code to srcML
*/

#include "srcMLTranslator.hpp"
#include "srcMLTranslatorCore.hpp"

// constructor
srcMLTranslator::srcMLTranslator(int language, const char* srcml_filename, OPTION_TYPE& op) : output_buffer(0), str_buffer(0) {

  translator = new srcMLTranslatorCore(language, srcml_filename, op);
}

// constructor
srcMLTranslator::srcMLTranslator(int language, xmlBuffer* output_buffer, OPTION_TYPE& op) : output_buffer(0), str_buffer(0) {

  translator = new srcMLTranslatorCore(language, output_buffer, op);
}

// constructor
srcMLTranslator::srcMLTranslator(int language, OPTION_TYPE op) : str_buffer(0) {

  output_buffer = xmlBufferCreate();
  options = op;
  translator = new srcMLTranslatorCore(language, output_buffer, options);
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
                                 ) : output_buffer(0), str_buffer(0) {

  translator = new srcMLTranslatorCore(language, src_encoding, xml_encoding, srcml_filename, op, directory, filename, version, uri, tabsize);
}

// constructor
srcMLTranslator::srcMLTranslator(int language,                // programming language of source code
                                 const char* src_encoding,    // text encoding of source code
                                 const char* xml_encoding,    // xml encoding of result srcML file
                                 char ** str_buf,
                                 OPTION_TYPE & op,             // many and varied options
                                 const char* directory,       // root unit directory
                                 const char* filename,        // root unit filename
                                 const char* version,         // root unit version
                                 const char* uri[],           // uri prefixes
                                 int tabsize                  // size of tabs
                                 ) : str_buffer(str_buf) {

  output_buffer = xmlBufferCreate();
  translator = new srcMLTranslatorCore(language, src_encoding, xml_encoding, output_buffer, op, directory, filename, version, uri, tabsize);
}

// constructor
srcMLTranslator::srcMLTranslator(int language,                // programming language of source code
                                 const char* src_encoding,    // text encoding of source code
                                 const char* xml_encoding,    // xml encoding of result srcML file
                                 xmlBuffer* output_buffer,
                                 OPTION_TYPE& op,             // many and varied options
                                 const char* directory,       // root unit directory
                                 const char* filename,        // root unit filename
                                 const char* version,         // root unit version
                                 const char* uri[],           // uri prefixes
                                 int tabsize                  // size of tabs
                                 ) : output_buffer(0), str_buffer(0) {

  translator = new srcMLTranslatorCore(language, src_encoding, xml_encoding, output_buffer, op, directory, filename, version, uri, tabsize);
}

// constructor
srcMLTranslator::srcMLTranslator(int language,                // programming language of source code
                                 const char* src_encoding,    // text encoding of source code
                                 const char* xml_encoding,    // xml encoding of result srcML file
                                 OPTION_TYPE op,             // many and varied options
                                 const char* directory,       // root unit directory
                                 const char* filename,        // root unit filename
                                 const char* version,         // root unit version
                                 const char* uri[],           // uri prefixes
                                 int tabsize                  // size of tabs
                                 ) : str_buffer(0) {

  output_buffer = xmlBufferCreate();
  options = op;
  translator = new srcMLTranslatorCore(language, src_encoding, xml_encoding, output_buffer, options, directory, filename, version, uri, tabsize);
}

// constructor
srcMLTranslator::srcMLTranslator(int language,                // programming language of source code
                                 const char* src_encoding,    // text encoding of source code
                                 const char* xml_encoding,    // xml encoding of result srcML file
                                 xmlTextWriterPtr writer,
                                 OPTION_TYPE& op,             // many and varied options
                                 const char* directory,       // root unit directory
                                 const char* filename,        // root unit filename
                                 const char* version,         // root unit version
                                 const char* uri[],           // uri prefixes
                                 int tabsize                  // size of tabs
                                 ) : output_buffer(0), str_buffer(0) {

  translator = new srcMLTranslatorCore(language, src_encoding, xml_encoding, writer, op, directory, filename, version, uri, tabsize);
}

// translate from input stream to output stream
void* srcMLTranslator::setInput(const char* path) {

  return translator->setInput(path);
}

// translate from input stream to output stream
void* srcMLTranslator::setInput(xmlParserInputBufferPtr input) {

  return translator->setInput(input);
}

// translate from input stream to output stream
void* srcMLTranslator::setInputString(const char* source) {

  return translator->setInputString(source);
}

// translate from input stream to output stream
void* srcMLTranslator::setInputString(const char* source, int size) {

  return translator->setInputString(source);
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

// translate from input stream to output stream separate of current output stream
void srcMLTranslator::translate_separate(const char* path, const char* unit_directory,
				const char* unit_filename, const char* unit_version,
                                           int language, xmlBuffer* output_buffer) {
  
  translator->translate_separate(path, unit_directory, unit_filename, unit_version, language, output_buffer);
}

void srcMLTranslator::add_unit(const char* xml) {

  translator->add_unit(xml);
}

// destructor
srcMLTranslator::~srcMLTranslator() {

  delete translator;
  if(str_buffer)
    (*str_buffer) = strdup((const char *)output_buffer->content);

  if(output_buffer)
    xmlBufferFree(output_buffer);
}

extern "C" {

  // factory method
  srcMLTranslator * srcml_new(int language, OPTION_TYPE op) {
    
    return new srcMLTranslator(language, op);
  }
  srcMLTranslator * srcml_new_long(int language, const char * src_encoding, const char * xml_encoding, OPTION_TYPE op
                              , const char * directory, const char * filename, const char * version, const char * uri[], int tabsize) {


    return new srcMLTranslator(language, src_encoding, xml_encoding, op, directory, filename, version, uri, tabsize);
  }

  // translate from input stream to output stream
  void* srcml_set_input(srcMLTranslator * translator, const char* path) {

    return translator->getTranslator()->setInput(path);
  }

  // translate from input stream to output stream
  void* srcml_set_input_string(srcMLTranslator * translator, const char* source) {

    return translator->getTranslator()->setInputString(source);
  }

  // close the output
  void srcml_close(srcMLTranslator * translator) {

    translator->getTranslator()->close();
  }

  // translate from input stream to output stream
  void srcml_translate(srcMLTranslator * translator, const char* path, const char* unit_directory,
                                                        const char* unit_filename, const char* unit_version,
                                                        int language) {

    translator->translate(path, unit_directory, unit_filename, unit_version, language);
  }

  const char * srcml_get_srcml(srcMLTranslator * translator) {

    return (const char *)translator->getBuffer()->content;
  }

  // destructor
  void srcml_delete(srcMLTranslator * translator) {

    delete translator;
  }


}
