/*
  srcMLOutput.hpp

  Copyright (C) 2003-2014  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

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
#include "Options.hpp"
#include "srcMLException.hpp"
#include <string>
#include <vector>

#include <libxml/xmlwriter.h>

class srcMLOutput : public srcMLParserTokenTypes {

public:
    // constructor
    srcMLOutput(TokenStream* ints,
                const char* srcml_filename,
                const char* language,
                const char* encoding,
                OPTION_TYPE& option,
                std::string * uri,
                int tabsize,
                xmlBuffer* output_buffer = 0,
                xmlOutputBuffer * output_buf = 0
                );

    void initWriter();
    xmlTextWriter * getWriter();
    void setWriter(xmlTextWriter *);
    int getDepth();
    void setDepth(int thedepth);

    static bool checkEncoding(const char* encoding);

    // same srcml file can be generated from multiple input token streams
    void setTokenStream(TokenStream& ints);

    void outputXMLDecl();

    // start a unit element with the passed metadata
    void startUnit(const char* unit_language,
                   const char* unit_directory, const char* unit_filename, const char* unit_version, bool outer);

    // consume the entire tokenstream with output of srcml
    void consume(const char* language, const char* unit_directory, const char* unit_filename, const char* unit_version = "");

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

    virtual const char * convert_num2prefix(unsigned int i) const;

    // destructor
    ~srcMLOutput();

public:
    TokenStream* input;

    xmlTextWriter* xout;

    const char* srcml_filename;
    const char* unit_language;
    const char* unit_dir;
    const char* unit_filename;
    const char* unit_version;
    OPTION_TYPE& options;
    const char* xml_encoding;
    std::string * num2prefix;
    int openelementcount;

    int curline;
    int curcolumn;
    int tabsize;

    int depth;
    xmlBuffer* output_buffer;
    xmlOutputBuffer * output_buf;

    // output line attribute content
    std::string lineAttribute;
    std::string line2Attribute;
    std::string columnAttribute;
    char out[21];
    std::vector<std::string> user_macro_list;

    void processUnit(const antlr::RefToken& token);

    void outputNamespaces(xmlTextWriterPtr xout, const OPTION_TYPE& options, int depth, bool outer);

    void setMacroList(std::vector<std::string> & list);
    void outputMacroList();

    bool isoption(const OPTION_TYPE& options) const;
    static bool isoption(const OPTION_TYPE& flag, const OPTION_TYPE& options);

    void srcMLTextWriterStartElement(xmlTextWriter*, const xmlChar* s);
    void srcMLTextWriterEndElement(xmlTextWriter*);

    // handler for optional literal tokens
    void processOptional(const antlr::RefToken& token, const char* attr_name, const char* attr_value);

    // token handlers
    void processAccess(const antlr::RefToken& token);
    void processToken(const antlr::RefToken& token);
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
    void processComplex(const antlr::RefToken& token);
    void processInterface(const antlr::RefToken& token);
    void processEscape(const antlr::RefToken& token);

    // method pointer for token processing dispatch
    typedef void (srcMLOutput::*PROCESS_PTR)(const antlr::RefToken & );

private:

    int consume_next();

    void outputToken(const antlr::RefToken& token);

    // List of element names
    static const char* const ElementNames[];
    static int ElementPrefix[];

    // table of method pointers for token processing dispatch
    static char process_table[];
    static srcMLOutput::PROCESS_PTR num2process[];

};

#endif
