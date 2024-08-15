// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLOutput.hpp
 *
 * @copyright Copyright (C) 2003-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * XML output
 */

#ifndef SRCMLOUTPUT_HPP
#define SRCMLOUTPUT_HPP

#include <antlr/Token.hpp>
#include <TokenStream.hpp>
#include <srcMLException.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <srcml_options.hpp>
#include <libxml/xmlwriter.h>
#include <srcmlns.hpp>
#include <Position.hpp>
#include <optional>
#include <stack>
#include <queue>

/**
 * anonymous enum for prefix positions
 */
enum PREFIXES { SRC,
       CPP,
       ERR,
       POS,
       OMP
};

class srcMLOutput;

struct Element {
    char const * const name;
    const PREFIXES prefix;
    char const * const attr_name;
    char const * const attr_value;
    char const * const attr2_name;
    char const * const attr2_value;
};

/**
 * Class for output of srcML. Consumes produced tokens.
 * progressively running the srcML parser and consuming tokens i.e. like a pull parser.
 */
class srcMLOutput {
public:

    // constructor
    srcMLOutput(TokenStream* stream,
                xmlOutputBuffer * output_buffer,
                const char* language,
                const char* encoding,
                OPTION_TYPE& option,
                const Attributes& attributes,
                const std::optional<std::pair<std::string, std::string>>& processing_instruction,
                size_t tabsize);

    // initialize the namespaces
    void initNamespaces(const Namespaces& namespaces);

    // current writer
    inline xmlTextWriter * getWriter() {
        return xout;
    }

    // same srcml file can be generated from multiple input token streams
    void setTokenStream(TokenStream& stream);

    // output the XML declaration
    void outputXMLDecl();

    // output a processing instruction
    void outputProcessingInstruction();

    // output the namespaces
    void outputNamespaces();

    // output the unit separator
    void outputUnitSeparator();

    // output the macro list
    void outputMacroList();

    // set the macro list
    void setMacroList(std::vector<std::string>& macros);

    // current namespaces
    const Namespaces& getNamespaces() const { return namespaces; }

    // start a unit element with the passed metadata
    void startUnit(const char* unit_language, const char* revision,
                   const char* unit_url, const char* unit_filename,
                   const char* unit_version, const char* unit_timestamp,
                   const char* unit_hash,
                   const char* encoding,
                   const Attributes& attributes,
                   bool output_macrolist);

    // consume the entire tokenstream with output of srcml
    void consume(const char* language, const char* unit_revision, const char* unit_url, const char* unit_filename,
                 const char* unit_version, const char* unit_timestamp, const char* unit_hash, const char* encoding);

    // close the output
    void close();

    // destructor
    ~srcMLOutput();

public:
    // output xml writer
    xmlTextWriter* xout = nullptr;

    // output buffer
    xmlOutputBuffer* output_buffer = nullptr;

private:

    // output a token
    void processText(const antlr::RefToken& token);

    // output a string
    void processText(std::string_view);

    // output a c-string
    void processText(const char* s, int size);

    // adds the position attributes to a token
    void addPosition(const antlr::RefToken& token);

    // token stream input
    TokenStream* input = nullptr;

    // unit attribute language
    const char* unit_language = nullptr;

    // unit attribute revision
    const char* unit_revision = nullptr;

    // unit attribute url
    const char* unit_url = nullptr;

    // unit attribute filename
    const char* unit_filename = nullptr;

    // unit attribute version
    const char* unit_version = nullptr;

    // unit attribute timestamp
    const char* unit_timestamp = nullptr;

    // unit attribute hash
    const char* unit_hash = nullptr;

    // unit attribute encoding
    const char* unit_encoding = nullptr;

    // output options
    OPTION_TYPE& options;

    // xml encoding
    const char* xml_encoding = nullptr;

    // namespaces declared and used
    Namespaces namespaces;

    // archive namespaces
    Namespaces archiveNamespaces;

    // attributes added
    Attributes attributes;

    // pre-root processing instruction
    std::optional<std::pair<std::string, std::string> > processing_instruction;

    // the tabstop size
    size_t tabsize = 0;

    // number of units output or depth into archive
    int depth = 0;

    // user defined macro list
    std::vector<std::string> user_macro_list;

    // if output was started
    bool startedOutput = false;

    // queue tokens for position calculation
    std::deque<antlr::RefToken> tokenQueue;

    // stack of start elements for position calculation
    std::stack<antlr::RefToken> startElementStack;

    // record position for <type prev=""/>
    Position lastTypeEndPosition;
    Position lastTypeStartPosition;

    // token handler
    void processToken(const antlr::RefToken& token, const char* name, const char* prefix,
                      const char* attr_name1, const char* attr_value1,
                      const char* attr_name2, const char* attr_value2);

    // output a token
    void outputToken(const antlr::RefToken& token);

    // map token to output element
    static const std::unordered_map<int, Element> process;
};

#endif
