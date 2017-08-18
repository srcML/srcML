/**
 * @file srcml_translator.cpp
 *
 * @copyright Copyright (C) 2003-2014 srcML, LLC. (www.srcML.org)
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

/** 
 * srcml_translator
 * @param str_buf buffer to assign output srcML
 * @param size integer to assign size of resulting srcML
 * @param xml_encoding output srcML encoding
 * @param op translator options
 * @param prefix namespace prefix array
 * @param uri namespace uri array
 * @param processing_instruciton a pre-root processing instuction
 * @param tabsize size of tabstop
 * @param language what language to parse in
 * @param revision what version of srcML
 * @param url unit url attribute
 * @param filename unit url attribute
 * @param version unit url attribute
 * @param timestamp unit timestamp attribute
 * @param hash unit hash attribute
 * @param encoding unit source encoding
 * 
 * Constructor for output to memory.
 */
srcml_translator::srcml_translator(char** str_buf,
                                 size_t* size,
                                 const char* xml_encoding,
                                 OPTION_TYPE& op,
//                                 const std::vector<std::string>& prefix,
//                                 const std::vector<std::string>& uri,
                                 const std::vector<Namespace>& namespaces,
                                 boost::optional<std::pair<std::string, std::string> > processing_instruction,
                                 size_t tabsize,
                                 int language,
                                 const char* revision,
                                 const char* url,
                                 const char* filename,
                                 const char* version,
                                 const std::vector<std::string>& attributes,
                                 const char* timestamp,
                                 const char* hash,
                                 const char* encoding)
    :  Language(language),
       revision(revision), url(url), filename(filename), version(version), timestamp(timestamp), hash(hash), encoding(encoding), attributes(attributes), namespaces(namespaces),
       options(op),
       out(0, 0, getLanguageString(), xml_encoding, options, attributes, processing_instruction, tabsize), tabsize(tabsize),
       str_buffer(str_buf), size(size) {

    buffer = xmlBufferCreate();
    xmlOutputBufferPtr obuffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler(xml_encoding));

    if (xml_encoding) {

#ifdef LIBXML2_NEW_BUFFER
        xmlBufShrink(obuffer->conv, xmlBufUse(obuffer->conv));
#else
        obuffer->conv->use = 0;
#endif
    }

    out.setOutputBuffer(obuffer);
}

/** 
 * srcml_translator
 * @param output_buffer general libxml2 output buffer
 * @param xml_encoding output srcML encoding
 * @param op translator options
 * @param prefix namespace prefix array
 * @param uri namespace uri array
 * @param processing_instruciton a pre-root processing instuction
 * @param tabsize size of tabstop
 * @param language what language to parse in
 * @param revision what version of srcML
 * @param url unit url attribute
 * @param filename unit url attribute
 * @param version unit url attribute
 * @param timestamp unit timestamp attribute
 * @param hash unit hash attribute
 * @param encoding unit source encoding
 * 
 * Constructor for output to libxml2 output buffer.
 */
srcml_translator::srcml_translator(xmlOutputBuffer * output_buffer,
                                 const char* xml_encoding,
                                 OPTION_TYPE& op,
//                                 const std::vector<std::string>& prefix,
//                                 const std::vector<std::string>& uri,
                                 const std::vector<Namespace>& namespaces,
                                 boost::optional<std::pair<std::string, std::string> > processing_instruction,
                                 size_t tabsize,
                                 int language,
                                 const char* revision,
                                 const char* url,
                                 const char* filename,
                                 const char* version,
                                 const std::vector<std::string>& attributes,
                                 const char* timestamp, 
                                 const char* hash,
                                 const char* encoding)
    : Language(language),
      revision(revision), url(url), filename(filename), version(version), timestamp(timestamp), hash(hash), encoding(encoding), attributes(attributes), namespaces(namespaces),
      options(op),
      out(0, output_buffer, getLanguageString(), xml_encoding, options, attributes, processing_instruction, tabsize), tabsize(tabsize)
{
}

/**
 * set_macro_list
 * @param list user defined macro list
 *
 * Set the user defined macro list to use.
 */
void srcml_translator::set_macro_list(std::vector<std::string>& list) {

    user_macro_list = list;
    out.setMacroList(list);
}

/**
 * close
 *
 * Close the srcml_translator and the srcML.
 */
void srcml_translator::close() {

    // @todo Is this needed? Is this why we have to clone the archive, because this
    // is always created?
    if (first && !text_only && (options & SRCML_OPTION_ARCHIVE) > 0) {

        // Open for write;
        out.initWriter();
        out.initNamespaces(namespaces);

        out.outputXMLDecl();
        out.outputProcessingInstruction();

        // root unit for compound srcML documents
        out.startUnit(0, revision, url, filename, version, 0, 0, 0, attributes, true);
    }
    first = false;

    if (is_outputting_unit)
        add_end_unit();

    /* FIXME: Crashes when deleted */
    out.close();

    if (str_buffer && buffer->use) {

        (*str_buffer) = (char *)malloc(buffer->use * sizeof(char));
        memcpy(*str_buffer, buffer->content, (size_t)buffer->use);
        if (size && *str_buffer)
            *size = (size_t)buffer->use;
    }
}

/**
 * translate
 *
 * Translate a single unit and output.  No xml declaration is added.
 */
void srcml_translator::translate(UTF8CharBuffer* parser_input) {

    if (first) {
  
        // Open for write;
        out.initWriter();
        out.initNamespaces(namespaces);
    }
    first = false;

    // output as inner unit
    if (isoption(options, SRCML_OPTION_ARCHIVE))
        out.setDepth(1);

    try {

        // master lexer with multiple streams
        antlr::TokenStreamSelector selector;

        // srcML lexical analyzer from standard input
        KeywordLexer lexer(parser_input, getLanguage(), options, user_macro_list);
        lexer.setSelector(&selector);
        lexer.setTabsize((int)tabsize);

        // pure block comment lexer
        CommentTextLexer textlexer(lexer.getInputState());
        textlexer.setSelector(&selector);

        // switching between lexers
        selector.addInputStream(&lexer, "main");
        selector.addInputStream(&textlexer, "text");
        selector.select(&lexer);

        // base stream parser srcML connected to lexical analyzer
        StreamMLParser parser(selector, getLanguage(), options);

        // connect local parser to attribute for output
        out.setTokenStream(parser);

        // parse and form srcML output with unit attributes
        out.consume(getLanguageString(), revision, url, filename, version, timestamp, hash, encoding);

    } catch (const std::exception& e) {
        fprintf(stderr, "SRCML Exception: %s\n", e.what());
    }
    catch (UTF8FileError) {
        fprintf(stderr, "UTF8 file error\n");
    }
    catch (...) {
        fprintf(stderr, "srcML translator error\n");
    }

    // set back to root
    out.setDepth(0);
}

void srcml_translator::prepareOutput() {

    bool is_archive = (options & SRCML_OPTION_ARCHIVE) > 0;

    // Open for write;
    out.initWriter();
    out.initNamespaces(namespaces);

    if ((options & SRCML_OPTION_XML_DECL) > 0)
      out.outputXMLDecl();
  
    out.outputProcessingInstruction();

    // root unit for compound srcML documents

    if (is_archive) {
        out.startUnit(0, revision, url, filename, version, 0, 0, 0, attributes, true);
        out.outputUnitSeparator();
    }
}

/**
 * add_unit
 * @param unit srcML to add to archive/non-archive with configuration options
 * @param xml the xml to output
 *
 * Add a unit as string directly to the archive.  If not an archive
 * and supplied unit does not have src namespace add it.  Also, write out
 * a supplied hash as part of output unit if specified.
 * Can not be in by element mode.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_unit(const srcml_unit* unit, const char* xml) {

    if (is_outputting_unit)
        return false;

    if (first)
        prepareOutput();
    first = false;

    // find out where the end of the unit start tag is so
    // we can replace it on output with updated namespaces, etc.
    // @todo record this in the unit
    const char* end_start_unit = (char *)strchr(xml, '>');
    if (!end_start_unit)
        return false;

    OPTION_TYPE save_options = options;

    // is there a cpp namespace? 
    // @todo Get this from set of used namespaces from the unit
    bool is_cpp = false;
    for (auto& prefix : SRCML_URI_PREFIX) {
        std::string cpp_uri = prefix + "srcML/cpp";
        is_cpp = std::search(xml, end_start_unit, cpp_uri.begin(), cpp_uri.end()) != end_start_unit;
        if (is_cpp)
            break;
    }
    if (is_cpp)
        options |= SRCML_OPTION_CPP;

    std::string language = Language(unit->derived_language).getLanguageString();
    if (language == "") {

        /** extract language */ 
        const char* lang = "language";    
        const char* language_start_name = std::search(xml, end_start_unit, lang, lang + strlen(lang));    

        char* language_start_value = 0;   
        char* language_end_value = 0;   
        if (language_start_name) {    
        
            language_start_value = (char *)strchr(language_start_name, '"');    
            language_end_value = (char *)strchr(language_start_value + 1, '"');   
            language = std::string(language_start_value + 1, language_end_value - language_start_value - 1);
        } 
    }

    bool is_archive = (options & SRCML_OPTION_ARCHIVE) > 0;

    out.startUnit(language.c_str(),
            is_archive && unit->revision ? unit->revision->c_str() : revision,
            optional_to_c_str(unit->url),
            optional_to_c_str(unit->filename),
            optional_to_c_str(unit->version),
            optional_to_c_str(unit->timestamp),
            optional_to_c_str(unit->hash),
            optional_to_c_str(unit->encoding),
            unit->attributes,
            false);

    options = save_options;

    size_t size = strlen(end_start_unit);
    if (size > 1) {

        // find the start of the end tag
        // also corrects any data beyond unit end tag
        while (end_start_unit[--size] != '<')
            ;

        xmlTextWriterWriteRawLen(out.getWriter(), BAD_CAST end_start_unit + 1, (int)size - 1);
    }

    xmlTextWriterEndElement(out.getWriter());

    if ((options & SRCML_OPTION_ARCHIVE) > 0)
        out.outputUnitSeparator();

    return true;
}

/**
 * add_unit
 * @param unit srcML to add to archive/non-archive with configuration options
 * @param xml the xml to output
 *
 * Add a unit as string directly to the archive.  If not an archive
 * and supplied unit does not have src namespace add it.  Also, write out
 * a supplied hash as part of output unit if specified.
 * Can not be in by element mode.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_unit_raw(const char* xml, int size) {

    if (is_outputting_unit)
        return false;

    if (first)
        prepareOutput();
    first = false;

    if (size)
        xmlTextWriterWriteRawLen(out.getWriter(), BAD_CAST xml, size);

    if ((options & SRCML_OPTION_ARCHIVE) > 0)
          out.outputUnitSeparator();

    return true;
}

/**
 * add_unit
 * @param unit srcML to add to archive/non-archive with configuration options
 * @param xml the xml to output
 *
 * Add a unit as string directly to the archive.  If not an archive
 * and supplied unit does not have src namespace add it.  Also, write out
 * a supplied hash as part of output unit if specified.
 * Can not be in by element mode.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_unit_raw_node(xmlNodePtr node, xmlDocPtr doc) {

    if (is_outputting_unit)
        return false;

    if (first)
        prepareOutput();
    first = false;

    xmlNodeDumpOutput(out.output_buffer, doc, node, 0, 0, 0);

    if ((options & SRCML_OPTION_ARCHIVE) > 0)
        out.outputUnitSeparator();

    return true;
}

/**
 * add_start_unit
 * @param unit srcML to add to archive/non-archive with configuration options
 *
 * Add the start tag of a unit and set up for the remainder of unit output.
 *
 * Can not use add_unit while outputtting by element.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_start_unit(const srcml_unit * unit){

    if (first) {
        out.initWriter();
        out.initNamespaces(namespaces);
    }
    first = false;

    if (is_outputting_unit)
        return false;
    is_outputting_unit = true;

    int lang = unit->language ? srcml_check_language(unit->language->c_str())
        : (unit->archive->language ? srcml_check_language(unit->archive->language->c_str()) : SRCML_LANGUAGE_NONE);
    if (lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX || lang == Language::LANGUAGE_CSHARP ||
      lang == Language::LANGUAGE_OBJECTIVE_C)
        options |= SRCML_OPTION_CPP;

    if (isoption(options, SRCML_OPTION_ARCHIVE))
        out.setDepth(1);

    // @todo Why are we saving the options then restoring them?
    OPTION_TYPE save_options = options;

    out.startUnit(optional_to_c_str(unit->language, optional_to_c_str(unit->archive->language)),
                  revision,
                  optional_to_c_str(unit->url),
                  optional_to_c_str(unit->filename),
                  optional_to_c_str(unit->version),
                  optional_to_c_str(unit->timestamp),
                  optional_to_c_str(unit->hash),
                  optional_to_c_str(unit->encoding),
                  unit->attributes,
                  false);

    options = save_options;

    out.setDepth(0);

    return true;
}

/**
 * add_end_unit
 *
 * Add the end tag of a unit disable worther element processing.
 * Outputs all ending tags that are currently open within the unit first.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_end_unit() {

    if (!is_outputting_unit)
        return false;

    while (output_unit_depth--)
        xmlTextWriterEndElement(out.getWriter());

    is_outputting_unit = false;

    return xmlTextWriterEndElement(out.getWriter()) != -1;
}

/**
 * add_start_element
 * @param prefix the namespace prefix for element
 * @param name the name of the tag/element
 * @param uri the namespace uri for element
 *
 * Add the start element to a started unit.
 * add_start_unit most be called first.
 *
 * Can not output a unit tag.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_start_element(const char* prefix, const char* name, const char* uri) {

    if (!is_outputting_unit || name == 0)
        return false;

    if (strcmp(name, "unit") == 0)
        return false;

    ++output_unit_depth;

    return xmlTextWriterStartElementNS(out.getWriter(), BAD_CAST prefix, BAD_CAST name, BAD_CAST uri) != -1;
}

/**
 * add_end_element
 *
 * Add the end element to a started unit.
 * add_start_unit most be called first.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_end_element() {

    if (!is_outputting_unit)
        return false;

    --output_unit_depth;

    return xmlTextWriterEndElement(out.getWriter()) != -1;
}

/**
 * add_namespace
 * @param prefix the prefix for the namespace
 * @param uri the namespace uri
 *
 * Add the namespace to a started element.
 * add_start_unit most be called first.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_namespace(const char* prefix, const char *uri) {

    if (!is_outputting_unit || uri == 0)
        return false;

    std::string name = "xmlns";
    if (prefix) {
        name += ":";
        name += prefix;
    }

    return xmlTextWriterWriteAttribute(out.getWriter(), BAD_CAST name.c_str(), BAD_CAST uri) != -1;
}

/**
 * add_start_element
 * @param prefix the namespace prefix for attribute
 * @param name the name of the attribute
 * @param uri the namespace uri for attriubute
 * @param content the contents/value of the attribute
 *
 * Add the attribute to a start element.
 * add_start_unit most be called first.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_attribute(const char* prefix, const char* name, const char* uri, const char* content) {

    if (!is_outputting_unit || name == 0)
        return false;

    return xmlTextWriterWriteAttributeNS(out.getWriter(), BAD_CAST prefix, BAD_CAST name, BAD_CAST uri, BAD_CAST content) != -1;
}

/**
 * add_string.
 * @param content the string to write out
 *
 * Write the string/text to a started unit.
 * i.e being added to start tag.
 * add_start_unit must be called first.
 *
 * @returns if succesfully added.
 */
bool srcml_translator::add_string(const char *content) {

    if (!is_outputting_unit || content == 0)
        return false;

    char* text = (char *)content;
    for(char * pos = text; *pos; ++pos) {

        if (*pos != '"')
            continue;

        *pos = 0;
        if (xmlTextWriterWriteString(out.getWriter(), BAD_CAST text) == -1)
            return false;

        *pos = '\"';
        if ( xmlTextWriterWriteRaw(out.getWriter(), BAD_CAST "\"") == -1)
            return false;

        text = pos + 1;
    }

    int ret = xmlTextWriterWriteString(out.getWriter(), BAD_CAST text);

    return ret != -1;
}

/**
 * ~srcml_translator
 *
 * Destructor.
 */
srcml_translator::~srcml_translator() {

    if (buffer)
        xmlBufferFree(buffer);
}
