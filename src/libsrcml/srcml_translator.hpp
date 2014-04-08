/**
 * @file srcml_translator.hpp
 *
 * @copyright Copyright (C) 2003-2014 SDML (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
  Class for straight forward source code to srcML translation
*/

#ifndef SRCML_TRANSLATOR_HPP
#define SRCML_TRANSLATOR_HPP

#include <Language.hpp>
#include <srcMLOutput.hpp>
#include <srcml.h>

#include <string>

/**
 * FileError
 *
 * Type thrown when a file error occurs
 */
class FileError {};

/** Forward declaration of input buffer type */
class UTF8CharBuffer;

/**
* srcml_translator
*
* Class for translation of individual files
* and projects into srcML.  Inherits from Language
* to set what language to use for parsing.
*/
class srcml_translator : public Language {
public:

    // constructor
    srcml_translator(int language,
                    const char* src_encoding,
                    const char* xml_encoding,
                    char** str_buf,
                    int * size,
                    OPTION_TYPE& options,
                    const char* directory,
                    const char* filename,
                    const char* version,
                    std::vector<std::string> & prefix,
                    std::vector<std::string> & uri,
                    int tabsize
                    );

    // constructor
    srcml_translator(int language,
                    const char* src_encoding,
                    const char* xml_encoding,
                    xmlOutputBuffer * output_buffer,
                    OPTION_TYPE& options,
                    const char* directory,
                    const char* filename,
                    const char* version,
                    std::vector<std::string> & prefix,
                    std::vector<std::string> & uri,
                    int tabsize
                    );

    void set_macro_list(std::vector<std::string> & list);

    void close();

    // translate from input stream to output stream
    void translate(const char* unit_directory = 0,
                   const char* unit_filename = 0,
                   const char* unit_version = 0,
                   const char* unit_timestamp = 0,
                   const char* unit_hash = 0,
                   int language = 0);

    void translate_separate(const char* unit_directory,
                            const char* unit_filename,
                            const char* unit_version,
                            const char* unit_timestamp,
                            const char* unit_hash,
                            int language, UTF8CharBuffer * parser_input, xmlBuffer* output_buffer,
                            OPTION_TYPE translation_options);

    void add_unit(const srcml_unit * unit, const char * xml);
    void add_raw_len(const char * content, size_t length);

    // destructor
    ~srcml_translator();

private:

    /** input source */
    UTF8CharBuffer* pinput;

    /** is this the first unit */
    bool first;

    /** the root unit directory attribute */
    const char* root_directory;

    /** the root unit filename attribute */
    const char* root_filename;

    /** the root unit version attribute */
    const char* root_version;

    /** the output encoding */
    const char* xml_encoding;

    /** translation options */
    OPTION_TYPE& options;

    /** buffer to hold output to memory */
    xmlBuffer * buffer;

    /** member for handling output */
    srcMLOutput out;

    /** size of tabstop */
    int tabsize;

    /** list of namespace prefixes */
    std::vector<std::string> & prefix;

    /** list of namespace uris */
    std::vector<std::string> & uri;

    /** list of user defined macros */
    std::vector<std::string> user_macro_list;

    /** location to store output when output is to memory */
    char ** str_buffer;
    
    /** location to store size of output when output is to memory */
    int * size;

};

#endif
