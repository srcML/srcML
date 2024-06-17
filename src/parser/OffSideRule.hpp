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

#include <antlr/TokenStream.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>

class OffSideRule : public antlr::TokenStream {

public:

    OffSideRule(antlr::TokenStream& input) : input(input) {}

    /** abstract method for getting next token */
    antlr::RefToken nextToken() override {

        const auto& token = input.nextToken();

        // convert break token to goto (just an example)
        if (token->getType() == srcMLParser::BREAK)
            token->setType(srcMLParser::GOTO);

        // replace while with if (including text)
        if (token->getType() == srcMLParser::WHILE) {
            auto token = srcMLToken::factory();
            token->setType(srcMLParser::IF);
            token->setText("if");

            return token;
        }

        return token;
    }

private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;
};

#endif
