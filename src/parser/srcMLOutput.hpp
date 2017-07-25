/**
 * @file srcMLOutput.hpp
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

/**
   XML output
*/

#ifndef SRCMLOUTPUT_HPP
#define SRCMLOUTPUT_HPP

#include "antlr/Token.hpp"
#include "srcMLParserTokenTypes.hpp"
#include <antlr/TokenStream.hpp>
#include "StreamMLParser.hpp"
#include "TokenStream.hpp"
#include "srcMLException.hpp"
#include <string>
#include <unordered_map>

#include <libxml/xmlwriter.h>

enum PREFIXES { SRC = 0, CPP = 1, OMP = 7 };

class srcMLOutput;

typedef void (*token_output_t)(srcMLOutput*, const antlr::RefToken&, const char* name, const char* prefix);

struct Element {
    const char* name;
    PREFIXES prefix;
    const char* attr_name;
    const char* attr_value;
    token_output_t token_output;
};

/**
 * srcMLOutput
 *
 * Class for output of srcML. Consumes produced tokens.
 * progressively running the srcML parser and consuming tokens i.e. like a pull parser.
 */
class srcMLOutput : public srcMLParserTokenTypes {
public:
    // constructor
    srcMLOutput(TokenStream* ints,
                xmlOutputBuffer * output_buffer,
                const char* language,
                const char* encoding,
                OPTION_TYPE& option,
                const std::vector<std::string> & attributes,
                boost::optional<std::pair<std::string, std::string> > processing_instruction,
                size_t tabsize);

    void setOutputBuffer(xmlOutputBufferPtr output_buffer);
    int initWriter();
    void initNamespaces(const std::vector<std::string> & prefix, const std::vector<std::string> & uri);
    xmlTextWriter * getWriter();
    void setDepth(int thedepth);

    // same srcml file can be generated from multiple input token streams
    void setTokenStream(TokenStream& ints);

    void outputXMLDecl();

    void outputPreRootProcessingInstruction();

    // start a unit element with the passed metadata
    void startUnit(const char* unit_language, const char * revision,
                   const char* unit_url, const char* unit_filename,
                   const char* unit_version, const char* unit_timestamp,
                   const char* unit_hash,
                   const char* encoding,
                   const std::vector<std::string> & attributes,
                   bool output_macrolist);

    // consume the entire tokenstream with output of srcml
    void consume(const char* language, const char * unit_revision, const char* unit_url, const char* unit_filename,
                 const char* unit_version, const char* unit_timestamp, const char* unit_hash, const char* encoding);

    // close the output
    void close();

    // standard processing of text
    void processText(const antlr::RefToken& token);
    void processText(const std::string&);
    void processText(const char* s, int size);

    void processTextPosition(const antlr::RefToken& token);
    void processTextPositionLine(const antlr::RefToken& token);

private:
    int last_line = 0;
    int last_line2 = 0;
    int last_column = 0;
    bool end_position_output = false;
public:

    void outputPosition();

    // destructor
    ~srcMLOutput();

public:
    /** token stream input */
    TokenStream* input;

    /** output xml writer */
    xmlTextWriter* xout = nullptr;
    
    /** output buffer */
    xmlOutputBuffer* output_buffer;

    /** unit attribute language */
    const char* unit_language;

    /** unit attribute revision */
    const char* unit_revision;

    /** unit attribute url */
    const char* unit_url = nullptr;

    /** unit attribute filename */
    const char* unit_filename = nullptr;

    /** unit attribute version */
    const char* unit_version = nullptr;

    /** unit attribute timestamp */
    const char* unit_timestamp = nullptr;

    /** unit attribute hash */
    const char* unit_hash = nullptr;

    /** unit attribute encoding */
    const char* unit_encoding = nullptr;

    /** output options */
    OPTION_TYPE& options;

    /** xml encoding */
    const char* xml_encoding = nullptr;

    /** array for a number to prefix */
    std::vector<std::string> num2prefix;

    /** array for a number to uri */
    std::vector<std::string> num2uri;

    /** array for number of uri's used */
    std::vector<bool> num2used;

    /** an array of name-value attribute pairs */
    const std::vector<std::string> & unit_attributes;

    /** pre-root processing instruction */
    boost::optional<std::pair<std::string, std::string> > processing_instruction;

    /** number of open elements */
    int openelementcount = 0;

    /** the tabstop size */
    size_t tabsize;

    /** number of units output or depth into archive */
    int depth = 0;

    /** line attribute content */
    std::string lineAttribute;

    /** line attribute content for second line */
    std::string line2Attribute;

    /** column attribute content */
    std::string columnAttribute;

    /** output array for line/column temporary storage */
    char out[21];

    /** user defined macro list */
    std::vector<std::string> user_macro_list;

    /**
        Info needed to rewrite srcML so that only the actually-used
        namespaces are listed. This is done external to the translator
        since the string produced here is copied to a std::string in libsrcml
    */

    /** first position of namespaces on unit */
    int start_ns_pos;

    int ns_list_size;

    /** reduced set of namespace declarations to those actually used */
    std::string reduced_ns;

    void processUnit(const antlr::RefToken& token);

    void outputNamespaces(xmlTextWriterPtr xout, const OPTION_TYPE& options, int depth);

    void setMacroList(std::vector<std::string> & list);
    void outputMacroList();

    void srcMLTextWriterStartElement(xmlTextWriter*, const xmlChar* s);
    void srcMLTextWriterEndElement(xmlTextWriter*);

    // token handlers
    void processToken(const antlr::RefToken& token);
    void processToken(const antlr::RefToken& token, const char* attr_name, const char* attr_value);
    void processToken(const antlr::RefToken& token, const char* name, const char* prefix, const char* attr_name, const char* attr_value);
    void processToken(const antlr::RefToken& token, const char* name, const char* prefix, const char* attr_name1, const char* attr_value1,
                                const char* attr_name2, const char* attr_value2);

private:

    int consume_next();

    void outputToken(const antlr::RefToken& token);

    static const std::unordered_map<int, Element> process;
};

#endif
