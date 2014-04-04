/**
 * @file srcMLTranslator.cpp
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
  Class for straightforward translation from source code to srcML
*/

#include "srcMLTranslator.hpp"
#include "KeywordLexer.hpp"
#include "srcMLParser.hpp"
#include "StreamMLParser.hpp"
#include "srcMLOutput.hpp"
#include "srcmlns.hpp"

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
 * srcMLTranslator
 * @param language what language to parse in
 * @param src_encoding input source code encoding
 * @param xml_encoding output srcML encoding
 * @param srcml_filename name of resultant srcML file
 * @param op translator options
 * @param directory root unit directory attribute
 * @param filename unit directory attribute
 * @param version root unit directory attribute
 * @param prefix namespace prefix array
 * @param uri namespace uri array
 * @param tabsize size of tabstop
 * 
 * Constructor for output to a filename.
 */
srcMLTranslator::srcMLTranslator(int language,
                                 const char* src_encoding,
                                 const char* xml_encoding,
                                 const char* srcml_filename,
                                 OPTION_TYPE& op,
                                 const char* directory,
                                 const char* filename,
                                 const char* version,
                                 std::vector<std::string> & prefix,
                                 std::vector<std::string> & uri,
                                 int tabsize)
    : Language(language), pinput(0), first(true),
      root_directory(directory), root_filename(filename), root_version(version),
      encoding(src_encoding), xml_encoding(xml_encoding), options(op), buffer(0),
      out(0, srcml_filename, getLanguageString(), xml_encoding, options, prefix, uri, tabsize, 0), tabsize(tabsize), prefix(prefix), uri(uri),
      str_buffer(0), size(0) {}

/** 
 * srcMLTranslator
 * @param language what language to parse in
 * @param src_encoding input source code encoding
 * @param xml_encoding output srcML encoding
 * @param str_buf buffer to assign output srcML
 * @param size integer to assign size of resulting srcML
 * @param op translator options
 * @param directory root unit directory attribute
 * @param filename unit directory attribute
 * @param version root unit directory attribute
 * @param prefix namespace prefix array
 * @param uri namespace uri array
 * @param tabsize size of tabstop
 * 
 * Constructor for output to memory.
 */
srcMLTranslator::srcMLTranslator(int language,
                                 const char* src_encoding,
                                 const char* xml_encoding,
                                 char ** str_buf,
                                 int * size,
                                 OPTION_TYPE & op,
                                 const char* directory,
                                 const char* filename,
                                 const char* version,
                                 std::vector<std::string> & prefix,
                                 std::vector<std::string> & uri,
                                 int tabsize)
    :  Language(language), pinput(0), first(true), root_directory(directory), root_filename(filename), root_version(version),
       encoding(src_encoding), xml_encoding(xml_encoding), options(op), buffer(0),
       out(0, 0, getLanguageString(), xml_encoding, options, prefix, uri, tabsize, 0), tabsize(tabsize),
       prefix(prefix), uri(uri), str_buffer(str_buf), size(size) {

    buffer = xmlBufferCreate();
    xmlOutputBufferPtr obuffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(xml_encoding));
    out.setOutputBuffer(obuffer);

}

/** 
 * srcMLTranslator
 * @param language what language to parse in
 * @param src_encoding input source code encoding
 * @param xml_encoding output srcML encoding
 * @param output_buffer general libxml2 output buffer
 * @param op translator options
 * @param directory root unit directory attribute
 * @param filename unit directory attribute
 * @param version root unit directory attribute
 * @param prefix namespace prefix array
 * @param uri namespace uri array
 * @param tabsize size of tabstop
 * 
 * Constructor for output to libxml2 output buffer.
 */
srcMLTranslator::srcMLTranslator(int language,
                                 const char* src_encoding,
                                 const char* xml_encoding,
                                 xmlOutputBuffer * output_buffer,
                                 OPTION_TYPE& op,
                                 const char* directory,
                                 const char* filename,
                                 const char* version,
                                 std::vector<std::string> & prefix,
                                 std::vector<std::string> & uri,
                                 int tabsize)
    : Language(language), pinput(0), first(true),
      root_directory(directory), root_filename(filename), root_version(version),
      encoding(src_encoding), xml_encoding(xml_encoding), options(op), buffer(0),
      out(0, 0, getLanguageString(), xml_encoding, options, prefix, uri, tabsize, output_buffer), tabsize(tabsize), prefix(prefix), uri(uri),
      str_buffer(0), size(0) {}

/**
 * setMacroList
 * @param list user defined macro list
 *
 * Set the user defined macro list to use.
 */
void srcMLTranslator::setMacroList(std::vector<std::string> & list) {
    user_macro_list = list;
    out.setMacroList(list);
}

/**
 * setInput
 * @param path path of input file
 *
 * Set the input to the file at given path
 */
void srcMLTranslator::setInput(const char* path) {

    try {

        pinput = new UTF8CharBuffer(path, encoding, 0);

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

/**
 * close
 *
 * Close the srcMLTranslator and the srcML.
 */
void srcMLTranslator::close() {

    if(first && (options & OPTION_ARCHIVE) > 0) {

        // Open for write;
        out.initWriter();

        out.outputXMLDecl();

        // root unit for compound srcML documents
        out.startUnit(0, root_directory, root_filename, root_version, 0, 0, true);

    }

    out.close();
}

/**
 * translate
 * @param unit_directory unit directory attribute
 * @param unit_filename unit directory attribute
 * @param unit_version unit version attribute
 * @param unit_timestamp unit timestamp attribute
 * @param unit_hash unit hash attribute
 * @param language the language to translate the input
 *
 * Translate the supplied input (setInput) with the given arguments.
 */
void srcMLTranslator::translate(const char* unit_directory,
                                const char* unit_filename,
                                const char* unit_version,
                                const char* unit_timestamp,
                                const char* unit_hash,
                                int language) {

    if(first) {

        out.initWriter();

        out.outputXMLDecl();

        // root unit for compound srcML documents
        if((options & OPTION_ARCHIVE) > 0)
            out.startUnit(0, root_directory, root_filename, root_version, 0, 0, true);

    }

    first = false;

    try {

        // master lexer with multiple streams
        antlr::TokenStreamSelector selector;

        // srcML lexical analyzer from standard input
        KeywordLexer lexer(pinput, language, options, user_macro_list);
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
        StreamMLParser parser(selector, language, options);

        // connect local parser to attribute for output
        out.setTokenStream(parser);

        // parse and form srcML output with unit attributes
        Language l(language);
        out.consume(l.getLanguageString(), unit_directory, unit_filename, unit_version, unit_timestamp, unit_hash);

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

/**
 * translate_separate
 * @param unit_directory unit directory attribute
 * @param unit_filename unit directory attribute
 * @param unit_version unit version attribute
 * @param unit_timestamp unit timestamp attribute
 * @param unit_hash unit hash attribute
 * @param language the language to translate the input
 * @param parser_input the input source
 * @param output_buffer the output buffer
 * @param translation_options the translation options
 *
 * Translate the supplied input (parser_input) with the given arguments separately
 * from translator output and put in supplied output buffer.
 */
void srcMLTranslator::translate_separate(const char* unit_directory,
                                         const char* unit_filename,
                                         const char* unit_version,
                                         const char* unit_timestamp,
                                         const char* unit_hash,
                                         int language, UTF8CharBuffer * parser_input, xmlBuffer* output_buffer,
                                         OPTION_TYPE translation_options) {

    xmlOutputBufferPtr obuffer = xmlOutputBufferCreateBuffer(output_buffer, xmlFindCharEncodingHandler("UTF-8"));
    srcMLOutput sep_out(0, 0, getLanguageString(), xml_encoding, translation_options, prefix, uri, tabsize, obuffer);
    sep_out.initWriter();
    sep_out.setMacroList(user_macro_list);

    // save old output
    if(isoption(translation_options, OPTION_ARCHIVE))
        sep_out.setDepth(1);

    try {

        // master lexer with multiple streams
        antlr::TokenStreamSelector selector;

        // srcML lexical analyzer from standard input
        KeywordLexer lexer(parser_input, language, translation_options, user_macro_list);
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
        StreamMLParser parser(selector, language, translation_options);

        // connect local parser to attribute for output
        sep_out.setTokenStream(parser);

        // parse and form srcML output with unit attributes
        Language l(language);
        sep_out.consume(l.getLanguageString(), unit_directory, unit_filename, unit_version, unit_timestamp, unit_hash);

    } catch (const std::exception& e) {
        fprintf(stderr, "SRCML Exception: %s\n", e.what());
    }
    catch (UTF8FileError) {
        throw FileError();
    }
    catch (...) {
        fprintf(stderr, "ERROR\n");
    }

    sep_out.setDepth(0);

}

/**
 * add_unit
 * @param xml srcML to add to archive/non-archive
 * @param hash a possible hash to include with xml output as attribute
 *
 * Add a unit as string directly to the archive.  If not an archive
 * and supplied unit does not have src namespace add it.  Also, write out
 * a supplied hash as part of output unit if specified.
 */
void srcMLTranslator::add_unit(std::string xml, const char * hash) {

    if(first) {

        // Open for write;
        out.initWriter();

        out.outputXMLDecl();

        // root unit for compound srcML documents
        if((options & OPTION_ARCHIVE) > 0)
            out.startUnit(0, root_directory, root_filename, root_version, 0, 0, true);

        if ((options & OPTION_ARCHIVE) > 0)
            out.processText("\n\n", 2);

    }

    first = false;

    char * cxml = (char *)xml.c_str();

    if(!isoption(options, OPTION_ARCHIVE)) {

        char * pos = strchr(cxml, '>');
        if(pos == 0) return;

        char * src_ns_pos = strnstr(cxml, SRCML_SRC_NS_URI, pos - cxml);

        if(src_ns_pos == 0) {

            char * unit_pos = strstr(cxml, "unit");

            std::string ns = " xmlns";

            if((unit_pos - cxml) != 1) {

                ns += ":";
                char * colon_pos = strchr(cxml, ':');
                colon_pos[0] = 0;
                ns += (cxml + 1);
                colon_pos[0] = ':';

            }

            ns += "=\"";
            ns += SRCML_SRC_NS_URI;
            ns += "\"";

            // write out up to unit
            xmlTextWriterWriteRawLen(out.getWriter(), (xmlChar *)cxml, (int)((unit_pos + 4) - cxml));

            // write out namespace declaration
            xmlTextWriterWriteRaw(out.getWriter(), (xmlChar *)ns.c_str());

            // update pointer for remaining
            cxml = unit_pos + 4;

        }

    } 

    if(hash) {

        char * pos = strchr(cxml, '>');
        if(pos == 0) return;

        char * hash_pos = strnstr(cxml, "hash", pos - cxml);

        if(hash_pos != 0) {

            char * start_value = strchr(hash_pos, '"');

            xmlTextWriterWriteRawLen(out.getWriter(), (xmlChar *)cxml, (int)((start_value + 1) - cxml));
            xmlTextWriterWriteRaw(out.getWriter(), (xmlChar *)hash);
            cxml = start_value + 1;

            // consume hash if already there this is generic may consider using just 20 for standard hash size
            if(cxml[0] != '"')
                cxml = strchr(cxml, '"');

        }


    } 

    xmlTextWriterWriteRaw(out.getWriter(), (xmlChar *)cxml);


    if ((options & OPTION_ARCHIVE) > 0)
        out.processText("\n\n", 2);


}

/**
 * add_raw_len
 * @param content srcML text to add to archive
 * @param length number of bytes to write to archive
 *
 * Add the srcML text as string directly to the archive.
 */
void srcMLTranslator::add_raw_len(const char * content, size_t length) {

    if(first) {

        // Open for write;
        out.initWriter();

        out.outputXMLDecl();

        // root unit for compound srcML documents
        if((options & OPTION_ARCHIVE) > 0)
            out.startUnit(0, root_directory, root_filename, root_version, 0, 0, true);

    }

    first = false;

    if(length > 0)
      xmlTextWriterWriteRawLen(out.getWriter(), (xmlChar *)content, (int)length);

}

/**
 * ~srcMLTranslator
 *
 * Destructor.  If output to memory, free xml buffer and assign output to
 * locations.
 */
srcMLTranslator::~srcMLTranslator() {

    if(str_buffer && buffer->use) {

        (*str_buffer) = strdup((const char *)buffer->content);
        if(size && *str_buffer) *size = (int)buffer->use;

    }

    if(buffer)
        xmlBufferFree(buffer);

}
