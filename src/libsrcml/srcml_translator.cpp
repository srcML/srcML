/**
 * @file srcml_translator.cpp
 *
 * @copyright Copyright (C) 2003-2019 srcML, LLC. (www.srcML.org)
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
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
#include <unit_utilities.hpp>

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
                                 const Namespaces& namespaces,
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
    out.initNamespaces(namespaces);
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

    if (!first && (options & SRCML_OPTION_ARCHIVE) > 0)
        out.outputUnitSeparator();

    prepareOutput();

    if (is_outputting_unit)
        add_end_unit();

    out.close();
}

/**
 * translate
 *
 * Translate a single unit and output.  No xml declaration is added.
 */
void srcml_translator::translate(UTF8CharBuffer* parser_input) {

    first = false;

    const int lang = getLanguage();
    if (lang == Language::LANGUAGE_C || lang == Language::LANGUAGE_CXX || lang == Language::LANGUAGE_CSHARP ||
      lang & Language::LANGUAGE_OBJECTIVE_C)
        options |= SRCML_OPTION_CPP;

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
}

void srcml_translator::prepareOutput() {

    if (!first)
        return;
    first = false;

    bool is_archive = (options & SRCML_OPTION_ARCHIVE) > 0;

    if ((options & SRCML_OPTION_NO_XML_DECL) == 0)
      out.outputXMLDecl();

    out.outputProcessingInstruction();

    // root unit for compound srcML documents

    if (is_archive) {
        out.startUnit(0, revision, url, filename, version, 0, 0, 0, attributes, true);
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
bool srcml_translator::add_unit(const srcml_unit* unit) {

    if (is_outputting_unit)
        return false;

    prepareOutput();

    // space between the previous unit and this one
    if ((options & SRCML_OPTION_ARCHIVE) > 0) {
        out.outputUnitSeparator();
    }

    // if the unit has namespaces, then use those
    Namespaces mergedns = unit->archive->namespaces;

    if (unit->namespaces) {
        mergedns += *unit->namespaces;
    }

    // if a srcdiff revision, remove the srcdiff namespace
    if (unit->archive->revision_number) {
        auto&& view = mergedns.get<nstags::uri>();
        auto it = view.find(SRCML_DIFF_NS_URI);
        if (it != view.end()) {
            view.erase(it);
        }
    }

    std::string language = unit->language ? *unit->language : Language(unit->derived_language).getLanguageString();

    // create a new unit start tag with all new info (hash value, namespaces actually used, etc.)
    out.initNamespaces(mergedns);
    auto nrevision = unit->archive->revision_number;
    out.startUnit(language.c_str(),
            (options & SRCML_OPTION_ARCHIVE) && unit->revision ? unit->revision->c_str() : revision,
            (options & SRCML_OPTION_ARCHIVE) || !unit->url       ? 0 : (nrevision ? attribute_revision(*unit->url, (int) *nrevision).c_str() : unit->url->c_str()),
            !unit->filename  ? 0 : (nrevision ? attribute_revision(*unit->filename, (int) *nrevision).c_str() : unit->filename->c_str()),
            !unit->version   ? 0 : (nrevision ? attribute_revision(*unit->version, (int) *nrevision).c_str() : unit->version->c_str()),
            !unit->timestamp ? 0 : (nrevision ? attribute_revision(*unit->timestamp, (int) *nrevision).c_str() : unit->timestamp->c_str()),
            !unit->hash      ? 0 : (nrevision ? attribute_revision(*unit->hash, (int) *nrevision).c_str() : unit->hash->c_str()),
            !unit->encoding  ? 0 : (nrevision ? attribute_revision(*unit->encoding, (int) *nrevision).c_str() : unit->encoding->c_str()),
            unit->attributes,
            false);

    // write out the contents, excluding the start and end unit tags
    int size = unit->content_end - unit->content_begin - 1;

    if (unit->archive->revision_number && issrcdiff(unit->archive->namespaces)) {

        std::string s = extract_revision(unit->srcml.c_str() + unit->content_begin, size, (int) *unit->archive->revision_number);

        xmlTextWriterWriteRawLen(out.getWriter(), BAD_CAST s.c_str(), (int) s.size());

    } else if (size > 0) {
        xmlTextWriterWriteRawLen(out.getWriter(), BAD_CAST (unit->srcml.c_str() + unit->content_begin), size);
    }

    // end the unit
    xmlTextWriterEndElement(out.getWriter());

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

    if (is_outputting_unit)
        return false;
    is_outputting_unit = true;

    first = false;

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

    while (output_unit_depth > 0)
        add_end_element();

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

    const char* used_uri = nullptr;
    if (uri == nullptr || strcmp(SRCML_SRC_NS_URI, uri) != 0) {
        used_uri = uri;
    }

    return xmlTextWriterStartElementNS(out.getWriter(), BAD_CAST prefix, BAD_CAST name, BAD_CAST used_uri) != -1;
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
    for (char * pos = text; *pos; ++pos) {

        if (*pos != '"')
            continue;

        *pos = 0;
        if (xmlTextWriterWriteString(out.getWriter(), BAD_CAST text) == -1)
            return false;

        *pos = '\"';
        if (xmlTextWriterWriteRaw(out.getWriter(), BAD_CAST "\"") == -1)
            return false;

        text = pos + 1;
    }

    return xmlTextWriterWriteString(out.getWriter(), BAD_CAST text) != -1;
}

/**
 * ~srcml_translator
 *
 * Destructor.
 */
srcml_translator::~srcml_translator() {}
