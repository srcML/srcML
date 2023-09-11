// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcml_translator.hpp
 *
 * @copyright Copyright (C) 2003-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Class for straight forward source code to srcML translation
*/

#ifndef SRCML_TRANSLATOR_HPP
#define SRCML_TRANSLATOR_HPP

#include <Language.hpp>
#include <srcMLOutput.hpp>
#include <srcml_types.hpp>
#include <srcml_options.hpp>

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
    srcml_translator(xmlOutputBuffer * output_buffer,
                     const char* xml_encoding,
                     OPTION_TYPE& op,
                     const Namespaces& namespaces,
                     std::optional<std::pair<std::string, std::string> > processing_instruction,
                     size_t tabsize,
                     int language,
                     const char* revision,
                     const char* url,
                     const char* filename,
                     const char* version,
                     const Attributes& attributes,
                     const char* timestamp,
                     const char* hash,
                     const char* encoding);

    void set_macro_list(std::vector<std::string> & list);

    void close();

    void translate(UTF8CharBuffer* parser_input);

    bool add_unit(const srcml_unit* unit);
    bool add_start_unit(const srcml_unit* unit);
    bool add_end_unit();
    bool add_start_element(const char* prefix, const char* name, const char* uri);
    bool add_end_element();
    bool add_namespace(const char* prefix, const char* uri);
    bool add_attribute(const char* prefix, const char* name, const char* uri, const char* content);
    bool add_string(const char* content);

    xmlOutputBufferPtr output_buffer() { return out.output_buffer; }

    xmlTextWriterPtr output_textwriter() { return out.xout; }

    // destructor
    ~srcml_translator();

private:

    /** is this the first unit */
    bool first = true;

    /** the unit revision attribute */
    const char* revision = nullptr;

    /** the unit url attribute */
    const char* url = nullptr;

    /** the unit filename attribute */
    const char* filename = nullptr;

    /** the unit version attribute */
    const char* version = nullptr;

    /** the unit timestamp attribute */
    const char* timestamp = nullptr;

    /** the unit hash attribute */
    const char* hash = nullptr;

    /** the unit encoding attribute */
    const char* encoding = nullptr;

    /** an array of name-value attribute pairs */
    const Attributes& attributes;

    Namespaces namespaces;

    /** translation options */
    OPTION_TYPE options;

public:
    /** member for handling output */
    srcMLOutput out;

private:

    void prepareOutput();

    /** size of tabstop */
    size_t tabsize;

    /** list of user defined macros */
    std::vector<std::string> user_macro_list;

    /** mark if have outputted starting unit tag for by element writing */
    bool is_outputting_unit = false;

public:
    /** track depth for by element writing */
    int output_unit_depth = 0;
};

#endif
