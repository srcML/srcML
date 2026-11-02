// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file OffSideRule.hpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Injects INDENT and DEDENT tokens to the token stream
 */

#ifndef INCLUDED_OFFSIDERULE_HPP
#define INCLUDED_OFFSIDERULE_HPP

#include <ANTLRRuntime.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>

class OffSideRule : public antlr::TokenStream {

public:

    OffSideRule(antlr::TokenStream& input) : input(input) {}

    antlr::RefToken nextToken();

    void handleBlocks(antlr::RefToken token);

    void arrangeBlockEnds();

    void checkBracketToken(antlr::RefToken token);

    void expectBlockCheck(antlr::RefToken token);

    void setBlockStartToken(int token);

    int getBlockStartToken() const;

private:
    antlr::TokenStream& input;

    std::deque<antlr::RefToken> buffer;
    std::deque<antlr::RefToken> indentBuffer;
    std::deque<antlr::RefToken> tempBuffer;
    std::deque<int> bracketBuffer;

    antlr::RefToken tempPostWSToken = srcMLToken::factory();
    antlr::RefToken tokenAfterIndent = srcMLToken::factory();
    antlr::RefToken tempDocstringToken = srcMLToken::factory();

    int blockStartToken = -1;
    int numIndents = 0;
    int numBrackets = 0;  // encompasses (), {}, and []
    int numSpacesPerIndent = -1;

    bool isOneLineStatement = false;
    bool isFunctionOrClass = false;
    bool expectBlock = false;
    bool delayExpectBlockCheck = false;
    bool checkOneLineStatement = false;
    bool statementContinues = false;
    bool recordToken = false;
};

#endif
