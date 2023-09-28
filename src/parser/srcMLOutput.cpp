// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLOutput.cpp
 *
 * @copyright Copyright (C) 2003-2019 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Output of the XML format based on srcMLParser
 */

#include <srcMLOutput.hpp>
#undef CONST
#undef VOID
#undef DELETE
#undef INTERFACE
#undef OUT
#undef IN
#undef THIS
#include <srcMLParserTokenTypes.hpp>
#include <srcMLToken.hpp>

// Definition of elements, including name, URI, attributes, and special processing
// Included to take advantage of inlined methods
#include <srcMLOutputElements.hpp>

namespace {
    void srcMLTextWriterWriteNamespace(xmlTextWriterPtr xout, const Namespace& ns) {

        std::string prefix = "xmlns";
        if (!ns.prefix.empty()) {
            prefix += ':';
            prefix += ns.prefix;
        }

        xmlTextWriterWriteAttribute(xout, BAD_CAST prefix.data(), BAD_CAST ns.uri.data());
    }

    // itoa-type function
    inline const char* positoa(int n) {

        // enough space to store int as string
        static constexpr int SIZE = sizeof(int) * 4;
        thread_local char s[SIZE] = { 0 };

        // create string backwards from 1's place
        char* p = s + SIZE - 1;
        do {
            *--p = '0' + (n % 10);
            n /= 10;
        } while (n);

        // end of string that we are using
        return p;
    }
}

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
                         const Attributes& attributes,
                         const std::optional<std::pair<std::string, std::string>>& processing_instruction,
                         size_t ts)
    : input(ints), output_buffer(output_buffer), unit_language(language),
      options(op), xml_encoding(xml_enc), attributes(attributes), processing_instruction(processing_instruction),
      tabsize(ts)
{
    // open the output text writer stream
    xout = xmlNewTextWriter(output_buffer);
}

/**
 * initNamespaces
 *
 * Initialize the output namespaces.
 */
void srcMLOutput::initNamespaces(const Namespaces& otherns) {

    // start with the default
    namespaces = default_namespaces;

    // merge in the other namespaces
    namespaces += otherns;
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

        if (didwrite)
            xmlTextWriterEndDocument(xout);
        xmlFreeTextWriter(xout);
        xout = 0;
        output_buffer = 0;

    } else if (output_buffer) {

        xmlOutputBufferClose(output_buffer);
        output_buffer = 0;
    }
}

void srcMLOutput::outputUnitSeparator() {

    processText("\n\n", 2);
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

    while (1) {
        const antlr::RefToken& token = input->nextToken();
        if (token->getType() == antlr::Token::EOF_TYPE)
            break;

        outputToken(token);
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
    if (depth == 0 && !isoption(options, SRCML_PARSER_OPTION_NO_XML_DECL))
        xmlTextWriterStartDocument(xout, XML_VERSION.data(), xml_encoding, XML_DECLARATION_STANDALONE.data());
}

/**
 * outputProcessingInstruction
 *
 * Output a pre-root preprocessing instruction.
 */
void srcMLOutput::outputProcessingInstruction() {

    if (depth == 0 && processing_instruction) {

        didwrite = true;

        xmlTextWriterStartPI(xout, BAD_CAST processing_instruction->first.data());
        xmlTextWriterWriteString(xout, BAD_CAST processing_instruction->second.data());
        xmlTextWriterEndPI(xout);
        xmlTextWriterWriteString(xout, BAD_CAST "\n");
    }
}

/**
 * outputNamespaces
 *
 * Output the namespaces on the units.
 */
void srcMLOutput::outputNamespaces() {

    // based on options, turn on specific namespaces (i.e., mark as used)
    if (isoption(options, SRCML_OPTION_CPP_DECLARED))
        findNSURI(namespaces, SRCML_CPP_NS_URI)->flags |= NS_USED;

    if (isoption(options, SRCML_PARSER_OPTION_POSITION))
        findNSURI(namespaces, SRCML_POSITION_NS_URI)->flags |= NS_USED;

    if (isoption(options, SRCML_OPTION_DEBUG))
        findNSURI(namespaces, SRCML_ERROR_NS_URI)->flags |= NS_USED;

    for (const auto& ns : namespaces) {

        // output standard namespaces for outer unit or non-archive unit
        if (depth == 0 || !isoption(options, SRCML_OPTION_ARCHIVE)) {

            // must be required, or on the root and used
            if ((ns.flags & NS_STANDARD) && ((ns.flags & NS_REQUIRED) || ((ns.flags & NS_ROOT) && (ns.flags & NS_USED)))) {
                srcMLTextWriterWriteNamespace(xout, ns);
                continue;
            }

            // must be user registered
            if (ns.flags & NS_REGISTERED && !(ns.flags & NS_STANDARD)) {
                srcMLTextWriterWriteNamespace(xout, ns);
                continue;
            }
        }

        // output standard namespaces for inner unit or non-archive unit
        if (depth == 1 || !isoption(options, SRCML_OPTION_ARCHIVE)) {

            // must be required, must not be on the root, and must be used
            if ((ns.flags & NS_STANDARD) && !(ns.flags & NS_ROOT) && !(ns.flags & NS_REQUIRED) && (ns.flags & NS_USED)) {
                srcMLTextWriterWriteNamespace(xout, ns);
                continue;
            }
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
                            const Attributes& attributes,
                            bool output_macrolist) {

    // go with default encoding
    if (!encoding || encoding[0] == '\0') {
        encoding = "UTF-8";
    }

    didwrite = true;

    // start of main tag
    std::string_view unitprefix = namespaces[SRC].prefix;
    xmlTextWriterStartElementNS(xout, BAD_CAST (!unitprefix.empty() ? unitprefix.data() : 0), BAD_CAST "unit", 0);
    ++openelementcount;

    // output namespaces for root and nested units
    if (isoption(options, SRCML_OPTION_NAMESPACE_DECL)) {
        outputNamespaces();
    }

    // options
    std::string soptions;
    if (isoption(options, SRCML_PARSER_OPTION_CPP_TEXT_ELSE))
        soptions = "CPP_TEXT_ELSE";

    if (isoption(options, SRCML_PARSER_OPTION_CPP_MARKUP_IF0)) {
        if (!soptions.empty())
            soptions += ',';
        soptions = "CPP_MARKUP_IF0";
    }

    if (isoption(options, SRCML_OPTION_LINE)) {
        if (!soptions.empty())
            soptions += ',';
        soptions = "LINE";
    }

    // revision standard attribute
    if (revision) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_REVISION.data(), BAD_CAST revision);
    }

    // language standard attribute
    if (language) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_LANGUAGE.data(), BAD_CAST language);
    }

    // url standard attribute
    if (url) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_URL.data(), BAD_CAST url);
    }

    // filename standard attribute
    if (filename) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_FILENAME.data(), BAD_CAST filename);
    }

    // version standard attribute
    if (version) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_VERSION.data(), BAD_CAST version);
    }

    // tabsize standard attribute
    if (isoption(options, SRCML_PARSER_OPTION_POSITION)) {
        std::string tabattribute(namespaces[POS].getPrefix());
        if (!tabattribute.empty())
            tabattribute += ':';
        for (auto c : "tabs"sv)
            tabattribute += c;
        auto stabsize = std::to_string(tabsize);
        xmlTextWriterWriteAttribute(xout, BAD_CAST tabattribute.data(), BAD_CAST stabsize.data());
    }

    // timestamp standard attribute
    if (timestamp) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_TIMESTAMP.data(), BAD_CAST timestamp);
    }

    // hash standard attribute
    if (hash) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_HASH.data(), BAD_CAST hash);
    }

    // encoding standard attribute
    if (isoption(options, SRCML_PARSER_OPTION_STORE_ENCODING)) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_SOURCE_ENCODING.data(), BAD_CAST encoding);
    }

    // options standard attribute
    if (depth == 0 && !soptions.empty()) {
        xmlTextWriterWriteAttribute(xout, BAD_CAST UNIT_ATTRIBUTE_OPTIONS.data(), BAD_CAST soptions.data());
    }

    // custom attributes
    for (const auto& attribute : attributes) {
        std::string qName;
        if (!attribute.prefix.empty()) {
            qName += attribute.prefix;
            qName += ':';
        }
        qName += attribute.name;

        xmlTextWriterWriteAttribute(xout, BAD_CAST qName.data(), BAD_CAST attribute.value.data());
    }

    if (output_macrolist)
        outputMacroList();

    if (depth == 0)
        depth = 1;
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
        xmlTextWriterWriteAttribute(xout, BAD_CAST "token", BAD_CAST user_macro_list[i].data());
        xmlTextWriterWriteAttribute(xout, BAD_CAST "type", BAD_CAST user_macro_list[i + 1].data());
        xmlTextWriterEndElement(xout);
    }
}

/**
 * processText
 * @param str text to output
 *
 * Callback to process/output text.
 */
inline void srcMLOutput::processText(std::string_view str) {

    // skip processing empty stings
    if (str.empty())
        return;

    // output any '<', '>', or '&', or any text before
    std::size_t p = 0;
    auto lastp = p;
    while ((p = str.find_first_of("<>&"sv, p)) != str.npos) {

        // output section before
        xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) &str.data()[lastp], p - lastp);

        // output special characters
        if (str[p] == '<') {
            xmlTextWriterWriteRawLen(xout, BAD_CAST "&lt;", "&lt;"sv.size());
        } else if (str[p] == '>') {
            xmlTextWriterWriteRawLen(xout, BAD_CAST "&gt;", "&gt;"sv.size());
        } else if (str[p] == '&') {
            xmlTextWriterWriteRawLen(xout, BAD_CAST "&amp;", "&amp;"sv.size());
        }
        ++p;
        lastp = p;
    }

    // output remaining text after last '<', '>', or '&', or the entire string if these do not occur
    xmlTextWriterWriteRawLen(xout, BAD_CAST (unsigned char*) &str.data()[lastp], str.size() - lastp);
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
void srcMLOutput::addPosition(const antlr::RefToken& token) {

    srcMLToken* stoken = static_cast<srcMLToken*>(&(*token));

    // how we detect empty elements: the position is wrong
    if (stoken->endline < stoken->getLine() || (stoken->endline == stoken->getLine() && stoken->endcolumn < stoken->getColumn()))
            return;

    thread_local const std::string prefix(namespaces[POS].prefix);
    thread_local const std::string startAttribute = " " + prefix + (!prefix.empty() ? ":" : "") + "start=\"";
    thread_local const std::string endAttribute   = " " + prefix + (!prefix.empty() ? ":" : "") + "end=\"";

    // highly optimized as this is output for every start tag

    // position start attribute, e.g. pos:start="1:4"
    xmlOutputBufferWrite(output_buffer, (int) startAttribute.size(), startAttribute.data());
    xmlOutputBufferWriteString(output_buffer, positoa(token->getLine()));
    xmlOutputBufferWrite(output_buffer, 1, ":");
    xmlOutputBufferWriteString(output_buffer, positoa(token->getColumn()));
    xmlOutputBufferWrite(output_buffer, 1, "\"");

    // position end attribute, e.g. pos:end="2:1"
    xmlOutputBufferWrite(output_buffer, (int) endAttribute.size(), endAttribute.data());
    if (token->getLine() > stoken->endline) {
        xmlOutputBufferWriteString(output_buffer, "INVALID_POS(");
    }
    xmlOutputBufferWriteString(output_buffer, positoa(stoken->endline));
    if (token->getLine() > stoken->endline) {
        xmlOutputBufferWriteString(output_buffer, ")");
    }
    xmlOutputBufferWrite(output_buffer, 1, ":");
    xmlOutputBufferWriteString(output_buffer, positoa(stoken->endcolumn));
    xmlOutputBufferWrite(output_buffer, 1, "\"");
}

void srcMLOutput::processToken(const antlr::RefToken& token, const char* name, const char* prefix, const char* attr_name1, const char* attr_value1,
                                const char* attr_name2, const char* attr_value2) {

    // no name, no token
    if (name[0] == 0)
        return;

    bool isposition = isoption(options, SRCML_PARSER_OPTION_POSITION);

    if (isstart(token) || isempty(token)) {

        // empty prefixes have to be null for output
        if (prefix[0] == '\0')
            prefix = 0;

        xmlTextWriterStartElementNS(xout, BAD_CAST prefix, BAD_CAST name, 0);
        ++openelementcount;

        if (attr_name1)
            xmlTextWriterWriteAttribute(xout, BAD_CAST attr_name1, BAD_CAST attr_value1);

        if (attr_name2)
            xmlTextWriterWriteAttribute(xout, BAD_CAST attr_name2, BAD_CAST attr_value2);

        // if position attributes for non-empty start elements
        if (isposition && !isempty(token))
            addPosition(token);
    }

    if (!isstart(token) || isempty(token)) {

        --openelementcount;
        xmlTextWriterEndElement(xout);
    }
}

/**
 * outputToken
 * @param token token to output
 *
 * Get callback that output token and output.
 */
inline void srcMLOutput::outputToken(const antlr::RefToken& token) {

    // unit element is handled specially
    if (srcMLParserTokenTypes::SUNIT == token->getType())
        return;

    // find the token in the element map. If found and it has a name, then process the token
    auto search = process.find(token->getType());
    if (search != process.end() && search->second.name) {
        const Element& eparts = search->second;

        // process the token using the fields in the element
        processToken(token, eparts.name,
                    // use getPrefix() to record that this prefix was used
                    namespaces[eparts.prefix].getPrefix().data(),
                    eparts.attr_name,
                    // if attribute name and no value, then take text from token
                    eparts.attr_name && eparts.attr_value ? eparts.attr_value : token->getText().data(),
                    eparts.attr2_name,
                    eparts.attr2_value);

        return;
    }

    // remainder are treated as text tokens
    processText(token);
}
