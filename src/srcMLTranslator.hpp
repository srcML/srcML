/*
  srcMLTranslator.hpp

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
  Class for straightforward source code to srcML translation
*/

#ifndef SRCMLTRANSLATOR_HPP
#define SRCMLTRANSLATOR_HPP

#include "Options.hpp"
#include <libxml/xmlwriter.h>

#include <string>

class srcMLTranslatorCore;
class FileError {};

class srcMLTranslator {
public:

    // constructor
    srcMLTranslator(int language, const char* srcml_filename, OPTION_TYPE& op);

    // constructor
    srcMLTranslator(int language, xmlBuffer* output_buffer, OPTION_TYPE& op);

    srcMLTranslator(int language, OPTION_TYPE op);

    // constructor
    srcMLTranslator(int language,
                    const char* src_encoding,
                    const char* xml_encoding,
                    const char* srcml_filename,
                    OPTION_TYPE& options,
                    const char* directory,
                    const char* filename,
                    const char* version,
                    const char* uri[],
                    int tabsize,
                    std::string * suri = 0
        );

    // constructor
    srcMLTranslator(int language,
                    const char* src_encoding,
                    const char* xml_encoding,
                    char** str_buf,
                    int * size,
                    OPTION_TYPE& options,
                    const char* directory,
                    const char* filename,
                    const char* version,
                    std::string * uri,
                    int tabsize
        );

    // constructor
    srcMLTranslator(int language,
                    const char* src_encoding,
                    const char* xml_encoding,
                    xmlBuffer* output_buffer,
                    OPTION_TYPE& options,
                    const char* directory,
                    const char* filename,
                    const char* version,
                    const char* uri[],
                    int tabsize
        );

    // constructor
    srcMLTranslator(int language,                // programming language of source code
                    const char* src_encoding,    // text encoding of source code
                    const char* xml_encoding,    // xml encoding of result srcML file
                    OPTION_TYPE op,             // many and varied options
                    const char* directory,       // root unit directory
                    const char* filename,        // root unit filename
                    const char* version,         // root unit version
                    const char* uri[],           // uri prefixes
                    int tabsize                  // size of tabs
        );

    // constructor
    srcMLTranslator(int language,
                    const char* src_encoding,
                    const char* xml_encoding,
                    xmlTextWriterPtr writer,
                    OPTION_TYPE& options,
                    const char* directory,
                    const char* filename,
                    const char* version,
                    const char* uri[],
                    int tabsize,
                    std::string * suri = 0
        );


    void* setInput(const char* path);
    void* setInput(xmlParserInputBufferPtr input);
    void* setInputString(const char* source);
    void* setInputString(const char* source, int size);

    void close();

    // translate from input stream to output stream
    void translate(const char* path,
                   const char* unit_directory = 0,
                   const char* unit_filename = 0,
                   const char* unit_version = 0,
                   int language = 0);
  void translate_separate(const char* path, const char* unit_directory,
                          const char* unit_filename, const char* unit_version,
                          int language, xmlBuffer* output_buffer);

  void add_unit(const char* xml);

    // destructor
    ~srcMLTranslator();

    srcMLTranslatorCore * getTranslator() {
        return translator;
    }

    xmlBuffer * getBuffer() {
        return output_buffer;
    }

private:
    xmlBuffer * output_buffer;
    srcMLTranslatorCore * translator;
    OPTION_TYPE options;
    char ** str_buffer;
    int * size;

};

#endif
