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
    srcml_translator(char ** str_buf,
                     int * size,
                     const char* xml_encoding,
                     OPTION_TYPE & op,
                     std::vector<std::string> & prefix,
                     std::vector<std::string> & uri,
                     int tabsize,
                     int language,
                     const char* directory,
                     const char* filename,
                     const char* version,
                     const char* timestamp = 0,
                     const char* hash = 0);

    // constructor
    srcml_translator(xmlOutputBuffer * output_buffer,
                     const char* xml_encoding,
                     OPTION_TYPE& op,
                     std::vector<std::string> & prefix,
                     std::vector<std::string> & uri,
                     int tabsize,
                     int language,
                     const char* directory,
                     const char* filename,
                     const char* version,
                     const char* timestamp = 0, 
                     const char* hash = 0);

    void set_macro_list(std::vector<std::string> & list);

    void close();

    void translate(UTF8CharBuffer* parser_input);

    void add_unit(const srcml_unit * unit, const char * xml);
    void add_start_unit(const srcml_unit * unit);
    void add_end_unit();
    void add_start_element(const char * prefix, const char * name, const char * uri);
    void add_end_element();
    void add_write_namespace(const char * prefix, const char * uri);
    void add_write_attribute(const char * prefix, const char * name, const char * uri, const char * content);

    // destructor
    ~srcml_translator();

private:

    /** is this the first unit */
    bool first;

    /** the unit directory attribute */
    const char* directory;

    /** the unit filename attribute */
    const char* filename;

    /** the unit version attribute */
    const char* version;

    /** the unit timestamp attribute */
    const char* timestamp;

    /** the unit hash attribute */
    const char* hash;

    /** translation options */
    OPTION_TYPE& options;

    /** buffer to hold output to memory */
    xmlBuffer * buffer;

    /** member for handling output */
    srcMLOutput out;

    /** size of tabstop */
    int tabsize;

    /** list of user defined macros */
    std::vector<std::string> user_macro_list;

    /** location to store output when output is to memory */
    char ** str_buffer;
    
    /** location to store size of output when output is to memory */
    int * size;

};

#endif
