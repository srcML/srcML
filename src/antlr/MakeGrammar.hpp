// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file MakeGrammar.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.DefineGrammarSymbols and antlr.MakeGrammar, the actions of the
 * grammar-file parser that build the grammar representation.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_MAKEGRAMMAR_HPP
#define ANTLR2CPP_MAKEGRAMMAR_HPP

#include "Grammar.hpp"
#include <vector>

namespace antlrcpp {

class LLkAnalyzer;

struct BlockContext {
    virtual ~BlockContext() = default;
    virtual void addAlternativeElement(AlternativeElement* e) { currentAlt()->addElement(e); }
    Alternative* currentAlt() const { return block->getAlternativeAt(altNum); }
    AlternativeElement* currentElement() const { return currentAlt()->tail; }

    AlternativeBlock* block = nullptr;
    int altNum = 0;
    BlockEndElement* blockEnd = nullptr;
};

struct TreeBlockContext : public BlockContext {
    void addAlternativeElement(AlternativeElement* e) override;
    bool nextElementIsRoot = true;
};

class MakeGrammar {
public:
    MakeGrammar(Tool* tool, std::vector<std::string> args, LLkAnalyzer* analyzer);

    // DefineGrammarSymbols
    void _refStringLiteral(const TokenRef& lit);
    void _refToken(const TokenRef& t);
    void abortGrammar();
    void defineToken(const TokenRef& tokname, const TokenRef& tokliteral);
    void endOptions();
    void refHeaderAction(const TokenRef& name, const TokenRef& act);
    void refPreambleAction(const TokenRef& act) { thePreambleAction = act; }
    void setCharVocabulary(const BitSet& b);
    void setFileOption(const TokenRef& key, const TokenRef& value, const std::string& filename);
    void setGrammarOption(const TokenRef& key, const TokenRef& value);
    void startLexer(const std::string& file, const TokenRef& name, const OStr& superClass, const OStr& doc);
    void startParser(const std::string& file, const TokenRef& name, const OStr& superClass, const OStr& doc);
    void startTreeWalker(const std::string& file, const TokenRef& name, const OStr& superClass, const OStr& doc);

    // MakeGrammar
    void beginAlt(bool doAutoGen);
    void beginChildList();
    void beginExceptionGroup();
    void beginExceptionSpec(const TokenRef& label);
    void beginSubRule(const TokenRef& label, const TokenRef& start, bool not_);
    void beginTree(const TokenRef& tok);
    BlockContext* context() const { return blocks.empty() ? nullptr : blocks.back(); }
    static RuleBlock* createNextTokenRule(Grammar* g, const std::vector<RuleSymbol*>& lexRules, const std::string& rname);
    void defineRuleName(const TokenRef& r, const std::string& access, bool ruleAutoGen, const OStr& docComment);
    void endAlt();
    void endChildList();
    void endExceptionGroup() {}
    void endExceptionSpec();
    void endGrammar();
    void endRule(const std::string& rule);
    void endSubRule();
    void endTree();
    void hasError() { grammarError = true; }
    void noASTSubRule() { context()->block->setAutoGen(false); }
    void oneOrMoreSubRule();
    void optionalSubRule();
    void refAction(const TokenRef& action);
    void setUserExceptions(const std::string& thr);
    void refArgAction(const TokenRef& action);
    void refCharLiteral(const TokenRef& lit, const TokenRef& label, bool inverted, int autoGenType, bool lastInRule);
    void refCharRange(const TokenRef& t1, const TokenRef& t2, const TokenRef& label, int autoGenType, bool lastInRule);
    void refTokensSpecElementOption(const TokenRef& tok, const TokenRef& option, const TokenRef& value);
    void refElementOption(const TokenRef& option, const TokenRef& value);
    void refExceptionHandler(const TokenRef& exTypeAndName, const TokenRef& action);
    void refInitAction(const TokenRef& action);
    void refMemberAction(const TokenRef& act);
    void refReturnAction(const TokenRef& returnAction);
    void refRule(const TokenRef& idAssign, const TokenRef& r, const TokenRef& label, const TokenRef& args, int autoGenType);
    void refSemPred(const TokenRef& pred);
    void refStringLiteral(const TokenRef& lit, const TokenRef& label, int autoGenType, bool lastInRule);
    void refToken(const TokenRef& idAssign, const TokenRef& t, const TokenRef& label, const TokenRef& args, bool inverted, int autoGenType, bool lastInRule);
    void refTokenRange(const TokenRef& t1, const TokenRef& t2, const TokenRef& label, int autoGenType, bool lastInRule);
    void refWildcard(const TokenRef& t, const TokenRef& label, int autoGenType);
    void reset();
    static void setBlock(AlternativeBlock* b, AlternativeBlock* src);
    void setRuleOption(const TokenRef& key, const TokenRef& value);
    void setSubruleOption(const TokenRef& key, const TokenRef& value);
    void synPred();
    void zeroOrMoreSubRule();

    // DefineGrammarSymbols state
    JavaHashtable<Grammar*> grammars;
    JavaHashtable<TokenManager*> tokenManagers;
    Grammar* grammar = nullptr;
    Tool* tool;
    LLkAnalyzer* analyzer;
    std::vector<std::string> args;
    JavaHashtable<TokenRef> headerActions;
    TokenRef thePreambleAction = makeTokenRef(TokenType::INVALID_TYPE, "");
    std::string language = "Java";
    int numLexers = 0;
    int numParsers = 0;
    int numTreeParsers = 0;

private:
    void addElementToCurrentAlt(AlternativeElement* e);
    AlternativeBlock* createOptionalRuleRef(const std::string& rule, const TokenRef& start);
    void labelElement(AlternativeElement* el, const TokenRef& label);

    // MakeGrammar state
    std::vector<BlockContext*> blocks;
    RuleRefElement* lastRuleRef = nullptr;
    RuleEndElement* ruleEnd = nullptr;
    RuleBlock* ruleBlock = nullptr;
    int nested = 0;
    bool grammarError = false;
    ExceptionSpec* currentExceptionSpec = nullptr;
};

} // namespace antlrcpp

#endif
