// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file LLkAnalyzer.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.LLkAnalyzer, the linear approximate LL(k) lookahead analysis.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_LLKANALYZER_HPP
#define ANTLR2CPP_LLKANALYZER_HPP

#include "Grammar.hpp"

namespace antlrcpp {

class LLkAnalyzer {
public:
    explicit LLkAnalyzer(Tool* tool) : tool(tool) {}

    bool deterministic(AlternativeBlock* blk);
    bool deterministic(OneOrMoreBlock* blk);
    bool deterministic(ZeroOrMoreBlock* blk);
    bool deterministicImpliedPath(BlockWithImpliedExitPath* blk);

    Lookahead FOLLOW(int k, RuleEndElement* end);

    Lookahead look(int k, ActionElement* action);
    Lookahead look(int k, AlternativeBlock* blk);
    Lookahead look(int k, BlockEndElement* end);
    Lookahead look(int k, CharLiteralElement* atom);
    Lookahead look(int k, CharRangeElement* r);
    Lookahead look(int k, GrammarAtom* atom);
    Lookahead look(int k, OneOrMoreBlock* blk);
    Lookahead look(int k, RuleBlock* blk);
    Lookahead look(int k, RuleEndElement* end);
    Lookahead look(int k, RuleRefElement* rr);
    Lookahead look(int k, StringLiteralElement* atom);
    Lookahead look(int k, SynPredBlock* blk);
    Lookahead look(int k, TokenRangeElement* r);
    Lookahead look(int k, TreeElement* t);
    Lookahead look(int k, WildcardElement* wc);
    Lookahead look(int k, ZeroOrMoreBlock* blk);
    Lookahead look(int k, const std::string& rule);

    static bool lookaheadEquivForApproxAndFullAnalysis(const std::vector<std::optional<Lookahead>>& bset, int k);

    void setGrammar(Grammar* g);
    bool subruleCanBeInverted(AlternativeBlock* blk, bool forLexer);

private:
    bool altUsesWildcardDefault(Alternative* alt);
    Lookahead getAltLookahead(AlternativeBlock* blk, int alt, int k);
    void removeCompetingPredictionSets(BitSet& b, AlternativeElement* el);
    void reset();

    // warnings issued by DefaultToolErrorHandler
    void warnAltAmbiguity(AlternativeBlock* blk, int depth, const std::vector<Lookahead>& sets, int altIdx1, int altIdx2);
    void warnAltExitAmbiguity(BlockWithImpliedExitPath* blk, int depth, const std::vector<Lookahead>& sets, int altIdx);
    std::vector<std::string> dumpSets(int depth, const std::vector<Lookahead>& sets);

    AlternativeBlock* currentBlock = nullptr;
    Tool* tool;
    Grammar* grammar = nullptr;
    bool lexicalAnalysis = false;
};

} // namespace antlrcpp

#endif
