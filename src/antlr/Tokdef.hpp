// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Tokdef.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Reader for token vocabulary files (ANTLRTokdefLexer and ANTLRTokdefParser).
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_TOKDEF_HPP
#define ANTLR2CPP_TOKDEF_HPP

#include <string>

namespace antlrcpp {

class TokenManager;
class Tool;

// throws RecognitionException on syntax errors
void readTokdef(const std::string& input, const std::string& filename, TokenManager* tm, Tool* tool);

} // namespace antlrcpp

#endif
