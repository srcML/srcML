// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file EolEnd.hpp
 *
 * @copyright Copyright (C) 2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Injects INDENT and DEDENT tokens to the token stream
 */

#ifndef INCLUDED_EOLEND_HPP
#define INCLUDED_EOLEND_HPP

#include <antlr/TokenStream.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>

class EolEnd : public antlr::TokenStream {

public:

    EolEnd(antlr::TokenStream& input) : input(input) {}

    /** abstract method for getting next token */
    antlr::RefToken nextToken() override {

        const auto& token = input.nextToken();

        return token;
    }

private:
    antlr::TokenStream& input;
};

#endif
