// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLToken.hpp
 *
 * @copyright Copyright (C) 2004-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Specialized token for srcML markup elements.
 */

#ifndef INCLUDED_SRCMLTOKEN_HPP
#define INCLUDED_SRCMLTOKEN_HPP

#include <ANTLRRuntime.hpp>

/** anonymous enum for srcML token categories (xml based) */
enum { STARTTOKEN = 0, ENDTOKEN = 50, EMPTYTOKEN = 75 };

/** srcML tokens are the built-in tokens */
using srcMLToken = antlr::Token;

/**
 * EndTokenFactory
 *
 * Factory for creating an end element tag token.
 *
 * @returns the created end token.
 */
inline srcMLToken* EndTokenFactory(int token) {

    return new srcMLToken(token, ENDTOKEN);
}

/**
 * EmptyTokenFactory
 *
 * Factory for creating an empty element tag token.
 *
 * @returns the created empty token.
 */
inline srcMLToken* EmptyTokenFactory(int token) {

    return new srcMLToken(token, EMPTYTOKEN);
}

/**
 * StartTokenFactory
 *
 * Factory for creating a start element tag token.
 *
 * @returns the created start token.
 */
inline srcMLToken* StartTokenFactory(int token) {

    return new srcMLToken(token, STARTTOKEN);
}

/**
 * isstart
 *
 * Predicate to query if token is a start element tag token.
 *
 * @returns if is a start token.
 */
inline bool isstart(const antlr::RefToken& token) {

    return token->category == STARTTOKEN || token->category == EMPTYTOKEN;
}

inline bool ispurestart(const antlr::RefToken& token) {

    return token->category == STARTTOKEN;
}

/**
 * isempty
 *
 * Predicate to query if token is an empty element tag token.
 *
 * @returns if is an empty token.
 */
inline bool isempty(const antlr::RefToken& token) {

    return token->category == EMPTYTOKEN;
}

inline bool isend(const antlr::RefToken& token) {

    return token->category == ENDTOKEN;
}

#endif
