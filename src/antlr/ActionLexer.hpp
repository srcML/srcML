// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ActionLexer.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.actions.cpp.ActionLexer, which translates the special symbols
 * ($setType(), #id, ...) in C++ actions.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_ACTIONLEXER_HPP
#define ANTLR2CPP_ACTIONLEXER_HPP

#include "Scanner.hpp"

namespace antlrcpp {

class RuleBlock;
class CppCodeGenerator;
struct ActionTransInfo;
class Tool;

class ActionLexer : public CharScanner {
public:
    ActionLexer(const std::string& s, RuleBlock* currentRule, CppCodeGenerator* generator, ActionTransInfo* transInfo);

    void setLineOffset(int lineOffset) { line = lineOffset; }
    void setTool(Tool* tool) { antlrTool = tool; }
    void reportError(const RecognitionException& e);
    void reportError(const std::string& s);
    void reportWarning(const std::string& s);

    void mACTION(bool createToken);

private:
    void mSTUFF(bool createToken);
    void mAST_ITEM(bool createToken);
    void mTEXT_ITEM(bool createToken);
    void mCOMMENT(bool createToken);
    void mSTRING(bool createToken);
    void mCHAR(bool createToken);
    void mTREE(bool createToken);
    void mWS(bool createToken);
    void mID(bool createToken);
    void mVAR_ASSIGN(bool createToken);
    void mAST_CONSTRUCTOR(bool createToken);
    void mTEXT_ARG(bool createToken);
    void mTREE_ELEMENT(bool createToken);
    bool mID_ELEMENT(bool createToken);
    void mAST_CTOR_ELEMENT(bool createToken);
    void mINT(bool createToken);
    void mARG(bool createToken);
    void mTEXT_ARG_ELEMENT(bool createToken);
    void mTEXT_ARG_ID_ELEMENT(bool createToken);
    void mINT_OR_FLOAT(bool createToken);
    void mSL_COMMENT(bool createToken);
    void mML_COMMENT(bool createToken);
    void mESC(bool createToken);
    void mDIGIT(bool createToken);

    // helpers for optional whitespace that is removed from the text
    void strippedWS();

    RuleBlock* currentRule;
    CppCodeGenerator* generator;
    Tool* antlrTool = nullptr;
    ActionTransInfo* transInfo;
};

} // namespace antlrcpp

#endif
