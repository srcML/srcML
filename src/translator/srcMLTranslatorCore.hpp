/*
  srcMLTranslator.hpp

  Copyright (C) 2003-2014  SDML (www.srcML.org)

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

#ifndef SRCMLTRANSLATORCORE_HPP
#define SRCMLTRANSLATORCORE_HPP

#include "Language.hpp"
#include "srcMLOutput.hpp"

#include <string>

class UTF8CharBuffer;

class srcMLTranslatorCore : public Language {
public:

    // constructor
    srcMLTranslatorCore(int language, const char* srcml_filename, OPTION_TYPE& op);

    // constructor
    srcMLTranslatorCore(int language, xmlBuffer* output_buffer, OPTION_TYPE& op);

    // constructor
    srcMLTranslatorCore(int language,
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
    srcMLTranslatorCore(int language,
                        const char* src_encoding,
                        const char* xml_encoding,
                        xmlBuffer* output_buffer,
                        OPTION_TYPE& options,
                        const char* directory,
                        const char* filename,
                        const char* version,
                        const char* uri[],
                        int tabsize,
                        std::string * suri = 0
        );

    // constructor
    srcMLTranslatorCore(int language,
                        const char* src_encoding,
                        const char* xml_encoding,
                        xmlOutputBuffer * output_buf,
                        OPTION_TYPE& options,
                        const char* directory,
                        const char* filename,
                        const char* version,
                        const char* uri[],
                        int tabsize,
                        std::string * suri = 0
        );

    void setMacroList(std::vector<std::string> & list);

    void* setInput(const char* path);
    void* setInput(xmlParserInputBufferPtr input);

    void close();

    // translate from input stream to output stream
    void translate(const char* unit_directory = 0,
                   const char* unit_filename = 0,
                   const char* unit_version = 0,
                   int language = 0);

  void translate_separate(const char* unit_directory,
                          const char* unit_filename, const char* unit_version,
                          int language, xmlParserInputBufferPtr input, xmlBuffer* output_buffer,
			  OPTION_TYPE translation_options);

  void add_unit(const char* xml);

    // destructor
    ~srcMLTranslatorCore();

private:
    UTF8CharBuffer* pinput;
    bool first;
    const char* root_directory;
    const char* root_filename;
    const char* root_version;
    const char* encoding;
    const char* xml_encoding;
    OPTION_TYPE& options;
    srcMLOutput out;
    int tabsize;
    const char** uri;
    std::string * suri;
    std::vector<std::string> user_macro_list;
};

#endif
