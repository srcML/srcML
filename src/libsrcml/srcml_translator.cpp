/**
 * @file srcml_translator.cpp
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
  Class for straight forward translation from source code to srcML
*/

#include "srcml_translator.hpp"
#include "KeywordLexer.hpp"
#include "srcMLParser.hpp"
#include "StreamMLParser.hpp"
#include "srcMLOutput.hpp"
#include "srcmlns.hpp"
#include <srcml_types.hpp>

#include <cstring>

#ifdef _MSC_BUILD

/** correction for Visual Studio not liking POSIX style names */
#define strdup _strdup

#endif

#ifndef __APPLE__

/**
 * strnstr
 * @param s1 string to search
 * @param s2 string to search for in s1
 * @param n number of items to search
 * 
 * strnstr is only on BSD by default 
 * Bounded substring search. Search for s2 in s1 only search first n characters.
 *
 * @returns location of first match or NULL.
 */
char * strnstr(const char *s1, const char *s2, size_t n) {

  char save_char = s1[n];
  ((char *)s1)[n] = 0;
   char * ret = (char *)strstr(s1, s2);
  ((char *)s1)[n] = save_char;

  return ret;

}

#endif

/** 
 * srcml_translator
 * @param str_buf buffer to assign output srcML
 * @param size integer to assign size of resulting srcML
 * @param xml_encoding output srcML encoding
 * @param op translator options
 * @param prefix namespace prefix array
 * @param uri namespace uri array
 * @param tabsize size of tabstop
 * @param language what language to parse in
 * @param directory unit directory attribute
 * @param filename unit directory attribute
 * @param version unit directory attribute
 * @param timestamp unit timestamp attribute
 * @param hash unit hash attribute
 * 
 * Constructor for output to memory.
 */
srcml_translator::srcml_translator(char ** str_buf,
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
                                 const char* timestamp,
                                 const char* hash)
    :  Language(language), first(true), directory(directory), filename(filename), version(version), timestamp(timestamp), hash(hash),
       options(op), buffer(0),
       out(0, 0, getLanguageString(), xml_encoding, options, prefix, uri, tabsize), tabsize(tabsize),
       str_buffer(str_buf), size(size), is_outputting_unit(false), output_unit_depth(0) {

    buffer = xmlBufferCreate();
    xmlOutputBufferPtr obuffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(xml_encoding));
    out.setOutputBuffer(obuffer);

}

/** 
 * srcml_translator
 * @param output_buffer general libxml2 output buffer
 * @param xml_encoding output srcML encoding
 * @param op translator options
 * @param prefix namespace prefix array
 * @param uri namespace uri array
 * @param tabsize size of tabstop
 * @param language what language to parse in
 * @param directory unit directory attribute
 * @param filename unit directory attribute
 * @param version unit directory attribute
 * @param timestamp unit timestamp attribute
 * @param hash unit hash attribute
 * 
 * Constructor for output to libxml2 output buffer.
 */
srcml_translator::srcml_translator(xmlOutputBuffer * output_buffer,
                                 const char* xml_encoding,
                                 OPTION_TYPE& op,

                                 std::vector<std::string> & prefix,
                                 std::vector<std::string> & uri,
                                 int tabsize,
                                 int language,
                                 const char* directory,
                                 const char* filename,
                                 const char* version,
                                 const char* timestamp, 
                                 const char* hash)
    : Language(language), first(true),
      directory(directory), filename(filename), version(version), timestamp(timestamp), hash(hash),
      options(op), buffer(0),
      out(0, output_buffer, getLanguageString(), xml_encoding, options, prefix, uri, tabsize), tabsize(tabsize),
      str_buffer(0), size(0), is_outputting_unit(false), output_unit_depth(0) {}

/**
 * set_macro_list
 * @param list user defined macro list
 *
 * Set the user defined macro list to use.
 */
void srcml_translator::set_macro_list(std::vector<std::string> & list) {

  user_macro_list = list;
  out.setMacroList(list);

}

/**
 * close
 *
 * Close the srcml_translator and the srcML.
 */
void srcml_translator::close() {

    if(first && (options & OPTION_ARCHIVE) > 0) {

        // Open for write;
        out.initWriter();

        out.outputXMLDecl();

        // root unit for compound srcML documents
        out.startUnit(0, directory, filename, version, 0, 0, true);

    }

    out.close();
}

/**
 * translate
 *
 * Translate a single unit and output.  No xml declaration is added.
 */
void srcml_translator::translate(UTF8CharBuffer * parser_input) {

    if(first)
        // Open for write;
        out.initWriter();

    first = false;

    // output as inner unit
    if(isoption(options, SRCML_OPTION_ARCHIVE))
      out.setDepth(1);

    //options |= SRCML_OPTION_ARCHIVE;

    try {

        // master lexer with multiple streams
        antlr::TokenStreamSelector selector;

        // srcML lexical analyzer from standard input
        KeywordLexer lexer(parser_input, getLanguage(), options, user_macro_list);
        lexer.setSelector(&selector);
        lexer.setTabsize(tabsize);

        // pure block comment lexer
        CommentTextLexer textlexer(lexer.getInputState());
        textlexer.setSelector(&selector);

        // switching between lexers
        selector.addInputStream(&lexer, "main");
        selector.addInputStream(&textlexer, "text");
        selector.select(&lexer);

        // base stream parser srcML connected to lexical analyzer
        StreamMLParser parser(selector,  getLanguage(), options);

        // connect local parser to attribute for output
        out.setTokenStream(parser);

        // parse and form srcML output with unit attributes
        out.consume(getLanguageString(), directory, filename, version, timestamp, hash);

    } catch (const std::exception& e) {
        fprintf(stderr, "SRCML Exception: %s\n", e.what());
    }
    catch (UTF8FileError) {
        throw FileError();
    }
    catch (...) {
        fprintf(stderr, "ERROR\n");
    }

    // set back to root
    out.setDepth(0);

}

/**
 * add_unit
 * @param unit srcML to add to archive/non-archive with configuration options
 * @param hash a possible hash to include with xml output as attribute
 *
 * Add a unit as string directly to the archive.  If not an archive
 * and supplied unit does not have src namespace add it.  Also, write out
 * a supplied hash as part of output unit if specified.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_unit(const srcml_unit * unit, const char * xml) {

    if(first) {

        // Open for write;
        out.initWriter();

        out.outputXMLDecl();

        // root unit for compound srcML documents
        if((options & OPTION_ARCHIVE) > 0)
            out.startUnit(0, directory, filename, version, 0, 0, true);

        if ((options & OPTION_ARCHIVE) > 0)
            out.processText("\n\n", 2);

    }

    first = false;

    const char * end_start_unit = (char *)strchr(xml, '>');
    if(!end_start_unit) return false;

    /** extract language */
    char * language_start_name = strnstr(xml, "language", end_start_unit - xml);
    char * language_start_value = (char *)strchr(language_start_name, '"');
    char * language_end_value = (char *)strchr(language_start_value + 1, '"');
    (*language_end_value) = '\0';

    /** is there a cpp namespace */
    bool is_cpp = strnstr(xml, SRCML_CPP_NS_URI, end_start_unit - xml) != 0;

    OPTION_TYPE save_options = options;
    if(is_cpp) options |= SRCML_OPTION_CPP;

    out.startUnit(language_start_value + 1, unit->directory ? unit->directory->c_str() : 0, unit->filename ? unit->filename->c_str() : 0,
                          unit->version ? unit->version->c_str() : 0, unit->timestamp ? unit->timestamp->c_str() : 0, unit->hash ? unit->hash->c_str() : 0, false);

    options = save_options;
    (*language_end_value) = '"';

    size_t size = strlen(end_start_unit);

    while(end_start_unit[--size] != '/')
  ;

    if(end_start_unit[size - 1] == '<')
      --size;

    xmlTextWriterWriteRawLen(out.getWriter(), (xmlChar *)end_start_unit + 1, (int)size - 1);

    out.srcMLTextWriterEndElement(out.getWriter());

    if ((options & OPTION_ARCHIVE) > 0)
        out.processText("\n\n", 2);

    return true;

}

bool srcml_translator::add_start_unit(const srcml_unit * unit){

    if(first) {

        // Open for write;
        out.initWriter();

        out.outputXMLDecl();

        // root unit for compound srcML documents
        if((options & OPTION_ARCHIVE) > 0)
            out.startUnit(0, directory, filename, version, 0, 0, true);

        if ((options & OPTION_ARCHIVE) > 0)
            out.processText("\n\n", 2);

    }

    if(is_outputting_unit) return false;

    is_outputting_unit = true;

    out.startUnit(unit->language ? unit->language->c_str() : (unit->archive->language ? unit->archive->language->c_str() : 0), unit->directory ? unit->directory->c_str() : 0, unit->filename ? unit->filename->c_str() : 0,
                          unit->version ? unit->version->c_str() : 0, unit->timestamp ? unit->timestamp->c_str() : 0, unit->hash ? unit->hash->c_str() : 0, false);


    return true;

}

bool srcml_translator::add_end_unit() {

    if(!is_outputting_unit) return false;

    while(output_unit_depth--) xmlTextWriterEndElement(out.getWriter()) != -1;

    is_outputting_unit = false;

    return xmlTextWriterEndElement(out.getWriter()) != -1;

}

bool srcml_translator::add_start_element(const char * prefix, const char * name, const char * uri) {

    if(!is_outputting_unit) return false;

    ++output_unit_depth;

    return xmlTextWriterStartElementNS(out.getWriter(), (const xmlChar *)prefix, (const xmlChar *)name, (const xmlChar *)uri) != -1;

}

bool srcml_translator::add_end_element() {

    if(!is_outputting_unit) return false;

    --output_unit_depth;

    return xmlTextWriterEndElement(out.getWriter()) != -1;

}

bool srcml_translator::add_write_namespace(const char * prefix, const char * uri) {

    if(!is_outputting_unit && output_unit_depth) return false;

    std::string name = "xmlns";
    if(prefix) {

        name += ":";
        name += prefix;

    }

    return xmlTextWriterWriteAttributeNS(out.getWriter(), 0, (const xmlChar *)name.c_str(), 0, (const xmlChar *)uri) != -1;

}

bool srcml_translator::add_write_attribute(const char * prefix, const char * name, const char * uri, const char * content) {

    if(!is_outputting_unit && output_unit_depth) return false;

    return xmlTextWriterWriteAttributeNS(out.getWriter(), (const xmlChar *)prefix, (const xmlChar *)name, (const xmlChar *)uri, (const xmlChar *)content) != -1;

}

/**
 * ~srcml_translator
 *
 * Destructor.  If output to memory, free xml buffer and assign output to
 * locations.
 */
srcml_translator::~srcml_translator() {

    if(str_buffer && buffer->use) {

        (*str_buffer) = strdup((const char *)buffer->content);
        if(size && *str_buffer) *size = (int)buffer->use;

    }

    if(buffer)
        xmlBufferFree(buffer);

}
