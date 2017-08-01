/**
 * @file srcMLOutput.cpp
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
  Output of the XML format based on srcMLParser
*/

#include "srcMLOutput.hpp"
#include "srcMLToken.hpp"
#include "srcmlns.hpp"
#include <srcml.h>
#include <cstring>
#include <sstream>
#ifdef _MSC_BUILD
#include <io.h>
#define snprintf _snprintf
#endif

// Definition of elements, including name, URI, attributes, and special processing
// Included to take advantage of inlined methods
#include <srcMLOutputElements.hpp>

#define SRCML_OPTION_NO_REVISION ((unsigned long long)1 << 63)

/**
 * srcMLOutput
 * @param ints a token stream
 * @param filename if output to a file
 * @param language the unit language
 * @param xml_enc output encoding
 * @param op output operations
 * @param ts tabstop size
 * @param output_buffer if output is to a output buffer
 *
 * Constructor. Handles all outputs.
 */
srcMLOutput::srcMLOutput(TokenStream* ints,
                         xmlOutputBuffer* output_buffer,
                         const char* language,
                         const char* xml_enc,
                         OPTION_TYPE& op,
                         const std::vector<std::string> & attributes,
                         boost::optional<std::pair<std::string, std::string> > processing_instruction,
                         size_t ts)
    : input(ints), output_buffer(output_buffer), unit_language(language), 
      options(op), xml_encoding(xml_enc), unit_attributes(attributes), processing_instruction(processing_instruction),
      tabsize(ts)
{
}

/**
 * initWriter
 *
 * Initializes output xmlWriter.  Supports delayed initialization.
 */
int srcMLOutput::initWriter() {

    // open the output text writer stream
    xout = xmlNewTextWriter(output_buffer);
    if (!xout) {

        fprintf(stderr, "srcml: " "Unable to open output buffer\n");
        return SRCML_STATUS_ERROR;
    }

    return SRCML_STATUS_OK;
}

/**
 * initNamespaces
 *
 * Initialize the output namespaces.
 */
void srcMLOutput::initNamespaces(const std::vector<std::string>& prefix, const std::vector<std::string>& uri) {

    namespaces = {
        { SRCML_SRC_NS_PREFIX_DEFAULT,          SRCML_SRC_NS_URI,                     false },
        { SRCML_CPP_NS_PREFIX_DEFAULT,          SRCML_CPP_NS_URI,                     false },
        { SRCML_ERR_NS_PREFIX_DEFAULT,          SRCML_ERR_NS_URI,                     false },
        { SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,  SRCML_EXT_LITERAL_NS_URI,             false },
        { SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT, SRCML_EXT_OPERATOR_NS_URI,            false },
        { SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT, SRCML_EXT_MODIFIER_NS_URI,            false },
        { SRCML_EXT_POSITION_NS_PREFIX_DEFAULT, SRCML_EXT_POSITION_NS_URI,            false },
        { SRCML_EXT_OPENMP_NS_PREFIX_DEFAULT,   SRCML_EXT_OPENMP_NS_URI,              false },
    };

    for (std::vector<std::string>::size_type outer_pos = 0; outer_pos < uri.size(); ++outer_pos) {

        const std::string& value = uri[outer_pos];

        // find where the new URI is in the default URI list, or not
        auto posit = std::find_if(namespaces.begin(), namespaces.end(), [value](const Namespace& n) { return n.uri == value; });
        if (posit != namespaces.end()) {

            // update the default prefix
            posit->prefix = prefix[outer_pos];

        } else {

            // create a new entry for this URI
            namespaces.push_back({ prefix[outer_pos], uri[outer_pos], false });
        }
    }

    // setup attributes names for line/column position if used
    if (isoption(options, SRCML_OPTION_POSITION)) {

        lineAttribute = namespaces[POS].prefix + ":line;";

        line2Attribute = namespaces[POS].prefix + ":line2";

        columnAttribute = namespaces[POS].prefix + ":column";
    }
}

/**
 * ~srcMLOutput
 *
 * Destructor.  Closes output.
 */
srcMLOutput::~srcMLOutput() {

    close();
}

/**
 * close
 *
 * Close/finish the output.
 */
void srcMLOutput::close() {

    if (xout) {

        xmlTextWriterEndDocument(xout);
        xmlFreeTextWriter(xout);
        xout = 0;
        output_buffer = 0;

    } else if(output_buffer) {

        xmlOutputBufferClose(output_buffer);
        output_buffer = 0;
    }
}

/**
 * srcMLTextWriterStartElement
 * @param xout xml writer
 * @param s name of element to start
 *
 * Start output of element s in writer xout.
 */
inline void srcMLOutput::srcMLTextWriterStartElement(xmlTextWriter* xout, const xmlChar* s) {

    xmlTextWriterStartElement(xout, s);
    ++openelementcount;
}

/**
 * srcMLTextWriterEndElement
 * @param xout xml writer
 *
 * End element in writer xout.
 */
void srcMLOutput::srcMLTextWriterEndElement(xmlTextWriter* xout) {

    xmlTextWriterEndElement(xout);
    --openelementcount;
}

void srcMLOutput::outputUnitSeparator() {
    
    processText("\n\n", 2);
}

/**
 * lineAttributeValue
 * @param aline to convert to string
 *
 * Convert the line to string attribute.
 *
 * @returns the line as a string.
 */
void srcMLOutput::outputPosition() {

    if (end_position_output)
        return;

    const char * position_localname = "position";
    const char* prefix = namespaces[3].prefix.c_str();
    namespaces[3].used = true;

    if (prefix[0] == 0)
        xmlTextWriterStartElement(xout, BAD_CAST position_localname);
    else
        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST position_localname, 0);

    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST std::to_string(last_line).c_str());

    if(isoption(options,SRCML_OPTION_LINE))
        xmlTextWriterWriteAttribute(xout, BAD_CAST line2Attribute.c_str(), BAD_CAST std::to_string(last_line2).c_str());

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST std::to_string(last_column).c_str());

    xmlTextWriterEndElement(xout);

    end_position_output = true;
}

/**
 * setTokenStream
 * @param ints token stream to set to.
 *
 * Set the token stream.
 */
void srcMLOutput::setTokenStream(TokenStream& ints) {

    input = &ints;
}

/**
 * consume
 * @param language unit language attribute
 * @param revision unit revision attribute
 * @param url unit url attribute
 * @param filename unit filename attribute
 * @param version unit version attribute
 * @param timestamp unit timestamp attribute
 * @param hash unit hash attribute
 *
 * Start consumption of tokens/parsing of source code with unit attributes.
 */
void srcMLOutput::consume(const char* language, const char* revision, const char* url, const char* filename,
                          const char* version, const char* timestamp, const char* hash, const char* encoding) {

    // store attributes so that first occurrence of unit element will be correct
    unit_revision = revision;
    unit_url = url;
    unit_filename = filename;
    unit_version = version;
    unit_timestamp = timestamp;
    unit_language = language;
    unit_hash = hash;
    unit_encoding = encoding;

    if (!isoption(options, SRCML_OPTION_INTERACTIVE)) {

        // consume all input until EOF
        while (consume_next() != antlr::Token::EOF_TYPE) {}

    } else {

        // consume all input until EOF
        while (consume_next() != antlr::Token::EOF_TYPE) {

            // in interactive mode flush after each token is discovered
            xmlTextWriterFlush(xout);
        }
    }
}

/**
 * consume_next
 *
 * Consume/get and output the next token.
 *
 * @returns the time of consumed token
 */
int srcMLOutput::consume_next() {

    const antlr::RefToken& token = input->nextToken();

    outputToken(token);

    return token->getType();
}

/**
 * outputXMLDecl
 *
 * Output the starting xml declaration.
 */
void srcMLOutput::outputXMLDecl() {

    // issue the xml declaration, but only if we want to
    if (depth == 0 && isoption(options, SRCML_OPTION_XML_DECL))
        xmlTextWriterStartDocument(xout, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);
}

/**
 * outputPreRootProcessingInstruction
 *
 * Output a pre-root preprocessing instruction.
 */
void srcMLOutput::outputPreRootProcessingInstruction() {

    if (depth == 0 && processing_instruction) {

        xmlTextWriterStartPI(xout, BAD_CAST processing_instruction->first.c_str());
        xmlTextWriterWriteString(xout, BAD_CAST processing_instruction->second.c_str());
        xmlTextWriterEndPI(xout);
        xmlTextWriterWriteString(xout, BAD_CAST "\n");
    }
}

/**
 * outputNamesapces
 * @param xout the xml writer to write namespaces
 * @param options the current set options
 * @param depth the depth in the archive number of output units
 * @param outer is this an outer unit or inner unit
 *
 * Output the namespaces on the units.
 */
void srcMLOutput::outputNamespaces(xmlTextWriterPtr xout, const OPTION_TYPE& options, int depth) {

    // figure out which namespaces are needed
    char const * const ns[] = {

        // main srcML namespace declaration always used
        (depth == 0) ? SRCML_SRC_NS_URI : 0,

        // main cpp namespace declaration
        isoption(options, SRCML_OPTION_CPP) && (isoption(options, SRCML_OPTION_ARCHIVE) == !(depth == 0)) ? SRCML_CPP_NS_URI : 0,

        // optional debugging xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_DEBUG)    ? SRCML_ERR_NS_URI : 0,

        // optional literal xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_OPTIONAL_MARKUP) && isoption(options, SRCML_OPTION_LITERAL)  ? SRCML_EXT_LITERAL_NS_URI : 0,

        // optional operator xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_OPTIONAL_MARKUP) && isoption(options, SRCML_OPTION_OPERATOR) ? SRCML_EXT_OPERATOR_NS_URI : 0,

        // optional modifier xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_OPTIONAL_MARKUP) && isoption(options, SRCML_OPTION_MODIFIER) ? SRCML_EXT_MODIFIER_NS_URI : 0,

        // optional position xml namespace
        (depth == 0) && isoption(options, SRCML_OPTION_POSITION) ? SRCML_EXT_POSITION_NS_URI : 0,

        // optional position xml namespace
        (false && depth == 0) && isoption(options, SRCML_OPTION_OPENMP) ? SRCML_EXT_OPENMP_NS_URI : 0,
    };

    // output the namespaces
    // record length of namespaces list
    ns_list_size = 0;
    for (unsigned int i = 0; i < sizeof(ns) / sizeof(ns[0]); ++i) {

        if (i == 0 && depth > 0)
            continue;

        if (!ns[i])
            continue;

        std::string prefix = "xmlns";
        if (namespaces[i].prefix[0] != '\0') {
            prefix += ':';
            prefix += namespaces[i].prefix;
        }

        ns_list_size += prefix.size();
        ns_list_size += strlen(ns[i]);
        ns_list_size += 4;

        xmlTextWriterWriteAttribute(xout, BAD_CAST prefix.c_str(), BAD_CAST ns[i]);
    }

    if(depth == 0) {

        for(std::vector<std::string>::size_type pos =  OMP + 1; pos < namespaces.size(); ++pos) {

            std::string prefix = "xmlns";
            if (namespaces[pos].prefix[SRC] != '\0') {
                prefix += ':';
                prefix += namespaces[pos].prefix;
         }

            xmlTextWriterWriteAttribute(xout, BAD_CAST prefix.c_str(), BAD_CAST namespaces[pos].uri.c_str());
        }
    }
}

/**
 * startUnit
 * @param language the language attribute
 * @param revision what version of srcML
 * @param url the url attribute
 * @param filename the filename attribute
 * @param version the version attribute
 * @param timestamp the timestamp attribute
 * @param hash the hash attribute
 * @param encoding the encoding attribute
 * @param outer is this an outer or inner unit
 *
 * Output the start of a unit tag.
 */
void srcMLOutput::startUnit(const char* language, const char* revision,
                            const char* url, const char* filename,
                            const char* version, const char* timestamp,
                            const char* hash,
                            const char* encoding,
                            const std::vector<std::string> & attributes,
                            bool output_macrolist) {

    // recording which namespaces are used on this unit
    for (auto& ns : namespaces)
        ns.used = false;

    const char* prefix = namespaces[0].prefix.c_str();
    std::string maintag = prefix ? prefix : "";
    if (!maintag.empty())
        maintag += ":";
    maintag += "unit";

    // start of main tag
    srcMLTextWriterStartElement(xout, BAD_CAST /* type2name(SUNIT) */ maintag.c_str());

    // record where unit start tag name ends
    start_ns_pos = 1 + (int) maintag.size() + 1;

    // outer units have namespaces
    if (/* outer && */ isoption(options, SRCML_OPTION_NAMESPACE_DECL)) {
        outputNamespaces(xout, options, depth);
    }

    // setting up for tabs, even if not used
    std::ostringstream stabs;
    std::string tabattribute;
    if (isoption(options, SRCML_OPTION_POSITION)) {
        stabs << tabsize;
        tabattribute = namespaces[POS].prefix;
        tabattribute.append(":tabs");
    }

    std::string soptions;
    std::string SEP;
    //if(isoption(options, SRCML_OPTION_XML_DECL))        { soptions = "XMLDECL"; }
    //if(isoption(options, SRCML_OPTION_NAMESPACE_DECL))  { if(soptions != "") SEP = ","; soptions += SEP + "NAMESPACEDECL"; }
    if(isoption(options, SRCML_OPTION_CPP_TEXT_ELSE))  { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "CPP_TEXT_ELSE"; }
    if(isoption(options, SRCML_OPTION_CPP_MARKUP_IF0)) { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "CPP_MARKUP_IF0"; }
    if(isoption(options, SRCML_OPTION_LINE))           { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "LINE"; }
    if(isoption(options, SRCML_OPTION_NESTIF))         { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "NESTIF"; }
    if(isoption(options, SRCML_OPTION_CPPIF_CHECK))    { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "CPPIF_CHECK"; }
    if(isoption(options, SRCML_OPTION_WRAP_TEMPLATE))  { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "WRAP_TEMPLATE"; }
    if(!isoption(options, SRCML_OPTION_TERNARY))       { if(SEP.empty() && soptions != "") SEP = ","; soptions += SEP + "OPERATOR_TERNARY"; }

    std::string stab = stabs.str();

    // list of attributes
    const char* const attrs[][2] = {

        { UNIT_ATTRIBUTE_REVISION, !isoption(options, SRCML_OPTION_NO_REVISION) ? revision : 0 },

        // language attribute
        { UNIT_ATTRIBUTE_LANGUAGE, language },

        // url attribute
        { UNIT_ATTRIBUTE_URL, url },

        // filename attribute
        { UNIT_ATTRIBUTE_FILENAME, filename },

        // version attribute
        { UNIT_ATTRIBUTE_VERSION, version },

        // position tab setting
        { tabattribute.c_str(), isoption(options, SRCML_OPTION_POSITION) ? stab.c_str() : 0 },

        // timestamp attribute
        { UNIT_ATTRIBUTE_TIMESTAMP, timestamp },

        // hash attribute
        { UNIT_ATTRIBUTE_HASH, hash },

        // source encoding attribute
        { UNIT_ATTRIBUTE_SOURCE_ENCODING, isoption(options, SRCML_OPTION_STORE_ENCODING) ? encoding : 0 },

        { UNIT_ATTRIBUTE_OPTIONS,  depth == 0 && (isoption(options, SRCML_OPTION_NESTIF)
         || isoption(options, SRCML_OPTION_CPPIF_CHECK) || isoption(options, SRCML_OPTION_WRAP_TEMPLATE) || !isoption(options, SRCML_OPTION_TERNARY)) ? soptions.c_str() : 0 },

    };

    // output attributes
    for (unsigned int i = 0; i < sizeof(attrs) / sizeof(attrs[0]); ++i) {
        if (!attrs[i][1])
            continue;

        xmlTextWriterWriteAttribute(xout, BAD_CAST attrs[i][0], BAD_CAST attrs[i][1]);
    }

    for(std::vector<std::string>::size_type pos = 0; pos < attributes.size(); pos += 2) {

        xmlTextWriterWriteAttribute(xout, BAD_CAST attributes[pos].c_str(), BAD_CAST attributes[pos + 1].c_str());
    }

    if (output_macrolist)
        outputMacroList();

    ++depth;
}

/**
 * setMacroList
 * @param list the user defined macro list
 *
 * Set the macro list to use for output.
 */
void srcMLOutput::setMacroList(std::vector<std::string> & list) {

    user_macro_list = list;
}

/**
 * outputMacroList
 *
 * Output the stored user defined macro list meta data.
 */
void srcMLOutput::outputMacroList() {

    for(std::vector<std::string>::size_type i = 0; i < user_macro_list.size(); i += 2) {

        xmlTextWriterStartElement(xout, BAD_CAST "macro-list");
        xmlTextWriterWriteAttribute(xout, BAD_CAST "token", BAD_CAST user_macro_list[i].c_str());
        xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST user_macro_list[i + 1].c_str());
        xmlTextWriterEndElement(xout);
    }
}

/**
 * processUnit
 * @param token token to output
 *
 * Callback to process/output unit token.
 */
void srcMLOutput::processUnit(const antlr::RefToken& token) {

    if (isstart(token)) {

        // keep track of number of open elements
        openelementcount = 0;
        startUnit(unit_language, unit_revision, unit_url, unit_filename, unit_version, unit_timestamp, unit_hash, unit_encoding, unit_attributes, !isoption(options, SRCML_OPTION_ARCHIVE));

    } else {

        // end anything still open, including the unit
        while (openelementcount > 0)
            srcMLTextWriterEndElement(xout);

        // output the namespaces
        // record length of namespaces list
        reduced_ns = "";
        // Note: Skipping first namespace (srcML)
        for (unsigned int i = 1; i < namespaces.size(); ++i) {

            if (!namespaces[i].used)
                continue;

            std::string prefix = "xmlns";
            if (namespaces[i].prefix[SRC] != '\0') {
                prefix += ':';
                prefix += namespaces[i].prefix;
            }

            reduced_ns += prefix;
            reduced_ns += "=\"";
            reduced_ns += namespaces[i].uri;
            reduced_ns += "\" ";
        }
    }
}

/**
 * processText
 * @param str text to output
 *
 * Callback to process/output text.
 */
inline void srcMLOutput::processText(const std::string& str) {

    xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) str.data(), (int)str.size());
}

/**
 * processText
 * @param s string to output
 * @param size of bytes to output
 *
 * Callback to process/output text outputting size bytes.
 */
inline void srcMLOutput::processText(const char* s, int size) {

    xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) s, size);
}

/**
 * processText
 * @param token token to output as text
 *
 * Callback to process/output token as text.
 */
inline void srcMLOutput::processText(const antlr::RefToken& token) {

    processText(token->getText());
}

/**
 * processTextPosition
 * @param token token to output as text
 *
 * Callback to process/output token as text with position information.
 */
void srcMLOutput::processTextPosition(const antlr::RefToken& token) {

    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST std::to_string(token->getLine()).c_str());

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST std::to_string(token->getColumn()).c_str());

    last_line = token->getLine();
    last_column = token->getColumn() + (int)token->getText().size();

    end_position_output = false;

    processText(token->getText());
}

/**
 * processTextPositionLine
 * @param token token to output as text
 *
 * Callback to process/output token as text with position information using @code#line@endcode embedded in half of set line.
 */
void srcMLOutput::processTextPositionLine(const antlr::RefToken& token) {

    xmlTextWriterWriteAttribute(xout, BAD_CAST lineAttribute.c_str(), BAD_CAST std::to_string(token->getLine() & 0xFFFF).c_str());
    xmlTextWriterWriteAttribute(xout, BAD_CAST line2Attribute.c_str(), BAD_CAST std::to_string(token->getLine() >> 16).c_str());

    xmlTextWriterWriteAttribute(xout, BAD_CAST columnAttribute.c_str(), BAD_CAST std::to_string(token->getColumn()).c_str());

    last_line = token->getLine() & 0xFFFF;
    last_line2 = token->getLine() >> 16;
    last_column = token->getColumn() + (int)token->getText().size();

    end_position_output = false;

    processText(token->getText());
}

inline void srcMLOutput::processToken(const antlr::RefToken& token, const char* name, const char* prefix, const char* attr_name, const char* attr_value) {

    processToken(token, name, prefix, attr_name, attr_value, 0, 0);
}

void srcMLOutput::processToken(const antlr::RefToken& token, const char* name, const char* prefix, const char* attr_name1, const char* attr_value1,
                                const char* attr_name2, const char* attr_value2) {

	if (name[0] == 0)
		return;
	
    if (isstart(token) || isempty(token)) {
        if (prefix[0] == 0)
            xmlTextWriterStartElement(xout, BAD_CAST name);
        else {
            xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST name, 0);
        }
        ++openelementcount;

        if (attr_name1)
            xmlTextWriterWriteAttribute(xout, BAD_CAST attr_name1, BAD_CAST attr_value1);

        if (attr_name2)
            xmlTextWriterWriteAttribute(xout, BAD_CAST attr_name2, BAD_CAST attr_value2);
    } 

    if (!isstart(token) || isempty(token)) {

        if (isoption(options, SRCML_OPTION_POSITION) && !isempty(token))
            outputPosition();

        xmlTextWriterEndElement(xout);
        --openelementcount;
    }
}

/**
 * outputToken
 * @param token token to output
 *
 * Get callback that output token and output.
 */
inline void srcMLOutput::outputToken(const antlr::RefToken& token) {

    if (SUNIT == token->getType()) {
        processUnit(token);
        return;
    }

    auto search = process.find(token->getType());
    if (search != process.end() && search->second.name) {
        const Element& eparts = search->second;
        processToken(token, eparts.name, namespaces[eparts.prefix].prefix.c_str(), eparts.attr_name, 
            eparts.attr_name && !eparts.attr_value ? token->getText().c_str() : eparts.attr_value,
            eparts.attr2_name, eparts.attr2_value);

        return;
    }

    processText(token);
}

/**
 * setOutputBuffer
 * @param output_buffer an output buffer
 *
 * Set to output to output_buffer.  Should be called before initWriter.
 */
void srcMLOutput::setOutputBuffer(xmlOutputBufferPtr output_buffer) {

    this->output_buffer = output_buffer;
}

/**
 * setDepth
 * @param thedepth depth to set to
 *
 * Set the current depth to thedepth
 */
void srcMLOutput::setDepth(int thedepth) {

    depth = thedepth;
}
