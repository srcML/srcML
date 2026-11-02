// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ActionLexer.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.actions.cpp.ActionLexer (actions/cpp/action.g). The decisions
 * follow the code that ANTLR 2.7.7 generated for action.g.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "ActionLexer.hpp"
#include "CppCodeGenerator.hpp"
#include "Tool.hpp"
#include <array>

namespace antlrcpp {

namespace {

enum ActionTokenType {
    ACT_ACTION = 4,
    ACT_STUFF,
    ACT_AST_ITEM,
    ACT_TEXT_ITEM,
    ACT_TREE,
    ACT_TREE_ELEMENT,
    ACT_AST_CONSTRUCTOR,
    ACT_AST_CTOR_ELEMENT,
    ACT_ID_ELEMENT,
    ACT_TEXT_ARG,
    ACT_TEXT_ARG_ELEMENT,
    ACT_TEXT_ARG_ID_ELEMENT,
    ACT_ARG,
    ACT_ID,
    ACT_VAR_ASSIGN,
    ACT_COMMENT,
    ACT_SL_COMMENT,
    ACT_ML_COMMENT,
    ACT_CHAR,
    ACT_STRING,
    ACT_ESC,
    ACT_DIGIT,
    ACT_INT,
    ACT_INT_OR_FLOAT,
    ACT_WS,
};

struct CharSet {
    std::array<bool, 256> m{};

    bool operator()(int c) const { return c >= 0 && c < 256 && m[static_cast<std::size_t>(c)]; }

    // characters, optionally with whitespace, digits, and letters (including '_')
    static CharSet of(const std::string& chars, bool ws, bool digits, bool letters) {
        CharSet s;
        for (unsigned char c : chars)
            s.m[c] = true;
        if (ws)
            for (unsigned char c : std::string(" \t\n\r"))
                s.m[c] = true;
        if (digits)
            for (int c = '0'; c <= '9'; ++c)
                s.m[static_cast<std::size_t>(c)] = true;
        if (letters) {
            for (int c = 'a'; c <= 'z'; ++c)
                s.m[static_cast<std::size_t>(c)] = true;
            for (int c = 'A'; c <= 'Z'; ++c)
                s.m[static_cast<std::size_t>(c)] = true;
            s.m['_'] = true;
        }
        return s;
    }

    // all characters in the vocabulary '\3'..'\377' except these
    static CharSet allExcept(const std::string& chars) {
        CharSet s;
        for (int c = 3; c <= 255; ++c)
            s.m[static_cast<std::size_t>(c)] = true;
        for (unsigned char c : chars)
            s.m[c] = false;
        return s;
    }
};

const CharSet tokenSet_0 = CharSet::allExcept("#$");
const CharSet tokenSet_1 = CharSet::allExcept("*/");
const CharSet tokenSet_2 = CharSet::allExcept("\n\r\"#$'/");
const CharSet tokenSet_3 = CharSet::of(":", true, false, true);
const CharSet tokenSet_4 = CharSet::of("", true, false, false);
const CharSet tokenSet_5 = CharSet::of("(", true, false, false);
const CharSet tokenSet_6 = CharSet::of("\"$'(+:", true, true, true);
const CharSet tokenSet_7 = CharSet::allExcept("\"\\");
const CharSet tokenSet_8 = CharSet::allExcept("'\\");
const CharSet tokenSet_9 = CharSet::of(":", false, true, true);
const CharSet tokenSet_10 = CharSet::of("\"$'+:", false, true, true);
const CharSet tokenSet_11 = CharSet::of("\"$')+,:]", true, true, true);
const CharSet tokenSet_12 = CharSet::of(":", false, false, true);
const CharSet tokenSet_13 = CharSet::of("()*+,-./:<=[]", true, false, false);
const CharSet tokenSet_14 = CharSet::allExcept(">");
const CharSet tokenSet_15 = CharSet::of("\"#'():[", true, true, true);
const CharSet tokenSet_16 = CharSet::of(")*+,-/=]", true, false, false);
const CharSet tokenSet_17 = CharSet::of("=", true, false, false);
const CharSet tokenSet_18 = CharSet::of(")*+,-/]", true, false, false);
const CharSet tokenSet_19 = CharSet::of("\"#(:[", false, false, true);
const CharSet tokenSet_20 = CharSet::of("*+-/", true, false, false);
const CharSet tokenSet_21 = CharSet::of("\"#'(*+-/:[", true, true, true);
const CharSet tokenSet_22 = CharSet::of("\"$'()+,-.:[]", true, true, true);
const CharSet tokenSet_23 = CharSet::of("\"$')+:", true, true, true);
const CharSet tokenSet_24 = CharSet::of("\"$'+:", true, true, true);
const CharSet tokenSet_25 = CharSet::of("\"$')*+,-./:]", true, true, true);
const CharSet tokenSet_26 = CharSet::of("\"$')*+,-/:]", true, true, true);

bool inRange(int c) {
    return c >= 3 && c <= 255;
}

bool isWS(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isLetter(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isDigit(int c) {
    return c >= '0' && c <= '9';
}

// start of an ID: letter, '_', or ':'
bool isIdStart(int c) {
    return isLetter(c) || c == '_' || c == ':';
}

// start of TREE_ELEMENT and ARG after optional whitespace
bool isArgStart(int c) {
    return c == '"' || c == '#' || c == '\'' || c == '(' || isDigit(c) || isIdStart(c) || c == '[';
}

const char* const preprocessorDirectives[] = { "if", "define", "ifdef", "ifndef", "else", "elif", "endif",
                                               "warning", "error", "ident", "pragma", "include" };

} // namespace

ActionLexer::ActionLexer(const std::string& s, RuleBlock* currentRule, CppCodeGenerator* generator, ActionTransInfo* transInfo)
    : CharScanner(s), currentRule(currentRule), generator(generator), transInfo(transInfo) {}

void ActionLexer::reportError(const RecognitionException& e) {
    antlrTool->error("Syntax error in action: " + e.toString(), filename, line, column);
}

void ActionLexer::reportError(const std::string& s) {
    antlrTool->error(s, filename, line, column);
}

void ActionLexer::reportWarning(const std::string& s) {
    if (filename.empty())
        antlrTool->warning(s);
    else
        antlrTool->warning(s, filename, line, column);
}

void ActionLexer::strippedWS() {
    std::size_t saveIndex = text.size();
    mWS(false);
    text.resize(saveIndex);
}

void ActionLexer::mACTION(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    int cnt = 0;
    for (;;) {
        if (LA(1) == '#') {
            mAST_ITEM(false);
        } else if (LA(1) == '$') {
            mTEXT_ITEM(false);
        } else if (tokenSet_0(LA(1))) {
            mSTUFF(false);
        } else {
            if (cnt >= 1)
                break;
            throwNoViable();
        }
        ++cnt;
    }
    finishToken(createToken, token, ACT_ACTION, begin);
}

void ActionLexer::mSTUFF(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    switch (LA(1)) {
    case '"': mSTRING(false); break;
    case '\'': mCHAR(false); break;
    case '\n':
        match('\n');
        newline();
        break;
    default:
        if (LA(1) == '/' && (LA(2) == '*' || LA(2) == '/')) {
            mCOMMENT(false);
        } else if (LA(1) == '\r' && LA(2) == '\n') {
            match("\r\n");
            newline();
        } else if (LA(1) == '\\' && LA(2) == '#') {
            match('\\');
            match('#');
            text.resize(begin);
            text += "#";
        } else if (LA(1) == '/' && tokenSet_1(LA(2))) {
            match('/');
            if (!tokenSet_1(LA(1)))
                throwNoViable();
            consume();
        } else if (LA(1) == '\r') {
            match('\r');
            newline();
        } else if (tokenSet_2(LA(1))) {
            consume();
        } else {
            throwNoViable();
        }
    }
    finishToken(createToken, token, ACT_STUFF, begin);
}

void ActionLexer::mAST_ITEM(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    if (LA(1) == '#' && LA(2) == '(') {
        std::size_t saveIndex = text.size();
        match('#');
        text.resize(saveIndex);
        mTREE(true);
    } else if (LA(1) == '#' && tokenSet_3(LA(2))) {
        std::size_t saveIndex = text.size();
        match('#');
        text.resize(saveIndex);
        if (isWS(LA(1)))
            mWS(false);
        else if (!isIdStart(LA(1)))
            throwNoViable();
        mID(true);
        TokenRef id = returnToken;

        std::string idt = id->text;
        OStr mapped = generator->mapTreeId(id->text, transInfo);
        if (mapped && idt != *mapped) {
            text.resize(begin);
            text += *mapped;
        } else {
            for (auto directive : preprocessorDirectives) {
                if (idt == directive) {
                    text.resize(begin);
                    text += "#" + idt;
                    break;
                }
            }
        }

        if (tokenSet_4(LA(1)))
            mWS(false);
        if (LA(1) == '=')
            mVAR_ASSIGN(false);
    } else if (LA(1) == '#' && LA(2) == '[') {
        std::size_t saveIndex = text.size();
        match('#');
        text.resize(saveIndex);
        mAST_CONSTRUCTOR(true);
    } else if (LA(1) == '#' && LA(2) == '#') {
        match("##");
        if (currentRule) {
            std::string r = currentRule->getRuleName() + "_AST";
            text.resize(begin);
            text += r;
            if (transInfo)
                transInfo->refRuleRoot = r;
        } else {
            reportWarning("\"##\" not valid in this context");
            text.resize(begin);
            text += "##";
        }
        if (tokenSet_4(LA(1)))
            mWS(false);
        if (LA(1) == '=')
            mVAR_ASSIGN(false);
    } else {
        throwNoViable();
    }
    finishToken(createToken, token, ACT_AST_ITEM, begin);
}

void ActionLexer::mTEXT_ITEM(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();

    // optional '(' TEXT_ARG ')' after $FOLLOW and $FIRST
    auto optionalArg = [this]() -> TokenRef {
        TokenRef a;
        if (tokenSet_5(LA(1)) && tokenSet_6(LA(2)) && inRange(LA(3))) {
            if (isWS(LA(1)))
                mWS(false);
            else if (LA(1) != '(')
                throwNoViable();
            match('(');
            mTEXT_ARG(true);
            a = returnToken;
            match(')');
        }
        return a;
    };

    // required '(' TEXT_ARG ')'
    auto requiredArg = [this]() -> TokenRef {
        if (isWS(LA(1)))
            mWS(false);
        else if (LA(1) != '(')
            throwNoViable();
        match('(');
        mTEXT_ARG(true);
        TokenRef a = returnToken;
        match(')');
        return a;
    };

    if (LA(1) == '$' && LA(2) == 'F' && LA(3) == 'O') {
        match("$FOLLOW");
        TokenRef a5 = optionalArg();
        std::string rule = currentRule->getRuleName();
        if (a5)
            rule = a5->text;
        OStr setName = generator->getFOLLOWBitSet(rule, 1);
        if (!setName) {
            reportError("$FOLLOW(" + rule + "): unknown rule or bad lookahead computation");
        } else {
            text.resize(begin);
            text += *setName;
        }
    } else if (LA(1) == '$' && LA(2) == 'F' && LA(3) == 'I') {
        match("$FIRST");
        TokenRef a6 = optionalArg();
        std::string rule = currentRule->getRuleName();
        if (a6)
            rule = a6->text;
        OStr setName = generator->getFIRSTBitSet(rule, 1);
        if (!setName) {
            reportError("$FIRST(" + rule + "): unknown rule or bad lookahead computation");
        } else {
            text.resize(begin);
            text += *setName;
        }
    } else if (LA(1) == '$' && LA(2) == 'a') {
        match("$append");
        TokenRef a1 = requiredArg();
        std::string t = "text += " + a1->text;
        text.resize(begin);
        text += t;
    } else if (LA(1) == '$' && LA(2) == 's') {
        match("$set");
        if (LA(1) == 'T' && LA(2) == 'e') {
            match("Text");
            TokenRef a2 = requiredArg();
            std::string t = "{ text.erase(_begin); text += " + a2->text + "; }";
            text.resize(begin);
            text += t;
        } else if (LA(1) == 'T' && LA(2) == 'o') {
            match("Token");
            TokenRef a3 = requiredArg();
            std::string t = "_token = " + a3->text;
            text.resize(begin);
            text += t;
        } else if (LA(1) == 'T' && LA(2) == 'y') {
            match("Type");
            TokenRef a4 = requiredArg();
            std::string t = "_ttype = " + a4->text;
            text.resize(begin);
            text += t;
        } else {
            throwNoViable();
        }
    } else if (LA(1) == '$' && LA(2) == 'g') {
        match("$getText");
        text.resize(begin);
        text += "text.substr(_begin,text.length()-_begin)";
    } else {
        throwNoViable();
    }
    finishToken(createToken, token, ACT_TEXT_ITEM, begin);
}

void ActionLexer::mCOMMENT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    if (LA(1) == '/' && LA(2) == '/')
        mSL_COMMENT(false);
    else if (LA(1) == '/' && LA(2) == '*')
        mML_COMMENT(false);
    else
        throwNoViable();
    finishToken(createToken, token, ACT_COMMENT, begin);
}

void ActionLexer::mSTRING(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    match('"');
    for (;;) {
        if (LA(1) == '\\')
            mESC(false);
        else if (tokenSet_7(LA(1)))
            matchNot('"');
        else
            break;
    }
    match('"');
    finishToken(createToken, token, ACT_STRING, begin);
}

void ActionLexer::mCHAR(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    match('\'');
    if (LA(1) == '\\')
        mESC(false);
    else if (tokenSet_8(LA(1)))
        matchNot('\'');
    else
        throwNoViable();
    match('\'');
    finishToken(createToken, token, ACT_CHAR, begin);
}

void ActionLexer::mTREE(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    std::vector<std::string> terms;

    std::size_t saveIndex = text.size();
    match('(');
    text.resize(saveIndex);
    if (isWS(LA(1)))
        strippedWS();
    else if (!(LA(1) == '"' || LA(1) == '#' || LA(1) == '(' || isIdStart(LA(1)) || LA(1) == '['))
        throwNoViable();
    saveIndex = text.size();
    mTREE_ELEMENT(true);
    text.resize(saveIndex);
    TokenRef t = returnToken;
    terms.push_back(generator->processStringForASTConstructor(t->text));
    if (isWS(LA(1)))
        strippedWS();
    else if (LA(1) != ')' && LA(1) != ',')
        throwNoViable();
    while (LA(1) == ',') {
        saveIndex = text.size();
        match(',');
        text.resize(saveIndex);
        if (isWS(LA(1)))
            strippedWS();
        else if (!(LA(1) == '"' || LA(1) == '#' || LA(1) == '(' || isIdStart(LA(1)) || LA(1) == '['))
            throwNoViable();
        saveIndex = text.size();
        mTREE_ELEMENT(true);
        text.resize(saveIndex);
        TokenRef t2 = returnToken;
        terms.push_back(generator->processStringForASTConstructor(t2->text));
        if (isWS(LA(1)))
            strippedWS();
        else if (LA(1) != ')' && LA(1) != ',')
            throwNoViable();
    }
    text.resize(begin);
    text += generator->getASTCreateString(terms);
    saveIndex = text.size();
    match(')');
    text.resize(saveIndex);
    finishToken(createToken, token, ACT_TREE, begin);
}

void ActionLexer::mWS(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    int cnt = 0;
    for (;;) {
        if (LA(1) == '\r' && LA(2) == '\n') {
            match('\r');
            match('\n');
            newline();
        } else if (LA(1) == ' ') {
            match(' ');
        } else if (LA(1) == '\t') {
            match('\t');
        } else if (LA(1) == '\r') {
            match('\r');
            newline();
        } else if (LA(1) == '\n') {
            match('\n');
            newline();
        } else {
            if (cnt >= 1)
                break;
            throwNoViable();
        }
        ++cnt;
    }
    finishToken(createToken, token, ACT_WS, begin);
}

void ActionLexer::mID(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    if (isLetter(LA(1)) || LA(1) == '_')
        consume();
    else if (LA(1) == ':')
        match("::");
    else
        throwNoViable();
    while (tokenSet_9(LA(1))) {
        if (LA(1) == ':')
            match("::");
        else
            consume();
    }
    finishToken(createToken, token, ACT_ID, begin);
}

void ActionLexer::mVAR_ASSIGN(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    match('=');
    if (LA(1) != '=' && transInfo && transInfo->refRuleRoot)
        transInfo->assignToRoot = true;
    finishToken(createToken, token, ACT_VAR_ASSIGN, begin);
}

void ActionLexer::mAST_CONSTRUCTOR(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    std::size_t saveIndex = text.size();
    match('[');
    text.resize(saveIndex);
    if (isWS(LA(1)))
        strippedWS();
    else if (!(LA(1) == '"' || LA(1) == '#' || LA(1) == '(' || isDigit(LA(1)) || isIdStart(LA(1)) || LA(1) == '['))
        throwNoViable();
    saveIndex = text.size();
    mAST_CTOR_ELEMENT(true);
    text.resize(saveIndex);
    TokenRef x = returnToken;
    TokenRef y;
    if (isWS(LA(1)))
        strippedWS();
    else if (LA(1) != ',' && LA(1) != ']')
        throwNoViable();
    if (LA(1) == ',') {
        saveIndex = text.size();
        match(',');
        text.resize(saveIndex);
        if (isWS(LA(1)))
            strippedWS();
        else if (!(LA(1) == '"' || LA(1) == '#' || LA(1) == '(' || isDigit(LA(1)) || isIdStart(LA(1)) || LA(1) == '['))
            throwNoViable();
        saveIndex = text.size();
        mAST_CTOR_ELEMENT(true);
        text.resize(saveIndex);
        y = returnToken;
        if (isWS(LA(1)))
            strippedWS();
        else if (LA(1) != ']')
            throwNoViable();
    } else if (LA(1) != ']') {
        throwNoViable();
    }
    saveIndex = text.size();
    match(']');
    text.resize(saveIndex);

    std::string ys = generator->processStringForASTConstructor(x->text);
    if (y)
        ys += "," + y->text;
    text.resize(begin);
    text += generator->getASTCreateString(nullptr, ys);
    finishToken(createToken, token, ACT_AST_CONSTRUCTOR, begin);
}

void ActionLexer::mTEXT_ARG(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    if (isWS(LA(1)))
        mWS(false);
    else if (!(LA(1) == '"' || LA(1) == '$' || LA(1) == '\'' || LA(1) == '+' || isDigit(LA(1)) || isIdStart(LA(1))))
        throwNoViable();
    int cnt = 0;
    for (;;) {
        if (tokenSet_10(LA(1)) && inRange(LA(2))) {
            mTEXT_ARG_ELEMENT(false);
            if (tokenSet_4(LA(1)) && tokenSet_11(LA(2)))
                mWS(false);
            else if (!tokenSet_11(LA(1)))
                throwNoViable();
        } else {
            if (cnt >= 1)
                break;
            throwNoViable();
        }
        ++cnt;
    }
    finishToken(createToken, token, ACT_TEXT_ARG, begin);
}

void ActionLexer::mTREE_ELEMENT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    switch (LA(1)) {
    case '(': mTREE(false); break;
    case '[': mAST_CONSTRUCTOR(false); break;
    case '"': mSTRING(false); break;
    default:
        if (isIdStart(LA(1))) {
            mID_ELEMENT(false);
        } else if (LA(1) == '#' && LA(2) == '(') {
            std::size_t saveIndex = text.size();
            match('#');
            text.resize(saveIndex);
            mTREE(false);
        } else if (LA(1) == '#' && LA(2) == '[') {
            std::size_t saveIndex = text.size();
            match('#');
            text.resize(saveIndex);
            mAST_CONSTRUCTOR(false);
        } else if (LA(1) == '#' && tokenSet_12(LA(2))) {
            std::size_t saveIndex = text.size();
            match('#');
            text.resize(saveIndex);
            bool was_mapped = mID_ELEMENT(true);
            TokenRef id = returnToken;
            if (!was_mapped) {
                OStr t = generator->mapTreeId(id->text, nullptr);
                if (t) {
                    text.resize(begin);
                    text += *t;
                }
            }
        } else if (LA(1) == '#' && LA(2) == '#') {
            match("##");
            if (currentRule) {
                std::string t = currentRule->getRuleName() + "_AST";
                text.resize(begin);
                text += t;
            } else {
                reportError("\"##\" not valid in this context");
                text.resize(begin);
                text += "##";
            }
        } else {
            throwNoViable();
        }
    }
    finishToken(createToken, token, ACT_TREE_ELEMENT, begin);
}

bool ActionLexer::mID_ELEMENT(bool createToken) {
    bool mapped = false;
    TokenRef token;
    std::size_t begin = text.size();
    mID(true);
    TokenRef id = returnToken;
    if (tokenSet_4(LA(1)) && tokenSet_13(LA(2)))
        strippedWS();
    else if (!tokenSet_13(LA(1)))
        throwNoViable();

    switch (LA(1)) {
    case '(':
    case '<': {
        if (LA(1) == '<') {
            match('<');
            while (tokenSet_14(LA(1)))
                matchNot('>');
            match('>');
        }
        match('(');
        if (tokenSet_4(LA(1)) && tokenSet_15(LA(2)) && inRange(LA(3)))
            strippedWS();
        else if (!(tokenSet_15(LA(1)) && inRange(LA(2))))
            throwNoViable();
        if (LA(1) == '"' || LA(1) == '#' || LA(1) == '\'' || LA(1) == '(' || isDigit(LA(1)) || isIdStart(LA(1)) || LA(1) == '[') {
            mARG(false);
            while (LA(1) == ',') {
                match(',');
                if (isWS(LA(1)))
                    strippedWS();
                else if (!isArgStart(LA(1)))
                    throwNoViable();
                mARG(false);
            }
        } else if (!(isWS(LA(1)) || LA(1) == ')')) {
            throwNoViable();
        }
        if (isWS(LA(1)))
            strippedWS();
        else if (LA(1) != ')')
            throwNoViable();
        match(')');
        break;
    }
    case '[': {
        int cnt = 0;
        for (;;) {
            if (LA(1) == '[') {
                match('[');
                if (isWS(LA(1)))
                    strippedWS();
                else if (!isArgStart(LA(1)))
                    throwNoViable();
                mARG(false);
                if (isWS(LA(1)))
                    strippedWS();
                else if (LA(1) != ']')
                    throwNoViable();
                match(']');
            } else {
                if (cnt >= 1)
                    break;
                throwNoViable();
            }
            ++cnt;
        }
        break;
    }
    case '.':
        match('.');
        mID_ELEMENT(false);
        break;
    case ':':
        match("::");
        mID_ELEMENT(false);
        break;
    default:
        if (LA(1) == '-' && LA(2) == '>' && tokenSet_12(LA(3))) {
            match("->");
            mID_ELEMENT(false);
        } else if (tokenSet_16(LA(1))) {
            mapped = true;
            OStr t = generator->mapTreeId(id->text, transInfo);
            if (t) {
                text.resize(begin);
                text += *t;
            }
            if (tokenSet_17(LA(1)) && tokenSet_16(LA(2)) && (transInfo && transInfo->refRuleRoot)) {
                if (isWS(LA(1)))
                    mWS(false);
                else if (LA(1) != '=')
                    throwNoViable();
                mVAR_ASSIGN(false);
            } else if (!tokenSet_18(LA(1))) {
                throwNoViable();
            }
        } else {
            throwNoViable();
        }
    }
    finishToken(createToken, token, ACT_ID_ELEMENT, begin);
    return mapped;
}

void ActionLexer::mAST_CTOR_ELEMENT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    if (LA(1) == '"' && inRange(LA(2)) && inRange(LA(3)))
        mSTRING(false);
    else if (tokenSet_19(LA(1)) && inRange(LA(2)))
        mTREE_ELEMENT(false);
    else if (isDigit(LA(1)))
        mINT(false);
    else
        throwNoViable();
    finishToken(createToken, token, ACT_AST_CTOR_ELEMENT, begin);
}

void ActionLexer::mINT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    int cnt = 0;
    for (;;) {
        if (isDigit(LA(1))) {
            mDIGIT(false);
        } else {
            if (cnt >= 1)
                break;
            throwNoViable();
        }
        ++cnt;
    }
    finishToken(createToken, token, ACT_INT, begin);
}

void ActionLexer::mARG(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    if (LA(1) == '\'')
        mCHAR(false);
    else if (isDigit(LA(1)))
        mINT_OR_FLOAT(false);
    else if (tokenSet_19(LA(1)) && inRange(LA(2)) && inRange(LA(3)))
        mTREE_ELEMENT(false);
    else if (LA(1) == '"' && inRange(LA(2)) && inRange(LA(3)))
        mSTRING(false);
    else
        throwNoViable();

    while (tokenSet_20(LA(1)) && tokenSet_21(LA(2)) && inRange(LA(3))) {
        if (isWS(LA(1)))
            mWS(false);
        else if (!(LA(1) == '*' || LA(1) == '+' || LA(1) == '-' || LA(1) == '/'))
            throwNoViable();
        if (LA(1) == '+' || LA(1) == '-' || LA(1) == '*' || LA(1) == '/')
            consume();
        else
            throwNoViable();
        if (isWS(LA(1)))
            mWS(false);
        else if (!isArgStart(LA(1)))
            throwNoViable();
        mARG(false);
    }
    finishToken(createToken, token, ACT_ARG, begin);
}

void ActionLexer::mTEXT_ARG_ELEMENT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    int c = LA(1);
    if (isIdStart(c))
        mTEXT_ARG_ID_ELEMENT(false);
    else if (c == '"')
        mSTRING(false);
    else if (c == '\'')
        mCHAR(false);
    else if (isDigit(c))
        mINT_OR_FLOAT(false);
    else if (c == '$')
        mTEXT_ITEM(false);
    else if (c == '+')
        match('+');
    else
        throwNoViable();
    finishToken(createToken, token, ACT_TEXT_ARG_ELEMENT, begin);
}

void ActionLexer::mTEXT_ARG_ID_ELEMENT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    mID(true);
    if (tokenSet_4(LA(1)) && tokenSet_22(LA(2)))
        strippedWS();
    else if (!tokenSet_22(LA(1)))
        throwNoViable();

    switch (LA(1)) {
    case '(': {
        match('(');
        if (tokenSet_4(LA(1)) && tokenSet_23(LA(2)) && inRange(LA(3)))
            strippedWS();
        else if (!(tokenSet_23(LA(1)) && inRange(LA(2))))
            throwNoViable();
        while (tokenSet_24(LA(1)) && inRange(LA(2)) && inRange(LA(3))) {
            mTEXT_ARG(false);
            while (LA(1) == ',') {
                match(',');
                mTEXT_ARG(false);
            }
        }
        if (isWS(LA(1)))
            strippedWS();
        else if (LA(1) != ')')
            throwNoViable();
        match(')');
        break;
    }
    case '[': {
        int cnt = 0;
        for (;;) {
            if (LA(1) == '[') {
                match('[');
                if (tokenSet_4(LA(1)) && tokenSet_24(LA(2)) && inRange(LA(3)))
                    strippedWS();
                else if (!(tokenSet_24(LA(1)) && inRange(LA(2)) && inRange(LA(3))))
                    throwNoViable();
                mTEXT_ARG(false);
                if (isWS(LA(1)))
                    strippedWS();
                else if (LA(1) != ']')
                    throwNoViable();
                match(']');
            } else {
                if (cnt >= 1)
                    break;
                throwNoViable();
            }
            ++cnt;
        }
        break;
    }
    case '.':
        match('.');
        mTEXT_ARG_ID_ELEMENT(false);
        break;
    case '-':
        match("->");
        mTEXT_ARG_ID_ELEMENT(false);
        break;
    default:
        if (LA(1) == ':' && LA(2) == ':' && tokenSet_12(LA(3))) {
            match("::");
            mTEXT_ARG_ID_ELEMENT(false);
        } else if (!tokenSet_11(LA(1))) {
            throwNoViable();
        }
    }
    finishToken(createToken, token, ACT_TEXT_ARG_ID_ELEMENT, begin);
}

void ActionLexer::mINT_OR_FLOAT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    int cnt = 0;
    for (;;) {
        if (isDigit(LA(1)) && tokenSet_25(LA(2))) {
            mDIGIT(false);
        } else {
            if (cnt >= 1)
                break;
            throwNoViable();
        }
        ++cnt;
    }
    if (LA(1) == 'L' && tokenSet_26(LA(2))) {
        match('L');
    } else if (LA(1) == 'l' && tokenSet_26(LA(2))) {
        match('l');
    } else if (LA(1) == '.') {
        match('.');
        while (isDigit(LA(1)) && tokenSet_26(LA(2)))
            mDIGIT(false);
    } else if (!tokenSet_26(LA(1))) {
        throwNoViable();
    }
    finishToken(createToken, token, ACT_INT_OR_FLOAT, begin);
}

void ActionLexer::mSL_COMMENT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    match("//");
    for (;;) {
        if (LA(1) == '\n' || LA(1) == '\r')
            break;
        if (inRange(LA(1)) && inRange(LA(2)))
            matchNot(EOF_CHAR);
        else
            break;
    }
    if (LA(1) == '\r' && LA(2) == '\n')
        match("\r\n");
    else if (LA(1) == '\n')
        match('\n');
    else if (LA(1) == '\r')
        match('\r');
    else
        throwNoViable();
    newline();
    finishToken(createToken, token, ACT_SL_COMMENT, begin);
}

void ActionLexer::mML_COMMENT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    match("/*");
    for (;;) {
        if (LA(1) == '*' && LA(2) == '/')
            break;
        if (LA(1) == '\r' && LA(2) == '\n' && inRange(LA(3))) {
            match('\r');
            match('\n');
            newline();
        } else if (LA(1) == '\r' && inRange(LA(2)) && inRange(LA(3))) {
            match('\r');
            newline();
        } else if (LA(1) == '\n' && inRange(LA(2)) && inRange(LA(3))) {
            match('\n');
            newline();
        } else if (inRange(LA(1)) && inRange(LA(2)) && inRange(LA(3))) {
            matchNot(EOF_CHAR);
        } else {
            break;
        }
    }
    match("*/");
    finishToken(createToken, token, ACT_ML_COMMENT, begin);
}

void ActionLexer::mESC(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    match('\\');
    switch (LA(1)) {
    case 'n': case 'r': case 't': case 'v': case 'b': case 'f': case '"': case '\'': case '\\':
        consume();
        break;
    case '0': case '1': case '2': case '3':
        matchRange('0', '3');
        if (isDigit(LA(1)) && inRange(LA(2))) {
            mDIGIT(false);
            if (isDigit(LA(1)) && inRange(LA(2)))
                mDIGIT(false);
            else if (!inRange(LA(1)))
                throwNoViable();
        } else if (!inRange(LA(1))) {
            throwNoViable();
        }
        break;
    case '4': case '5': case '6': case '7':
        matchRange('4', '7');
        if (isDigit(LA(1)) && inRange(LA(2)))
            mDIGIT(false);
        else if (!inRange(LA(1)))
            throwNoViable();
        break;
    default:
        throwNoViable();
    }
    finishToken(createToken, token, ACT_ESC, begin);
}

void ActionLexer::mDIGIT(bool createToken) {
    TokenRef token;
    std::size_t begin = text.size();
    matchRange('0', '9');
    finishToken(createToken, token, ACT_DIGIT, begin);
}

} // namespace antlrcpp
