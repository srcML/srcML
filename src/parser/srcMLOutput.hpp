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

#include <iostream>
#include "antlr/Token.hpp"
#include "srcMLParserTokenTypes.hpp"
#include <antlr/TokenStream.hpp>
#include "StreamMLParser.hpp"
#include "TokenStream.hpp"
#include "srcMLException.hpp"
#include <string>
#include <vector>
#include <srcml_types.hpp>
#include <srcml_macros.hpp>
#include <srcml.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma GCC diagnostic pop

#include <libxml/xmlwriter.h>

/**
 * srcMLOutput
 *
 * Class for outputting of srcML. Consumes produced tokens.
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


    const char * lineAttributeValue(const antlr::RefToken& token);
    const char * columnAttributeValue(const antlr::RefToken& token);
    const char * lineAttributeValue(int aline);
    const char * columnAttributeValue(int acolumn);

    int last_line, last_line2, last_column;
    bool end_position_output;
    void outputPosition();

    // destructor
    ~srcMLOutput();

public:
    /** token stream input */
    TokenStream* input;

    /** output xml writer */
    xmlTextWriter* xout;
    
    /** output buffer */
    xmlOutputBuffer * output_buffer;

    /** unit attribute language */
    const char* unit_language;

    /** unit attribute revision */
    const char* unit_revision;

    /** unit attribute url */
    const char* unit_url;

    /** unit attribute filename */
    const char* unit_filename;

    /** unit attribute version */
    const char* unit_version;

    /** unit attribute timestamp */
    const char* unit_timestamp;

    /** unit attribute hash */
    const char* unit_hash;

    /** unit attribute encoding */
    const char* unit_encoding;

    /** output options */
    OPTION_TYPE& options;

    /** xml encoding */
    const char* xml_encoding;

    /** array for a number to prefix */
    std::vector<std::string> num2prefix;

    /** array for a number to uri */
    std::vector<std::string> num2uri;

    /** an array of name-value attribute pairs */
    const std::vector<std::string> & unit_attributes;

    /** pre-root processing instruction */
    boost::optional<std::pair<std::string, std::string> > processing_instruction;

    /** number of open elements */
    int openelementcount;

    /** current line @todo is this used */
    int curline;

    /** current column @todo is this used */
    int curcolumn;

    /** the tabstop size */
    size_t tabsize;

    /** number of units output or depth into archive */
    int depth;

    /** starting time for debug stopwatch */
    boost::posix_time::ptime debug_time_start;

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

    void processUnit(const antlr::RefToken& token);

    void outputNamespaces(xmlTextWriterPtr xout, const OPTION_TYPE& options, int depth);

    void setMacroList(std::vector<std::string> & list);
    void outputMacroList();

    void srcMLTextWriterStartElement(xmlTextWriter*, const xmlChar* s);
    void srcMLTextWriterEndElement(xmlTextWriter*);

    // handler for optional literal tokens
    void processOptional(const antlr::RefToken& token, const char* attr_name, const char* attr_value);

    // token handlers
    void processAccess(const antlr::RefToken& token);
    void processPseudoBlock(const antlr::RefToken& token);
    void processToken(const antlr::RefToken& token);
    void processTypePrevious(const antlr::RefToken& token);
    void processBlockCommentStart(const antlr::RefToken& token);
    void processJavadocCommentStart(const antlr::RefToken& token);
    void processDoxygenCommentStart(const antlr::RefToken& token);
    void processLineDoxygenCommentStart(const antlr::RefToken& token);
    void processLineCommentStart(const antlr::RefToken& token);
    void processEndBlockToken(const antlr::RefToken& token);
    void processEndLineToken(const antlr::RefToken& token);
#if DEBUG
    void processMarker(const antlr::RefToken& token);
#endif
    void processString(const antlr::RefToken& token);
    void processChar(const antlr::RefToken& token);
    void processLiteral(const antlr::RefToken& token);
    void processBoolean(const antlr::RefToken& token);
    void processNull(const antlr::RefToken& token);
    void processNil(const antlr::RefToken& token);
    void processComplex(const antlr::RefToken& token);
    void processEscape(const antlr::RefToken& token);
    void processStaticAssert(const antlr::RefToken& token);
    void processClassInterface(const antlr::RefToken& token);
    void processClassImplementation(const antlr::RefToken& token);
    void processGenericArgumentList(const antlr::RefToken& token);
    void processCast(const antlr::RefToken& token);
    void processEnumClass(const antlr::RefToken& token);
    void processOperatorFunction(const antlr::RefToken& token);
    void processPseudoParameterList(const antlr::RefToken& token);
    void processIndexerParameterList(const antlr::RefToken& token);
    void processSizeofPack(const antlr::RefToken& token);
    void processCudaArgumentList(const antlr::RefToken& token);
    void processGenericParameterList(const antlr::RefToken& token);

    /** method pointer for token processing dispatch */
    typedef void (srcMLOutput::*PROCESS_PTR)(const antlr::RefToken & );

private:

    int consume_next();

    void outputToken(const antlr::RefToken& token);

    /** list of element names */
    static const char* const ElementNames[];

    /** list of element prefixes */
    static int ElementPrefix[];

    /* table of method pointers for token processing dispatch */
    static char process_table[];

    /** table for conversion from number to process */
    static srcMLOutput::PROCESS_PTR num2process[];

};

#endif
