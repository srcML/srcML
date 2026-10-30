// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Grammar.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of the ANTLR 2.7.7 grammar representation: grammar elements, blocks,
 * symbols, token managers, and the grammar classes themselves.
 * Objects are never freed, as in the garbage-collected original.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_GRAMMAR_HPP
#define ANTLR2CPP_GRAMMAR_HPP

#include "Util.hpp"
#include "BitSet.hpp"
#include "JavaHashtable.hpp"
#include "Scanner.hpp"
#include "ANTLRTokenTypes.hpp"
#include <string>
#include <vector>
#include <optional>
#include <climits>

namespace antlrcpp {

class Tool;
class Grammar;
class LexerGrammar;
class LLkAnalyzer;
class CppCodeGenerator;

constexpr int NONDETERMINISTIC = INT_MAX;
constexpr int LOOKAHEAD_DEPTH_INIT = -1;

// Lookahead set
struct Lookahead {
    BitSet fset;
    OStr cycle;
    std::optional<BitSet> epsilonDepth;
    bool hasEpsilon = false;

    Lookahead() = default;
    explicit Lookahead(BitSet p) : fset(std::move(p)) {}
    explicit Lookahead(const std::string& c) : cycle(c) {}

    static Lookahead of(int el) {
        Lookahead look;
        look.fset.add(el);
        return look;
    }

    void combineWith(const Lookahead& q) {
        if (!cycle)
            cycle = q.cycle;
        if (q.containsEpsilon())
            hasEpsilon = true;
        if (epsilonDepth) {
            if (q.epsilonDepth)
                epsilonDepth->orInPlace(*q.epsilonDepth);
        } else if (q.epsilonDepth) {
            epsilonDepth = *q.epsilonDepth;
        }
        fset.orInPlace(q.fset);
    }

    bool containsEpsilon() const { return hasEpsilon; }

    Lookahead intersection(const Lookahead& q) const {
        Lookahead p(fset.and_(q.fset));
        if (hasEpsilon && q.hasEpsilon)
            p.setEpsilon();
        return p;
    }

    bool nil() const { return fset.nil() && !hasEpsilon; }
    void resetEpsilon() { hasEpsilon = false; }
    void setEpsilon() { hasEpsilon = true; }
};

constexpr int AUTO_GEN_NONE = 1;
constexpr int AUTO_GEN_CARET = 2;
constexpr int AUTO_GEN_BANG = 3;

class GrammarElement {
public:
    explicit GrammarElement(Grammar* g) : grammar(g) {}
    GrammarElement(Grammar* g, const TokenRef& start) : grammar(g), line(start->line), column(start->column) {}
    virtual ~GrammarElement() = default;

    virtual void generate() {}
    virtual Lookahead look(int) { return Lookahead(); }
    int getLine() const { return line; }
    int getColumn() const { return column; }

    Grammar* grammar;
    int line = -1;
    int column = -1;
};

class AlternativeElement : public GrammarElement {
public:
    using GrammarElement::GrammarElement;
    AlternativeElement(Grammar* g, const TokenRef& start, int autoGenType) : GrammarElement(g, start), autoGenType(autoGenType) {}

    int getAutoGenType() const { return autoGenType; }
    void setAutoGenType(int a) { autoGenType = a; }
    virtual OStr getLabel() const { return std::nullopt; }
    virtual void setLabel(const std::string&) {}

    AlternativeElement* next = nullptr;
    int autoGenType = AUTO_GEN_NONE;
    std::string enclosingRuleName;
};

class Alternative;
class RuleBlock;
class SynPredBlock;

class ExceptionHandler {
public:
    ExceptionHandler(TokenRef typeAndName, TokenRef action) : exceptionTypeAndName(std::move(typeAndName)), action(std::move(action)) {}
    TokenRef exceptionTypeAndName;
    TokenRef action;
};

class ExceptionSpec {
public:
    explicit ExceptionSpec(TokenRef label) : label(std::move(label)) {}
    void addHandler(ExceptionHandler* h) { handlers.push_back(h); }
    TokenRef label;
    std::vector<ExceptionHandler*> handlers;
};

class Alternative {
public:
    Alternative() = default;
    explicit Alternative(AlternativeElement* firstElement) { addElement(firstElement); }

    void addElement(AlternativeElement* e) {
        if (!head) {
            head = tail = e;
        } else {
            tail->next = e;
            tail = e;
        }
    }
    bool atStart() const { return head == nullptr; }
    bool getAutoGen() const { return doAutoGen && !treeSpecifier; }
    void setAutoGen(bool b) { doAutoGen = b; }

    AlternativeElement* head = nullptr;
    AlternativeElement* tail = nullptr;
    SynPredBlock* synPred = nullptr;
    OStr semPred;
    ExceptionSpec* exceptionSpec = nullptr;
    std::vector<std::optional<Lookahead>> cache;
    int lookaheadDepth = 0;
    TokenRef treeSpecifier;
    bool doAutoGen = false;
};

class AlternativeBlock : public AlternativeElement {
public:
    explicit AlternativeBlock(Grammar* g);
    AlternativeBlock(Grammar* g, const TokenRef& start, bool not_);

    void addAlternative(Alternative* alt) { alternatives->push_back(alt); }
    void generate() override;
    Alternative* getAlternativeAt(int i) const { return (*alternatives)[static_cast<std::size_t>(i)]; }
    std::vector<Alternative*>& getAlternatives() { return *alternatives; }
    int numAlternatives() const { return static_cast<int>(alternatives->size()); }
    bool getAutoGen() const { return doAutoGen; }
    OStr getLabel() const override { return label; }
    Lookahead look(int k) override;
    virtual void prepareForAnalysis();
    void removeTrackingOfRuleRefs(Grammar* g);
    void setAutoGen(bool b) { doAutoGen = b; }
    void setInitAction(const std::string& a) { initAction = a; }
    void setLabel(const std::string& l) override { label = l; }
    virtual void setOption(const TokenRef& key, const TokenRef& value);

    OStr initAction;
    // shared, as the original shares the Vector between blocks with setBlock()
    std::shared_ptr<std::vector<Alternative*>> alternatives = std::make_shared<std::vector<Alternative*>>();
    OStr label;
    int alti = 0;
    int altj = 0;
    int analysisAlt = 0;
    bool hasAnAction = false;
    bool hasASynPred = false;
    int ID = 0;
    static int nblks;
    bool not_ = false;
    bool greedy = true;
    bool greedySet = false;
    bool doAutoGen = true;
    bool warnWhenFollowAmbig = true;
    bool generateAmbigWarnings = true;
};

class BlockEndElement : public AlternativeElement {
public:
    explicit BlockEndElement(Grammar* g);
    Lookahead look(int k) override;
    std::vector<bool> lock;
    AlternativeBlock* block = nullptr;
};

class RuleEndElement : public BlockEndElement {
public:
    explicit RuleEndElement(Grammar* g);
    Lookahead look(int k) override;
    std::vector<std::optional<Lookahead>> cache;
    bool noFOLLOW = false;
};

class RuleBlock : public AlternativeBlock {
public:
    RuleBlock(Grammar* g, const std::string& r);
    RuleBlock(Grammar* g, const std::string& r, int line, bool doAutoGen);

    void addExceptionSpec(ExceptionSpec* ex);
    ExceptionSpec* findExceptionSpec(const std::string& label) const;
    ExceptionSpec* findExceptionSpec(const TokenRef& label) const { return findExceptionSpec(label ? label->text : std::string()); }
    void generate() override;
    bool getDefaultErrorHandler() const { return defaultErrorHandler; }
    RuleEndElement* getEndElement() const { return endNode; }
    OStr getIgnoreRule() const { return ignoreRule; }
    const std::string& getRuleName() const { return ruleName; }
    bool getTestLiterals() const { return testLiterals; }
    bool isLexerAutoGenRule() const { return ruleName == "nextToken"; }
    Lookahead look(int k) override;
    void prepareForAnalysis() override;
    void setDefaultErrorHandler(bool value) { defaultErrorHandler = value; }
    void setEndElement(RuleEndElement* re) { endNode = re; }
    void setOption(const TokenRef& key, const TokenRef& value) override;

    std::string ruleName;
    OStr argAction;
    OStr throwsSpec;
    OStr returnAction;
    RuleEndElement* endNode = nullptr;
    bool testLiterals = false;
    std::vector<AlternativeElement*> labeledElements;
    std::vector<bool> lock;
    std::vector<std::optional<Lookahead>> cache;
    JavaHashtable<ExceptionSpec*> exceptionSpecs;
    bool defaultErrorHandler = true;
    OStr ignoreRule;
};

class BlockWithImpliedExitPath : public AlternativeBlock {
public:
    explicit BlockWithImpliedExitPath(Grammar* g);
    int exitLookaheadDepth = 0;
    std::vector<std::optional<Lookahead>> exitCache;
};

class ZeroOrMoreBlock : public BlockWithImpliedExitPath {
public:
    using BlockWithImpliedExitPath::BlockWithImpliedExitPath;
    void generate() override;
    Lookahead look(int k) override;
};

class OneOrMoreBlock : public BlockWithImpliedExitPath {
public:
    using BlockWithImpliedExitPath::BlockWithImpliedExitPath;
    void generate() override;
    Lookahead look(int k) override;
};

class SynPredBlock : public AlternativeBlock {
public:
    explicit SynPredBlock(Grammar* g) : AlternativeBlock(g) {}
    void generate() override;
    Lookahead look(int k) override;
};

class GrammarAtom : public AlternativeElement {
public:
    GrammarAtom(Grammar* g, const TokenRef& t, int autoGenType) : AlternativeElement(g, t, autoGenType), atomText(t->text) {}

    OStr getLabel() const override { return label; }
    void setLabel(const std::string& l) override { label = l; }
    const std::string& getText() const { return atomText; }
    int getType() const { return tokenType; }
    OStr getASTNodeType() const { return ASTNodeType; }
    void setASTNodeType(const OStr& type) { ASTNodeType = type; }
    void setOption(const TokenRef& option, const TokenRef& value);

    OStr label;
    std::string atomText;
    int tokenType = TokenType::INVALID_TYPE;
    bool not_ = false;
    OStr ASTNodeType;
};

class ActionElement : public AlternativeElement {
public:
    ActionElement(Grammar* g, const TokenRef& t) : AlternativeElement(g), actionText(t->text) {
        line = t->line;
        column = t->column;
    }
    void generate() override;
    Lookahead look(int k) override;

    std::string actionText;
    bool isSemPred = false;
};

class CharLiteralElement : public GrammarAtom {
public:
    CharLiteralElement(LexerGrammar* g, const TokenRef& t, bool inverted, int autoGenType);
    void generate() override;
    Lookahead look(int k) override;
};

class CharRangeElement : public AlternativeElement {
public:
    CharRangeElement(LexerGrammar* g, const TokenRef& t1, const TokenRef& t2, int autoGenType);
    void generate() override;
    OStr getLabel() const override { return label; }
    void setLabel(const std::string& l) override { label = l; }
    Lookahead look(int k) override;

    OStr label;
    int begin = 0;
    int end = 0;
    std::string beginText;
    std::string endText;
};

class RuleRefElement : public AlternativeElement {
public:
    RuleRefElement(Grammar* g, const TokenRef& t, int autoGenType);
    void generate() override;
    OStr getLabel() const override { return label; }
    void setLabel(const std::string& l) override { label = l; }
    Lookahead look(int k) override;

    std::string targetRule;
    OStr args;
    OStr idAssign;
    OStr label;
};

class StringLiteralElement : public GrammarAtom {
public:
    StringLiteralElement(Grammar* g, const TokenRef& t, int autoGenType);
    void generate() override;
    Lookahead look(int k) override;

    std::string processedAtomText;
};

class TokenRefElement : public GrammarAtom {
public:
    TokenRefElement(Grammar* g, const TokenRef& t, bool inverted, int autoGenType);
    void generate() override;
    Lookahead look(int k) override;
};

class TokenRangeElement : public AlternativeElement {
public:
    TokenRangeElement(Grammar* g, const TokenRef& t1, const TokenRef& t2, int autoGenType);
    void generate() override;
    OStr getLabel() const override { return label; }
    void setLabel(const std::string& l) override { label = l; }
    Lookahead look(int k) override;

    OStr label;
    int begin = TokenType::INVALID_TYPE;
    int end = TokenType::INVALID_TYPE;
    std::string beginText;
    std::string endText;
};

class WildcardElement : public GrammarAtom {
public:
    WildcardElement(Grammar* g, const TokenRef& t, int autoGenType) : GrammarAtom(g, t, autoGenType) { line = t->line; }
    void generate() override;
    Lookahead look(int k) override;
};

class TreeElement : public AlternativeBlock {
public:
    TreeElement(Grammar* g, const TokenRef& start) : AlternativeBlock(g, start, false) {}
    void generate() override;
    Lookahead look(int k) override;

    GrammarAtom* root = nullptr;
};

// Symbols

class GrammarSymbol {
public:
    explicit GrammarSymbol(std::string id) : id(std::move(id)) {}
    virtual ~GrammarSymbol() = default;
    const std::string& getId() const { return id; }
    std::string id;
};

class RuleSymbol : public GrammarSymbol {
public:
    using GrammarSymbol::GrammarSymbol;
    RuleBlock* getBlock() const { return block; }
    bool isDefined() const { return defined; }
    void setDefined() { defined = true; }
    void setBlock(RuleBlock* rb) { block = rb; }
    void addReference(RuleRefElement* e) { references.push_back(e); }
    int numReferences() const { return static_cast<int>(references.size()); }

    RuleBlock* block = nullptr;
    bool defined = false;
    std::vector<RuleRefElement*> references;
    std::string access;
    OStr comment;
};

class TokenSymbol : public GrammarSymbol {
public:
    using GrammarSymbol::GrammarSymbol;
    int getTokenType() const { return ttype; }
    void setTokenType(int t) { ttype = t; }

    int ttype = TokenType::INVALID_TYPE;
    OStr paraphrase;
    OStr ASTNodeType;
};

class StringLiteralSymbol : public TokenSymbol {
public:
    using TokenSymbol::TokenSymbol;
    OStr label;
};

// Token managers (SimpleTokenManager and ImportVocabTokenManager)
class TokenManager {
public:
    TokenManager(const std::string& name, Tool* tool);
    TokenManager(const TokenManager&) = default;

    TokenManager* clone() const { return new TokenManager(*this); }
    void define(TokenSymbol* ts);
    void define(const std::string& s, int ttype);
    const std::string& getName() const { return name; }
    OStr getTokenStringAt(int idx) const {
        if (idx < 0 || idx >= static_cast<int>(vocabulary.size()))
            return std::nullopt;
        return vocabulary[static_cast<std::size_t>(idx)];
    }
    TokenSymbol* getTokenSymbol(const std::string& sym) const {
        auto p = table.get(sym);
        return p ? *p : nullptr;
    }
    TokenSymbol* getTokenSymbolAt(int idx) const {
        auto s = getTokenStringAt(idx);
        return s ? getTokenSymbol(*s) : nullptr;
    }
    std::vector<std::string> getTokenSymbolKeys() const { return table.keys(); }
    std::vector<OStr>& getVocabulary() { return vocabulary; }
    int vocabularySize() const { return static_cast<int>(vocabulary.size()); }
    bool isReadOnly() const { return importVocab ? readOnly : false; }
    void mapToTokenSymbol(const std::string& n, TokenSymbol* sym) { table.put(n, sym); }
    int maxTokenType() const { return maxToken - 1; }
    int nextTokenType() { return maxToken++; }
    void setName(const std::string& n) { name = n; }
    void setReadOnly(bool ro) { readOnly = ro; }
    bool tokenDefined(const std::string& symbol) const { return table.containsKey(symbol); }

    void loadVocabFile(const std::string& filename);

    int maxToken = TokenType::MIN_USER_TYPE;
    std::vector<OStr> vocabulary;
    JavaHashtable<TokenSymbol*> table;
    Tool* antlrTool;
    std::string name;
    bool readOnly = false;
    bool importVocab = false;

private:
    void setVocabularyAt(int index, const std::string& s);
};

// Grammars

class Grammar {
public:
    Grammar(const std::string& className, Tool* tool, const OStr& superClass);
    virtual ~Grammar() = default;

    void define(RuleSymbol* rs) {
        rules.push_back(rs);
        symbols.put(rs->getId(), rs);
    }
    virtual void generate() = 0;
    const std::string& getClassName() const { return className; }
    bool getDefaultErrorHandler() const { return defaultErrorHandler; }
    const std::string& getFilename() const { return fileName; }
    // throws NumberFormatException as std::invalid_argument
    int getIntegerOption(const std::string& key) const;
    TokenRef getOption(const std::string& key) const {
        auto p = options.get(key);
        return p ? *p : nullptr;
    }
    virtual std::string getSuperClass() const = 0;
    RuleSymbol* getSymbol(const std::string& s) const {
        auto p = symbols.get(s);
        return p ? *p : nullptr;
    }
    bool hasOption(const std::string& key) const { return options.containsKey(key); }
    bool isDefined(const std::string& s) const { return symbols.containsKey(s); }
    virtual void processArguments(const std::vector<std::string>& args) = 0;
    void setFilename(const std::string& s) { fileName = s; }
    virtual bool setOption(const std::string& key, const TokenRef& value);

    virtual bool isLexer() const { return false; }
    virtual bool isParser() const { return false; }
    virtual bool isTreeWalker() const { return false; }

    Tool* antlrTool;
    CppCodeGenerator* generator = nullptr;
    LLkAnalyzer* theLLkAnalyzer = nullptr;
    JavaHashtable<RuleSymbol*> symbols;
    bool buildAST = false;
    bool analyzerDebug = false;
    bool interactive = false;
    OStr superClass;
    TokenManager* tokenManager = nullptr;
    OStr exportVocab;
    OStr importVocab;
    JavaHashtable<TokenRef> options;
    std::vector<RuleSymbol*> rules;
    TokenRef preambleAction = makeTokenRef(TokenType::INVALID_TYPE, "");
    std::string className;
    std::string fileName;
    TokenRef classMemberAction = makeTokenRef(TokenType::INVALID_TYPE, "");
    bool hasSyntacticPredicate = false;
    bool hasUserErrorHandling = false;
    int maxk = 1;
    bool traceRules = false;
    bool debuggingOutput = false;
    bool defaultErrorHandler = true;
    OStr comment;
};

class LexerGrammar : public Grammar {
public:
    LexerGrammar(const std::string& className, Tool* tool, const OStr& superClass);
    void generate() override;
    std::string getSuperClass() const override;
    bool getTestLiterals() const { return testLiterals; }
    void processArguments(const std::vector<std::string>& args) override;
    void setCharVocabulary(const BitSet& b) { charVocabulary = b; }
    bool setOption(const std::string& key, const TokenRef& value) override;
    bool isLexer() const override { return true; }

    BitSet charVocabulary;
    bool testLiterals = true;
    bool caseSensitiveLiterals = true;
    bool caseSensitive = true;
    bool filterMode = false;
    OStr filterRule;
};

class ParserGrammar : public Grammar {
public:
    using Grammar::Grammar;
    void generate() override;
    std::string getSuperClass() const override;
    void processArguments(const std::vector<std::string>& args) override;
    bool setOption(const std::string& key, const TokenRef& value) override;
    bool isParser() const override { return true; }
};

class TreeWalkerGrammar : public Grammar {
public:
    using Grammar::Grammar;
    void generate() override;
    std::string getSuperClass() const override { return "TreeParser"; }
    void processArguments(const std::vector<std::string>& args) override;
    bool setOption(const std::string& key, const TokenRef& value) override;
    bool isTreeWalker() const override { return true; }

    bool transform = false;
};

// ANTLRLexer static helpers
int escapeCharValue(const std::string& cs);
int tokenTypeForCharLiteral(const std::string& lit);

std::string encodeLexerRuleName(const std::string& id);
std::string decodeLexerRuleName(const std::string& id);

} // namespace antlrcpp

#endif
