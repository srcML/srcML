// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file LLkAnalyzer.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.LLkAnalyzer and the warnings of antlr.DefaultToolErrorHandler.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "LLkAnalyzer.hpp"
#include "Tool.hpp"
#include <algorithm>

namespace antlrcpp {

namespace {

// JavaCharFormatter
std::string javaEscapeChar(int c, bool forCharLiteral) {
    switch (c) {
    case '\n': return "\\n";
    case '\t': return "\\t";
    case '\r': return "\\r";
    case '\\': return "\\\\";
    case '\'': return forCharLiteral ? "\\'" : "'";
    case '"': return forCharLiteral ? "\"" : "\\\"";
    default:
        if (c < ' ' || c > 126) {
            if (c <= 0x000F)
                return "\\u000" + toRadix(c, 16);
            if (c <= 0x00FF)
                return "\\u00" + toRadix(c, 16);
            if (c <= 0x0FFF)
                return "\\u0" + toRadix(c, 16);
            return "\\u" + toRadix(c, 16);
        }
        return std::string(1, static_cast<char>(c));
    }
}

std::string javaLiteralChar(int c) {
    return "'" + javaEscapeChar(c, true) + "'";
}

std::string toStringWithRanges(const BitSet& set) {
    std::string str;
    auto elems = set.toArray();
    std::size_t i = 0;
    while (i < elems.size()) {
        std::size_t lastInRange = 0;
        for (std::size_t j = i + 1; j < elems.size(); j++) {
            if (elems[j] != elems[j - 1] + 1)
                break;
            lastInRange = j;
        }
        if (!str.empty())
            str += ",";
        if (lastInRange >= i + 2) {
            str += javaLiteralChar(elems[i]) + ".." + javaLiteralChar(elems[lastInRange]);
            i = lastInRange;
        } else {
            str += javaLiteralChar(elems[i]);
        }
        i++;
    }
    return str;
}

std::string toStringWithVocabulary(const BitSet& set, const std::vector<OStr>& vocabulary) {
    std::string str;
    for (int i = 0; i < set.size(); i++) {
        if (!set.member(i))
            continue;
        if (!str.empty())
            str += ",";
        if (i >= static_cast<int>(vocabulary.size()))
            str += "<bad element " + std::to_string(i) + ">";
        else if (!vocabulary[static_cast<std::size_t>(i)])
            str += "<" + std::to_string(i) + ">";
        else
            str += *vocabulary[static_cast<std::size_t>(i)];
    }
    return str;
}

bool isBlockEnd(AlternativeElement* e) {
    return dynamic_cast<BlockEndElement*>(e) != nullptr;
}

} // namespace

std::vector<std::string> LLkAnalyzer::dumpSets(int depth, const std::vector<Lookahead>& sets) {
    std::vector<std::string> output;
    for (int i = 1; i <= depth; i++) {
        std::string line = "k==" + std::to_string(i) + ":";
        const Lookahead& set = sets[static_cast<std::size_t>(i)];
        if (lexicalAnalysis) {
            std::string bits = toStringWithRanges(set.fset);
            if (set.containsEpsilon()) {
                line += "<end-of-token>";
                if (!bits.empty())
                    line += ",";
            }
            line += bits;
        } else {
            line += toStringWithVocabulary(set.fset, grammar->tokenManager->getVocabulary());
        }
        output.push_back(line);
    }
    return output;
}

void LLkAnalyzer::warnAltAmbiguity(AlternativeBlock* blk, int depth, const std::vector<Lookahead>& sets, int altIdx1, int altIdx2) {
    std::string line;
    auto rb = dynamic_cast<RuleBlock*>(blk);
    if (rb && rb->isLexerAutoGenRule()) {
        auto rri = static_cast<RuleRefElement*>(blk->getAlternativeAt(altIdx1)->head);
        auto rrj = static_cast<RuleRefElement*>(blk->getAlternativeAt(altIdx2)->head);
        line = "lexical nondeterminism between rules " + decodeLexerRuleName(rri->targetRule) + " and " +
               decodeLexerRuleName(rrj->targetRule) + " upon";
    } else {
        if (lexicalAnalysis)
            line += "lexical ";
        line += "nondeterminism between alts " + std::to_string(altIdx1 + 1) + " and " + std::to_string(altIdx2 + 1) + " of block upon";
    }
    std::vector<std::string> output{ line };
    for (auto& s : dumpSets(depth, sets))
        output.push_back(s);
    tool->warning(output, grammar->getFilename(), blk->getLine(), blk->getColumn());
}

void LLkAnalyzer::warnAltExitAmbiguity(BlockWithImpliedExitPath* blk, int depth, const std::vector<Lookahead>& sets, int altIdx) {
    std::vector<std::string> output{ std::string(lexicalAnalysis ? "lexical " : "") + "nondeterminism upon" };
    for (auto& s : dumpSets(depth, sets))
        output.push_back(s);
    output.push_back("between alt " + std::to_string(altIdx + 1) + " and exit branch of block");
    tool->warning(output, grammar->getFilename(), blk->getLine(), blk->getColumn());
}

bool LLkAnalyzer::altUsesWildcardDefault(Alternative* alt) {
    AlternativeElement* head = alt->head;
    if (auto te = dynamic_cast<TreeElement*>(head))
        if (dynamic_cast<WildcardElement*>(te->root))
            return true;
    if (dynamic_cast<WildcardElement*>(head) && isBlockEnd(head->next))
        return true;
    return false;
}

bool LLkAnalyzer::deterministic(AlternativeBlock* blk) {
    int k = 1;
    bool det = true;
    int nalts = blk->numAlternatives();
    AlternativeBlock* saveCurrentBlock = currentBlock;
    currentBlock = blk;

    if (!blk->greedy && !dynamic_cast<OneOrMoreBlock*>(blk) && !dynamic_cast<ZeroOrMoreBlock*>(blk))
        tool->warning("Being nongreedy only makes sense for (...)+ and (...)*", grammar->getFilename(), blk->getLine(), blk->getColumn());

    if (nalts == 1) {
        AlternativeElement* e = blk->getAlternativeAt(0)->head;
        currentBlock->alti = 0;
        blk->getAlternativeAt(0)->cache[1] = e->look(1);
        blk->getAlternativeAt(0)->lookaheadDepth = 1;
        currentBlock = saveCurrentBlock;
        return true;
    }

    for (int i = 0; i < nalts - 1; i++) {
        currentBlock->alti = i;
        currentBlock->analysisAlt = i;
        currentBlock->altj = i + 1;
        for (int j = i + 1; j < nalts; j++) {
            currentBlock->altj = j;
            currentBlock->analysisAlt = j;
            k = 1;
            std::vector<Lookahead> r(static_cast<std::size_t>(grammar->maxk + 1));
            bool haveAmbiguity;
            do {
                haveAmbiguity = false;
                Lookahead p = getAltLookahead(blk, i, k);
                Lookahead q = getAltLookahead(blk, j, k);
                r[static_cast<std::size_t>(k)] = p.intersection(q);
                if (!r[static_cast<std::size_t>(k)].nil()) {
                    haveAmbiguity = true;
                    k++;
                }
            } while (haveAmbiguity && k <= grammar->maxk);

            Alternative* ai = blk->getAlternativeAt(i);
            Alternative* aj = blk->getAlternativeAt(j);
            if (haveAmbiguity) {
                det = false;
                ai->lookaheadDepth = NONDETERMINISTIC;
                aj->lookaheadDepth = NONDETERMINISTIC;

                if (ai->synPred) {
                } else if (ai->semPred) {
                } else if (altUsesWildcardDefault(aj)) {
                } else if (!blk->warnWhenFollowAmbig && (isBlockEnd(ai->head) || isBlockEnd(aj->head))) {
                } else if (!blk->generateAmbigWarnings) {
                } else if (blk->greedySet && blk->greedy &&
                           ((isBlockEnd(ai->head) && !isBlockEnd(aj->head)) || (isBlockEnd(aj->head) && !isBlockEnd(ai->head)))) {
                } else {
                    warnAltAmbiguity(blk, grammar->maxk, r, i, j);
                }
            } else {
                ai->lookaheadDepth = std::max(ai->lookaheadDepth, k);
                aj->lookaheadDepth = std::max(aj->lookaheadDepth, k);
            }
        }
    }

    currentBlock = saveCurrentBlock;
    return det;
}

bool LLkAnalyzer::deterministic(OneOrMoreBlock* blk) {
    AlternativeBlock* saveCurrentBlock = currentBlock;
    currentBlock = blk;
    bool blkOk = deterministic(static_cast<AlternativeBlock*>(blk));
    bool det = deterministicImpliedPath(blk);
    currentBlock = saveCurrentBlock;
    return det && blkOk;
}

bool LLkAnalyzer::deterministic(ZeroOrMoreBlock* blk) {
    AlternativeBlock* saveCurrentBlock = currentBlock;
    currentBlock = blk;
    bool blkOk = deterministic(static_cast<AlternativeBlock*>(blk));
    bool det = deterministicImpliedPath(blk);
    currentBlock = saveCurrentBlock;
    return det && blkOk;
}

bool LLkAnalyzer::deterministicImpliedPath(BlockWithImpliedExitPath* blk) {
    int k;
    bool det = true;
    int nalts = blk->numAlternatives();
    currentBlock->altj = -1;

    for (int i = 0; i < nalts; i++) {
        Alternative* alt = blk->getAlternativeAt(i);
        if (isBlockEnd(alt->head))
            tool->warning("empty alternative makes no sense in (...)* or (...)+", grammar->getFilename(), blk->getLine(), blk->getColumn());

        k = 1;
        std::vector<Lookahead> r(static_cast<std::size_t>(grammar->maxk + 1));
        bool haveAmbiguity;
        do {
            haveAmbiguity = false;
            Lookahead follow = blk->next->look(k);
            blk->exitCache[static_cast<std::size_t>(k)] = follow;
            currentBlock->alti = i;
            Lookahead p = getAltLookahead(blk, i, k);
            r[static_cast<std::size_t>(k)] = follow.intersection(p);
            if (!r[static_cast<std::size_t>(k)].nil()) {
                haveAmbiguity = true;
                k++;
            }
        } while (haveAmbiguity && k <= grammar->maxk);

        if (haveAmbiguity) {
            det = false;
            alt->lookaheadDepth = NONDETERMINISTIC;
            blk->exitLookaheadDepth = NONDETERMINISTIC;
            Alternative* ambigAlt = blk->getAlternativeAt(currentBlock->alti);

            if (!blk->warnWhenFollowAmbig) {
            } else if (!blk->generateAmbigWarnings) {
            } else if (blk->greedy && blk->greedySet && !isBlockEnd(ambigAlt->head)) {
            } else if (!blk->greedy && !isBlockEnd(ambigAlt->head)) {
                if (!lookaheadEquivForApproxAndFullAnalysis(blk->exitCache, grammar->maxk)) {
                    tool->warning(std::vector<std::string>{ "nongreedy block may exit incorrectly due",
                                                            "\tto limitations of linear approximate lookahead (first k-1 sets",
                                                            "\tin lookahead not singleton)." },
                                  grammar->getFilename(), blk->getLine(), blk->getColumn());
                }
            } else {
                warnAltExitAmbiguity(blk, grammar->maxk, r, i);
            }
        } else {
            alt->lookaheadDepth = std::max(alt->lookaheadDepth, k);
            blk->exitLookaheadDepth = std::max(blk->exitLookaheadDepth, k);
        }
    }
    return det;
}

Lookahead LLkAnalyzer::FOLLOW(int k, RuleEndElement* end) {
    auto rb = static_cast<RuleBlock*>(end->block);
    std::string rule = lexicalAnalysis ? encodeLexerRuleName(rb->getRuleName()) : rb->getRuleName();
    auto K = static_cast<std::size_t>(k);

    if (end->lock[K])
        return Lookahead(rule);

    if (end->cache[K]) {
        if (!end->cache[K]->cycle)
            return *end->cache[K];
        RuleSymbol* rs = grammar->getSymbol(*end->cache[K]->cycle);
        RuleEndElement* re = rs->getBlock()->endNode;
        if (!re->cache[K]) {
            return *end->cache[K];
        } else {
            if (!re->cache[K]->cycle) {
                end->cache[K]->combineWith(*re->cache[K]);
                end->cache[K]->cycle.reset();
            } else {
                Lookahead refFOLLOW = FOLLOW(k, re);
                end->cache[K]->combineWith(refFOLLOW);
                end->cache[K]->cycle = refFOLLOW.cycle;
            }
            return *end->cache[K];
        }
    }

    end->lock[K] = true;

    Lookahead p;
    RuleSymbol* rs = grammar->getSymbol(rule);
    for (int i = 0; i < rs->numReferences(); i++) {
        RuleRefElement* rr = rs->references[static_cast<std::size_t>(i)];
        Lookahead q = rr->next->look(k);
        if (q.cycle && *q.cycle == rule)
            q.cycle.reset();
        p.combineWith(q);
    }

    end->lock[K] = false;

    if (p.fset.nil() && !p.cycle) {
        if (grammar->isTreeWalker())
            p.fset.add(TokenType::NULL_TREE_LOOKAHEAD);
        else if (grammar->isLexer())
            p.setEpsilon();
        else
            p.fset.add(TokenType::EOF_TYPE);
    }

    end->cache[K] = p;
    return p;
}

Lookahead LLkAnalyzer::getAltLookahead(AlternativeBlock* blk, int alt, int k) {
    Alternative* a = blk->getAlternativeAt(alt);
    AlternativeElement* e = a->head;
    auto K = static_cast<std::size_t>(k);
    if (!a->cache[K])
        a->cache[K] = e->look(k);
    return *a->cache[K];
}

Lookahead LLkAnalyzer::look(int k, ActionElement* action) {
    return action->next->look(k);
}

Lookahead LLkAnalyzer::look(int k, AlternativeBlock* blk) {
    AlternativeBlock* saveCurrentBlock = currentBlock;
    currentBlock = blk;
    Lookahead p;
    for (int i = 0; i < blk->numAlternatives(); i++) {
        currentBlock->analysisAlt = i;
        Alternative* alt = blk->getAlternativeAt(i);
        AlternativeElement* elem = alt->head;
        Lookahead q = elem->look(k);
        p.combineWith(q);
    }
    if (k == 1 && blk->not_ && subruleCanBeInverted(blk, lexicalAnalysis)) {
        if (lexicalAnalysis) {
            BitSet b = static_cast<LexerGrammar*>(grammar)->charVocabulary;
            for (int elem : p.fset.toArray())
                b.remove(elem);
            p.fset = b;
        } else {
            p.fset.notInPlace(TokenType::MIN_USER_TYPE, grammar->tokenManager->maxTokenType());
        }
    }
    currentBlock = saveCurrentBlock;
    return p;
}

Lookahead LLkAnalyzer::look(int k, BlockEndElement* end) {
    auto K = static_cast<std::size_t>(k);
    if (end->lock[K])
        return Lookahead();

    Lookahead p;
    if (dynamic_cast<ZeroOrMoreBlock*>(end->block) || dynamic_cast<OneOrMoreBlock*>(end->block)) {
        end->lock[K] = true;
        p = look(k, end->block);
        end->lock[K] = false;
    }

    if (dynamic_cast<TreeElement*>(end->block)) {
        p.combineWith(Lookahead::of(TokenType::NULL_TREE_LOOKAHEAD));
    } else if (dynamic_cast<SynPredBlock*>(end->block)) {
        p.setEpsilon();
    } else {
        Lookahead q = end->block->next->look(k);
        p.combineWith(q);
    }
    return p;
}

Lookahead LLkAnalyzer::look(int k, CharLiteralElement* atom) {
    if (k > 1)
        return atom->next->look(k - 1);
    if (lexicalAnalysis) {
        if (atom->not_) {
            BitSet b = static_cast<LexerGrammar*>(grammar)->charVocabulary;
            removeCompetingPredictionSets(b, atom);
            b.clear(atom->getType());
            return Lookahead(b);
        }
        return Lookahead::of(atom->getType());
    }
    tool->panic("Character literal reference found in parser");
}

Lookahead LLkAnalyzer::look(int k, CharRangeElement* r) {
    if (k > 1)
        return r->next->look(k - 1);
    BitSet p = BitSet::of(r->begin);
    for (int i = r->begin + 1; i <= r->end; i++)
        p.add(i);
    return Lookahead(p);
}

Lookahead LLkAnalyzer::look(int k, GrammarAtom* atom) {
    if (lexicalAnalysis)
        tool->panic("token reference found in lexer");
    if (k > 1)
        return atom->next->look(k - 1);
    Lookahead l = Lookahead::of(atom->getType());
    if (atom->not_) {
        int maxToken = grammar->tokenManager->maxTokenType();
        l.fset.notInPlace(TokenType::MIN_USER_TYPE, maxToken);
        removeCompetingPredictionSets(l.fset, atom);
    }
    return l;
}

Lookahead LLkAnalyzer::look(int k, OneOrMoreBlock* blk) {
    return look(k, static_cast<AlternativeBlock*>(blk));
}

Lookahead LLkAnalyzer::look(int k, RuleBlock* blk) {
    return look(k, static_cast<AlternativeBlock*>(blk));
}

Lookahead LLkAnalyzer::look(int k, RuleEndElement* end) {
    if (end->noFOLLOW) {
        Lookahead p;
        p.setEpsilon();
        p.epsilonDepth = BitSet::of(k);
        return p;
    }
    return FOLLOW(k, end);
}

Lookahead LLkAnalyzer::look(int k, RuleRefElement* rr) {
    RuleSymbol* rs = grammar->getSymbol(rr->targetRule);
    if (!rs || !rs->defined) {
        tool->error("no definition of rule " + rr->targetRule, grammar->getFilename(), rr->getLine(), rr->getColumn());
        return Lookahead();
    }
    RuleBlock* rb = rs->getBlock();
    RuleEndElement* end = rb->endNode;
    bool saveEnd = end->noFOLLOW;
    end->noFOLLOW = true;
    Lookahead p = look(k, rr->targetRule);
    end->noFOLLOW = saveEnd;

    if (p.cycle)
        tool->error("infinite recursion to rule " + *p.cycle + " from rule " + rr->enclosingRuleName, grammar->getFilename(), rr->getLine(), rr->getColumn());

    if (p.containsEpsilon()) {
        p.resetEpsilon();
        auto depths = p.epsilonDepth->toArray();
        p.epsilonDepth.reset();
        for (int depth : depths) {
            int rk = k - (k - depth);
            Lookahead q = rr->next->look(rk);
            p.combineWith(q);
        }
    }
    return p;
}

Lookahead LLkAnalyzer::look(int k, StringLiteralElement* atom) {
    if (lexicalAnalysis) {
        int len = static_cast<int>(atom->processedAtomText.size());
        if (k > len)
            return atom->next->look(k - len);
        return Lookahead::of(static_cast<unsigned char>(atom->processedAtomText[static_cast<std::size_t>(k - 1)]));
    }
    if (k > 1)
        return atom->next->look(k - 1);
    Lookahead l = Lookahead::of(atom->getType());
    if (atom->not_) {
        int maxToken = grammar->tokenManager->maxTokenType();
        l.fset.notInPlace(TokenType::MIN_USER_TYPE, maxToken);
    }
    return l;
}

Lookahead LLkAnalyzer::look(int k, SynPredBlock* blk) {
    return blk->next->look(k);
}

Lookahead LLkAnalyzer::look(int k, TokenRangeElement* r) {
    if (k > 1)
        return r->next->look(k - 1);
    BitSet p = BitSet::of(r->begin);
    for (int i = r->begin + 1; i <= r->end; i++)
        p.add(i);
    return Lookahead(p);
}

Lookahead LLkAnalyzer::look(int k, TreeElement* t) {
    if (k > 1)
        return t->next->look(k - 1);
    Lookahead l;
    if (dynamic_cast<WildcardElement*>(t->root)) {
        l = t->root->look(1);
    } else {
        l = Lookahead::of(t->root->getType());
        if (t->root->not_) {
            int maxToken = grammar->tokenManager->maxTokenType();
            l.fset.notInPlace(TokenType::MIN_USER_TYPE, maxToken);
        }
    }
    return l;
}

Lookahead LLkAnalyzer::look(int k, WildcardElement* wc) {
    if (k > 1)
        return wc->next->look(k - 1);
    BitSet b;
    if (lexicalAnalysis) {
        b = static_cast<LexerGrammar*>(grammar)->charVocabulary;
    } else {
        b = BitSet(1);
        int maxToken = grammar->tokenManager->maxTokenType();
        b.notInPlace(TokenType::MIN_USER_TYPE, maxToken);
    }
    return Lookahead(b);
}

Lookahead LLkAnalyzer::look(int k, ZeroOrMoreBlock* blk) {
    Lookahead p = look(k, static_cast<AlternativeBlock*>(blk));
    Lookahead q = blk->next->look(k);
    p.combineWith(q);
    return p;
}

Lookahead LLkAnalyzer::look(int k, const std::string& rule) {
    RuleSymbol* rs = grammar->getSymbol(rule);
    RuleBlock* rb = rs->getBlock();
    auto K = static_cast<std::size_t>(k);

    if (rb->lock[K])
        return Lookahead(rule);

    if (rb->cache[K])
        return *rb->cache[K];

    rb->lock[K] = true;
    Lookahead p = look(k, rb);
    rb->lock[K] = false;

    rb->cache[K] = p;
    return p;
}

bool LLkAnalyzer::lookaheadEquivForApproxAndFullAnalysis(const std::vector<std::optional<Lookahead>>& bset, int k) {
    for (int i = 1; i <= k - 1; i++)
        if (bset[static_cast<std::size_t>(i)]->fset.degree() > 1)
            return false;
    return true;
}

void LLkAnalyzer::removeCompetingPredictionSets(BitSet& b, AlternativeElement* el) {
    AlternativeElement* head = currentBlock->getAlternativeAt(currentBlock->analysisAlt)->head;
    if (auto te = dynamic_cast<TreeElement*>(head)) {
        if (te->root != el)
            return;
    } else if (el != head) {
        return;
    }
    for (int i = 0; i < currentBlock->analysisAlt; i++) {
        AlternativeElement* e = currentBlock->getAlternativeAt(i)->head;
        b.subtractInPlace(e->look(1).fset);
    }
}

void LLkAnalyzer::reset() {
    grammar = nullptr;
    currentBlock = nullptr;
    lexicalAnalysis = false;
}

void LLkAnalyzer::setGrammar(Grammar* g) {
    if (grammar)
        reset();
    grammar = g;
    lexicalAnalysis = grammar->isLexer();
}

bool LLkAnalyzer::subruleCanBeInverted(AlternativeBlock* blk, bool forLexer) {
    if (dynamic_cast<ZeroOrMoreBlock*>(blk) || dynamic_cast<OneOrMoreBlock*>(blk) || dynamic_cast<SynPredBlock*>(blk))
        return false;
    if (blk->numAlternatives() == 0)
        return false;
    for (int i = 0; i < blk->numAlternatives(); i++) {
        Alternative* alt = blk->getAlternativeAt(i);
        if (alt->synPred || alt->semPred || alt->exceptionSpec)
            return false;
        AlternativeElement* elt = alt->head;
        bool simple = dynamic_cast<CharLiteralElement*>(elt) || dynamic_cast<TokenRefElement*>(elt) ||
                      dynamic_cast<CharRangeElement*>(elt) || dynamic_cast<TokenRangeElement*>(elt) ||
                      (dynamic_cast<StringLiteralElement*>(elt) && !forLexer);
        if (!simple || !isBlockEnd(elt->next) || elt->getAutoGenType() != AUTO_GEN_NONE)
            return false;
    }
    return true;
}

} // namespace antlrcpp
