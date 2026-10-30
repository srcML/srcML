// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ANTLRParser.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of ANTLRLexer and ANTLRParser (antlr.g), the reader for grammar files.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_ANTLRPARSER_HPP
#define ANTLR2CPP_ANTLRPARSER_HPP

#include <string>

namespace antlrcpp {

class MakeGrammar;
class Tool;

void parseGrammar(const std::string& input, const std::string& filename, MakeGrammar* behavior, Tool* tool);

} // namespace antlrcpp

#endif
