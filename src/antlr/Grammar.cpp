// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Grammar.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of the ANTLR 2.7.7 grammar representation.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "Grammar.hpp"
#include "LLkAnalyzer.hpp"
#include "CppCodeGenerator.hpp"
#include "Tool.hpp"
#include "Tokdef.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace antlrcpp {

int AlternativeBlock::nblks = 0;

std::string encodeLexerRuleName(const std::string& id) {
    return "m" + id;
}

std::string decodeLexerRuleName(const std::string& id) {
    return id.substr(1);
}

// ANTLRLexer.escapeCharValue()
int escapeCharValue(const std::string& cs) {
    if (cs.size() < 3 || cs[1] != '\\')
        return 0;
    switch (cs[2]) {
    case 'b': return '\b';
    case 'r': return '\r';
    case 't': return '\t';
    case 'n': return '\n';
    case 'f': return '\f';
    case '"': return '"';
    case '\'': return '\'';
    case '\\': return '\\';
    case 'u':
        if (cs.size() != 8)
            return 0;
        return hexDigit(cs[3]) * 16 * 16 * 16 + hexDigit(cs[4]) * 16 * 16 + hexDigit(cs[5]) * 16 + hexDigit(cs[6]);
    case '0':
    case '1':
    case '2':
    case '3':
        if (cs.size() > 5 && isJavaDigit(cs[4]))
            return (cs[2] - '0') * 8 * 8 + (cs[3] - '0') * 8 + (cs[4] - '0');
        if (cs.size() > 4 && isJavaDigit(cs[3]))
            return (cs[2] - '0') * 8 + (cs[3] - '0');
        return cs[2] - '0';
    case '4':
    case '5':
    case '6':
    case '7':
        if (cs.size() > 4 && isJavaDigit(cs[3]))
            return (cs[2] - '0') * 8 + (cs[3] - '0');
        return cs[2] - '0';
    default:
        return 0;
    }
}

int tokenTypeForCharLiteral(const std::string& lit) {
    if (lit.size() > 3)
        return escapeCharValue(lit);
    return static_cast<unsigned char>(lit[1]);
}

// AlternativeBlock

AlternativeBlock::AlternativeBlock(Grammar* g) : AlternativeElement(g) {
    nblks++;
    ID = nblks;
}

AlternativeBlock::AlternativeBlock(Grammar* g, const TokenRef& start, bool not_) : AlternativeElement(g, start), not_(not_) {
    nblks++;
    ID = nblks;
}

void AlternativeBlock::generate() {
    grammar->generator->gen(this);
}

Lookahead AlternativeBlock::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

void AlternativeBlock::prepareForAnalysis() {
    for (auto a : *alternatives) {
        a->cache.assign(static_cast<std::size_t>(grammar->maxk + 1), std::nullopt);
        a->lookaheadDepth = LOOKAHEAD_DEPTH_INIT;
    }
}

void AlternativeBlock::removeTrackingOfRuleRefs(Grammar* g) {
    for (auto alt : *alternatives) {
        for (AlternativeElement* elem = alt->head; elem; elem = elem->next) {
            if (auto rr = dynamic_cast<RuleRefElement*>(elem)) {
                RuleSymbol* rs = g->getSymbol(rr->targetRule);
                if (!rs) {
                    grammar->antlrTool->error("rule " + rr->targetRule + " referenced in (...)=>, but not defined");
                } else {
                    auto& refs = rs->references;
                    for (auto it = refs.begin(); it != refs.end(); ++it) {
                        if (*it == rr) {
                            refs.erase(it);
                            break;
                        }
                    }
                }
            } else if (auto ab = dynamic_cast<AlternativeBlock*>(elem)) {
                ab->removeTrackingOfRuleRefs(g);
            }
        }
    }
}

void AlternativeBlock::setOption(const TokenRef& key, const TokenRef& value) {
    const std::string& v = value->text;
    if (key->text == "warnWhenFollowAmbig") {
        if (v == "true")
            warnWhenFollowAmbig = true;
        else if (v == "false")
            warnWhenFollowAmbig = false;
        else
            grammar->antlrTool->error("Value for warnWhenFollowAmbig must be true or false", grammar->getFilename(), key->line, key->column);
    } else if (key->text == "generateAmbigWarnings") {
        if (v == "true")
            generateAmbigWarnings = true;
        else if (v == "false")
            generateAmbigWarnings = false;
        else
            grammar->antlrTool->error("Value for generateAmbigWarnings must be true or false", grammar->getFilename(), key->line, key->column);
    } else if (key->text == "greedy") {
        if (v == "true") {
            greedy = true;
            greedySet = true;
        } else if (v == "false") {
            greedy = false;
            greedySet = true;
        } else {
            grammar->antlrTool->error("Value for greedy must be true or false", grammar->getFilename(), key->line, key->column);
        }
    } else {
        grammar->antlrTool->error("Invalid subrule option: " + key->text, grammar->getFilename(), key->line, key->column);
    }
}

// BlockEndElement, RuleEndElement

BlockEndElement::BlockEndElement(Grammar* g) : AlternativeElement(g), lock(static_cast<std::size_t>(g->maxk + 1), false) {}

Lookahead BlockEndElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

RuleEndElement::RuleEndElement(Grammar* g) : BlockEndElement(g), cache(static_cast<std::size_t>(g->maxk + 1)) {}

Lookahead RuleEndElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

// RuleBlock

RuleBlock::RuleBlock(Grammar* g, const std::string& r) : AlternativeBlock(g), ruleName(r), cache(static_cast<std::size_t>(g->maxk + 1)) {
    setAutoGen(g->isParser());
}

RuleBlock::RuleBlock(Grammar* g, const std::string& r, int line, bool doAutoGen) : RuleBlock(g, r) {
    this->line = line;
    setAutoGen(doAutoGen);
}

void RuleBlock::addExceptionSpec(ExceptionSpec* ex) {
    if (findExceptionSpec(ex->label)) {
        if (ex->label)
            grammar->antlrTool->error("Rule '" + ruleName + "' already has an exception handler for label: " + ex->label->text);
        else
            grammar->antlrTool->error("Rule '" + ruleName + "' already has an exception handler");
    } else {
        exceptionSpecs.put(ex->label ? ex->label->text : std::string(), ex);
    }
}

ExceptionSpec* RuleBlock::findExceptionSpec(const std::string& label) const {
    auto p = exceptionSpecs.get(label);
    return p ? *p : nullptr;
}

void RuleBlock::generate() {
    grammar->generator->gen(static_cast<AlternativeBlock*>(this));
}

Lookahead RuleBlock::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

void RuleBlock::prepareForAnalysis() {
    AlternativeBlock::prepareForAnalysis();
    lock.assign(static_cast<std::size_t>(grammar->maxk + 1), false);
}

void RuleBlock::setOption(const TokenRef& key, const TokenRef& value) {
    Tool* tool = grammar->antlrTool;
    const std::string& v = value->text;
    if (key->text == "defaultErrorHandler") {
        if (v == "true")
            defaultErrorHandler = true;
        else if (v == "false")
            defaultErrorHandler = false;
        else
            tool->error("Value for defaultErrorHandler must be true or false", grammar->getFilename(), key->line, key->column);
    } else if (key->text == "testLiterals") {
        if (!grammar->isLexer()) {
            tool->error("testLiterals option only valid for lexer rules", grammar->getFilename(), key->line, key->column);
        } else {
            if (v == "true")
                testLiterals = true;
            else if (v == "false")
                testLiterals = false;
            else
                tool->error("Value for testLiterals must be true or false", grammar->getFilename(), key->line, key->column);
        }
    } else if (key->text == "ignore") {
        if (!grammar->isLexer())
            tool->error("ignore option only valid for lexer rules", grammar->getFilename(), key->line, key->column);
        else
            ignoreRule = v;
    } else if (key->text == "paraphrase") {
        if (!grammar->isLexer()) {
            tool->error("paraphrase option only valid for lexer rules", grammar->getFilename(), key->line, key->column);
        } else {
            TokenSymbol* ts = grammar->tokenManager->getTokenSymbol(ruleName);
            if (!ts)
                tool->panic("cannot find token associated with rule " + ruleName);
            ts->paraphrase = v;
        }
    } else if (key->text == "generateAmbigWarnings") {
        if (v == "true")
            generateAmbigWarnings = true;
        else if (v == "false")
            generateAmbigWarnings = false;
        else
            tool->error("Value for generateAmbigWarnings must be true or false", grammar->getFilename(), key->line, key->column);
    } else {
        tool->error("Invalid rule option: " + key->text, grammar->getFilename(), key->line, key->column);
    }
}

// Loop blocks

BlockWithImpliedExitPath::BlockWithImpliedExitPath(Grammar* g) : AlternativeBlock(g), exitCache(static_cast<std::size_t>(g->maxk + 1)) {}

void ZeroOrMoreBlock::generate() {
    grammar->generator->gen(this);
}

Lookahead ZeroOrMoreBlock::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

void OneOrMoreBlock::generate() {
    grammar->generator->gen(this);
}

Lookahead OneOrMoreBlock::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

void SynPredBlock::generate() {
    grammar->generator->gen(static_cast<AlternativeBlock*>(this));
}

Lookahead SynPredBlock::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

// Atoms

void GrammarAtom::setOption(const TokenRef& option, const TokenRef& value) {
    if (option->text == "AST")
        setASTNodeType(value->text);
    else
        grammar->antlrTool->error("Invalid element option:" + option->text, grammar->getFilename(), option->line, option->column);
}

void ActionElement::generate() {
    grammar->generator->gen(this);
}

Lookahead ActionElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

CharLiteralElement::CharLiteralElement(LexerGrammar* g, const TokenRef& t, bool inverted, int autoGenType)
    : GrammarAtom(g, t, AUTO_GEN_NONE) {
    tokenType = tokenTypeForCharLiteral(t->text);
    g->charVocabulary.add(tokenType);
    line = t->line;
    not_ = inverted;
    this->autoGenType = autoGenType;
}

void CharLiteralElement::generate() {
    grammar->generator->gen(this);
}

Lookahead CharLiteralElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

CharRangeElement::CharRangeElement(LexerGrammar* g, const TokenRef& t1, const TokenRef& t2, int autoGenType) : AlternativeElement(g) {
    // Java stores these as char
    begin = tokenTypeForCharLiteral(t1->text) & 0xFFFF;
    beginText = t1->text;
    end = tokenTypeForCharLiteral(t2->text) & 0xFFFF;
    endText = t2->text;
    line = t1->line;
    for (int i = begin; i <= end; i++)
        g->charVocabulary.add(i);
    this->autoGenType = autoGenType;
}

void CharRangeElement::generate() {
    grammar->generator->gen(this);
}

Lookahead CharRangeElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

RuleRefElement::RuleRefElement(Grammar* g, const TokenRef& t, int autoGenType) : AlternativeElement(g, t, autoGenType), targetRule(t->text) {
    if (t->type == ANTLR_TOKEN_REF)
        targetRule = encodeLexerRuleName(targetRule);
}

void RuleRefElement::generate() {
    grammar->generator->gen(this);
}

Lookahead RuleRefElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

StringLiteralElement::StringLiteralElement(Grammar* g, const TokenRef& t, int autoGenType) : GrammarAtom(g, t, autoGenType) {
    if (!g->isLexer()) {
        TokenSymbol* ts = grammar->tokenManager->getTokenSymbol(atomText);
        if (!ts)
            g->antlrTool->error("Undefined literal: " + atomText, grammar->getFilename(), t->line, t->column);
        else
            tokenType = ts->getTokenType();
    }
    line = t->line;
    for (int i = 1; i < static_cast<int>(atomText.size()) - 1; i++) {
        char c = atomText[static_cast<std::size_t>(i)];
        if (c == '\\') {
            if (i + 1 < static_cast<int>(atomText.size()) - 1) {
                i++;
                c = atomText[static_cast<std::size_t>(i)];
                switch (c) {
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case 't': c = '\t'; break;
                }
            }
        }
        if (g->isLexer())
            static_cast<LexerGrammar*>(g)->charVocabulary.add(static_cast<unsigned char>(c));
        processedAtomText += c;
    }
}

void StringLiteralElement::generate() {
    grammar->generator->gen(this);
}

Lookahead StringLiteralElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

TokenRefElement::TokenRefElement(Grammar* g, const TokenRef& t, bool inverted, int autoGenType) : GrammarAtom(g, t, autoGenType) {
    not_ = inverted;
    TokenSymbol* ts = grammar->tokenManager->getTokenSymbol(atomText);
    if (!ts) {
        g->antlrTool->error("Undefined token symbol: " + atomText, grammar->getFilename(), t->line, t->column);
    } else {
        tokenType = ts->getTokenType();
        setASTNodeType(ts->ASTNodeType);
    }
    line = t->line;
}

void TokenRefElement::generate() {
    grammar->generator->gen(this);
}

Lookahead TokenRefElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, static_cast<GrammarAtom*>(this));
}

TokenRangeElement::TokenRangeElement(Grammar* g, const TokenRef& t1, const TokenRef& t2, int autoGenType) : AlternativeElement(g, t1, autoGenType) {
    begin = grammar->tokenManager->getTokenSymbol(t1->text)->getTokenType();
    beginText = t1->text;
    end = grammar->tokenManager->getTokenSymbol(t2->text)->getTokenType();
    endText = t2->text;
    line = t1->line;
}

void TokenRangeElement::generate() {
    grammar->generator->gen(this);
}

Lookahead TokenRangeElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

void WildcardElement::generate() {
    grammar->generator->gen(this);
}

Lookahead WildcardElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

void TreeElement::generate() {
    grammar->generator->gen(this);
}

Lookahead TreeElement::look(int k) {
    return grammar->theLLkAnalyzer->look(k, this);
}

// TokenManager

TokenManager::TokenManager(const std::string& name, Tool* tool) : antlrTool(tool), name(name) {
    auto ts = new TokenSymbol("EOF");
    ts->setTokenType(TokenType::EOF_TYPE);
    define(ts);
    setVocabularyAt(TokenType::NULL_TREE_LOOKAHEAD, "NULL_TREE_LOOKAHEAD");
}

void TokenManager::setVocabularyAt(int index, const std::string& s) {
    if (index >= static_cast<int>(vocabulary.size()))
        vocabulary.resize(static_cast<std::size_t>(index + 1));
    vocabulary[static_cast<std::size_t>(index)] = s;
}

void TokenManager::define(TokenSymbol* ts) {
    setVocabularyAt(ts->getTokenType(), ts->getId());
    mapToTokenSymbol(ts->getId(), ts);
}

// ImportVocabTokenManager.define(String, int)
void TokenManager::define(const std::string& s, int ttype) {
    TokenSymbol* ts = startsWith(s, "\"") ? new StringLiteralSymbol(s) : new TokenSymbol(s);
    ts->setTokenType(ttype);
    define(ts);
    maxToken = (ttype + 1) > maxToken ? (ttype + 1) : maxToken;
}

void TokenManager::loadVocabFile(const std::string& filename) {
    importVocab = true;
    std::string path = filename;
    if (!std::filesystem::exists(path)) {
        path = antlrTool->getOutputDirectory() + "/" + filename;
        if (!std::filesystem::exists(path))
            antlrTool->panic("Cannot find importVocab file '" + filename + "'");
    }
    setReadOnly(true);
    std::ifstream in(path, std::ios::binary);
    if (!in)
        antlrTool->panic("Cannot find importVocab file '" + filename + "'");
    std::stringstream buf;
    buf << in.rdbuf();
    try {
        readTokdef(buf.str(), filename, this, antlrTool);
    } catch (const RecognitionException& ex) {
        antlrTool->panic("Error parsing importVocab file '" + filename + "': " + ex.message);
    }
}

// Grammar

Grammar::Grammar(const std::string& className, Tool* tool, const OStr& superClass)
    : antlrTool(tool), superClass(superClass), className(className) {}

int Grammar::getIntegerOption(const std::string& key) const {
    TokenRef t = getOption(key);
    if (!t || t->type != ANTLR_INT)
        throw std::invalid_argument(key);
    return std::stoi(t->text);
}

bool Grammar::setOption(const std::string& key, const TokenRef& value) {
    options.put(key, value);
    const std::string& s = value->text;
    if (key == "k") {
        try {
            maxk = getIntegerOption("k");
            if (maxk <= 0) {
                antlrTool->error("option 'k' must be greater than 0 (was " + value->text + ")", getFilename(), value->line, value->column);
                maxk = 1;
            }
        } catch (const std::exception&) {
            antlrTool->error("option 'k' must be an integer (was " + value->text + ")", getFilename(), value->line, value->column);
        }
        return true;
    }
    if (key == "codeGenMakeSwitchThreshold") {
        try {
            getIntegerOption("codeGenMakeSwitchThreshold");
        } catch (const std::exception&) {
            antlrTool->error("option 'codeGenMakeSwitchThreshold' must be an integer", getFilename(), value->line, value->column);
        }
        return true;
    }
    if (key == "codeGenBitsetTestThreshold") {
        try {
            getIntegerOption("codeGenBitsetTestThreshold");
        } catch (const std::exception&) {
            antlrTool->error("option 'codeGenBitsetTestThreshold' must be an integer", getFilename(), value->line, value->column);
        }
        return true;
    }
    if (key == "defaultErrorHandler") {
        if (s == "true")
            defaultErrorHandler = true;
        else if (s == "false")
            defaultErrorHandler = false;
        else
            antlrTool->error("Value for defaultErrorHandler must be true or false", getFilename(), value->line, value->column);
        return true;
    }
    if (key == "analyzerDebug" || key == "codeGenDebug") {
        if (s == "true")
            analyzerDebug = true;
        else if (s == "false")
            analyzerDebug = false;
        else
            antlrTool->error("option '" + key + "' must be true or false", getFilename(), value->line, value->column);
        return true;
    }
    if (key == "classHeaderSuffix" || key == "classHeaderPrefix" || key == "namespaceAntlr" || key == "namespaceStd" ||
        key == "genHashLines" || key == "noConstructors")
        return true;
    return false;
}

// LexerGrammar

LexerGrammar::LexerGrammar(const std::string& className, Tool* tool, const OStr& superClass) : Grammar(className, tool, superClass) {
    BitSet cv;
    for (int i = 0; i <= 127; i++)
        cv.add(i);
    setCharVocabulary(cv);
    defaultErrorHandler = false;
}

void LexerGrammar::generate() {
    generator->gen(this);
}

std::string LexerGrammar::getSuperClass() const {
    if (debuggingOutput)
        return "debug.DebuggingCharScanner";
    return "CharScanner";
}

void LexerGrammar::processArguments(const std::vector<std::string>& args) {
    for (std::size_t i = 0; i < args.size(); i++) {
        if (args[i] == "-trace" || args[i] == "-traceLexer") {
            traceRules = true;
            antlrTool->setArgOK(static_cast<int>(i));
        } else if (args[i] == "-debug") {
            debuggingOutput = true;
            antlrTool->setArgOK(static_cast<int>(i));
        }
    }
}

bool LexerGrammar::setOption(const std::string& key, const TokenRef& value) {
    const std::string& s = value->text;
    auto boolOption = [&](bool& field, const std::string& message, bool isError) {
        if (s == "true")
            field = true;
        else if (s == "false")
            field = false;
        else if (isError)
            antlrTool->error(message, getFilename(), value->line, value->column);
        else
            antlrTool->warning(message, getFilename(), value->line, value->column);
        return true;
    };
    if (key == "buildAST") {
        antlrTool->warning("buildAST option is not valid for lexer", getFilename(), value->line, value->column);
        return true;
    }
    if (key == "testLiterals")
        return boolOption(testLiterals, "testLiterals option must be true or false", false);
    if (key == "interactive")
        return boolOption(interactive, "interactive option must be true or false", true);
    if (key == "caseSensitive")
        return boolOption(caseSensitive, "caseSensitive option must be true or false", false);
    if (key == "caseSensitiveLiterals")
        return boolOption(caseSensitiveLiterals, "caseSensitiveLiterals option must be true or false", false);
    if (key == "filter") {
        if (s == "true") {
            filterMode = true;
        } else if (s == "false") {
            filterMode = false;
        } else if (value->type == ANTLR_TOKEN_REF) {
            filterMode = true;
            filterRule = s;
        } else {
            antlrTool->warning("filter option must be true, false, or a lexer rule name", getFilename(), value->line, value->column);
        }
        return true;
    }
    if (key == "longestPossible") {
        antlrTool->warning("longestPossible option has been deprecated; ignoring it...", getFilename(), value->line, value->column);
        return true;
    }
    if (key == "className") {
        Grammar::setOption(key, value);
        return true;
    }
    if (Grammar::setOption(key, value))
        return true;
    antlrTool->error("Invalid option: " + key, getFilename(), value->line, value->column);
    return false;
}

// ParserGrammar

void ParserGrammar::generate() {
    generator->gen(this);
}

std::string ParserGrammar::getSuperClass() const {
    if (debuggingOutput)
        return "debug.LLkDebuggingParser";
    return "LLkParser";
}

void ParserGrammar::processArguments(const std::vector<std::string>& args) {
    for (std::size_t i = 0; i < args.size(); i++) {
        if (args[i] == "-trace" || args[i] == "-traceParser") {
            traceRules = true;
            antlrTool->setArgOK(static_cast<int>(i));
        } else if (args[i] == "-debug") {
            debuggingOutput = true;
            antlrTool->setArgOK(static_cast<int>(i));
        }
    }
}

bool ParserGrammar::setOption(const std::string& key, const TokenRef& value) {
    const std::string& s = value->text;
    if (key == "buildAST") {
        if (s == "true")
            buildAST = true;
        else if (s == "false")
            buildAST = false;
        else
            antlrTool->error("buildAST option must be true or false", getFilename(), value->line, value->column);
        return true;
    }
    if (key == "interactive") {
        if (s == "true")
            interactive = true;
        else if (s == "false")
            interactive = false;
        else
            antlrTool->error("interactive option must be true or false", getFilename(), value->line, value->column);
        return true;
    }
    if (key == "ASTLabelType" || key == "className") {
        Grammar::setOption(key, value);
        return true;
    }
    if (Grammar::setOption(key, value))
        return true;
    antlrTool->error("Invalid option: " + key, getFilename(), value->line, value->column);
    return false;
}

// TreeWalkerGrammar

void TreeWalkerGrammar::generate() {
    generator->gen(this);
}

void TreeWalkerGrammar::processArguments(const std::vector<std::string>& args) {
    for (std::size_t i = 0; i < args.size(); i++) {
        if (args[i] == "-trace" || args[i] == "-traceTreeParser") {
            traceRules = true;
            antlrTool->setArgOK(static_cast<int>(i));
        }
    }
}

bool TreeWalkerGrammar::setOption(const std::string& key, const TokenRef& value) {
    if (key == "buildAST") {
        if (value->text == "true")
            buildAST = true;
        else if (value->text == "false")
            buildAST = false;
        else
            antlrTool->error("buildAST option must be true or false", getFilename(), value->line, value->column);
        return true;
    }
    if (key == "ASTLabelType" || key == "className") {
        Grammar::setOption(key, value);
        return true;
    }
    if (Grammar::setOption(key, value))
        return true;
    antlrTool->error("Invalid option: " + key, getFilename(), value->line, value->column);
    return false;
}

} // namespace antlrcpp
