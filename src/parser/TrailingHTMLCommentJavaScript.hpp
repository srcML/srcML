// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file TrailingHTMLCommentJavaScript.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Differentiates if "-->" starts a line comment or is two operators "--" and ">"
 */

#ifndef HTMLCOMMENTJAVASCRIPT_HPP
#define HTMLCOMMENTJAVASCRIPT_HPP

#include <antlr/TokenStream.hpp>
#include <srcMLToken.hpp>
#include <srcMLParser.hpp>
#include <deque>

class TrailingHTMLCommentJavaScript: public antlr::TokenStream {
public:
    TrailingHTMLCommentJavaScript(antlr::TokenStream& input) : input(input) {}

    virtual antlr::RefToken nextToken();
private:
    antlr::TokenStream& input;
    std::deque<antlr::RefToken> buffer;
    antlr::RefToken token = srcMLToken::factory();
    
    bool foundNonWhitespace = false;
};

#endif 
