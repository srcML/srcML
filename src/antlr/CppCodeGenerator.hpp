// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file CppCodeGenerator.hpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.CodeGenerator and antlr.CppCodeGenerator.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#ifndef ANTLR2CPP_CPPCODEGENERATOR_HPP
#define ANTLR2CPP_CPPCODEGENERATOR_HPP

#include "Grammar.hpp"
#include "Tool.hpp"
#include <map>
#include <set>
#include <sstream>

namespace antlrcpp {

class MakeGrammar;

struct ActionTransInfo {
    bool assignToRoot = false;
    OStr refRuleRoot;
    OStr followSetName;
};

struct CppBlockFinishingInfo {
    OStr postscript;
    bool generatedSwitch = false;
    bool generatedAnIf = false;
    bool needAnErrorClause = true;
};

class CppCodeGenerator {
public:
    CppCodeGenerator();

    void setBehavior(MakeGrammar* b) { behavior = b; }
    void setAnalyzer(LLkAnalyzer* a) { analyzer = a; }
    void setTool(Tool* t) { antlrTool = t; }

    void gen();
    void gen(ActionElement* action);
    void gen(AlternativeBlock* blk);
    void gen(CharLiteralElement* atom);
    void gen(CharRangeElement* r);
    void gen(LexerGrammar* g);
    void gen(OneOrMoreBlock* blk);
    void gen(ParserGrammar* g);
    void gen(RuleRefElement* rr);
    void gen(StringLiteralElement* atom);
    void gen(TokenRangeElement* r);
    void gen(TokenRefElement* atom);
    void gen(TreeElement* t);
    void gen(TreeWalkerGrammar* g);
    void gen(WildcardElement* wc);
    void gen(ZeroOrMoreBlock* blk);

    // used by the action translator
    OStr mapTreeId(const std::string& id, ActionTransInfo* tInfo);
    std::string processStringForASTConstructor(const std::string& str);
    std::string getASTCreateString(const std::vector<std::string>& v);
    std::string getASTCreateString(GrammarAtom* atom, const std::string& str);
    std::string getASTCreateString(const std::string& str);
    OStr getFOLLOWBitSet(const std::string& ruleName, int k);
    OStr getFIRSTBitSet(const std::string& ruleName, int k);

private:
    // CodeGenerator
    void _print(const OStr& s);
    void _printAction(const OStr& s);
    void _println(const OStr& s);
    void print(const OStr& s);
    void printAction(const OStr& s);
    void println(const OStr& s);
    void printTabs();
    static bool elementsAreRange(const std::vector<int>& elems);
    std::string extractIdOfAction(const std::string& s, int line, int column);
    std::string extractTypeOfAction(const std::string& s, int line, int column);
    void genTokenInterchange(TokenManager* tm);
    std::string getBitsetName(int index) const { return "_tokenSet_" + std::to_string(index); }
    int markBitsetForGen(const BitSet& p);
    std::string removeAssignmentFromDeclaration(std::string d);
    void reset();
    void setGrammar(Grammar* g);

    // CppCodeGenerator
    void exitIfError();
    static int countLines(const std::string& s);
    void printAction(const TokenRef& t);
    void printHeaderAction(const std::string& name);
    void genLineNo(int line);
    void genLineNo(GrammarElement* el);
    void genLineNo(const TokenRef& t);
    void genLineNo2();
    std::string convertJavaToCppString(const std::string& lit, bool isCharLiteral);
    void genAlt(Alternative* alt, AlternativeBlock* blk);
    void genBitsets(std::vector<BitSet>& bitsetList, int maxVocabulary, const std::string& prefix);
    void genBitsetsHeader(std::vector<BitSet>& bitsetList, int maxVocabulary);
    void genBlockFinish(const CppBlockFinishingInfo& howToFinish, const std::string& noViableAction);
    void genBlockInitAction(AlternativeBlock* blk);
    void genBlockPreamble(AlternativeBlock* blk);
    void genBody(LexerGrammar* g);
    void genInitFactory(Grammar* g);
    void genBody(ParserGrammar* g);
    void genBody(TreeWalkerGrammar* g);
    void genCases(const BitSet& p);
    CppBlockFinishingInfo genCommonBlock(AlternativeBlock* blk, bool noTestForSingle);
    static bool suitableForCaseExpression(Alternative* a);
    void genElementAST(AlternativeElement* el);
    void genErrorCatchForElement(AlternativeElement* el);
    void genErrorHandler(ExceptionSpec* ex);
    void genErrorTryForElement(AlternativeElement* el);
    void genHeader(const std::string& fileName);
    void genInclude(LexerGrammar* g);
    void genInclude(ParserGrammar* g);
    void genInclude(TreeWalkerGrammar* g);
    void genASTDeclaration(AlternativeElement* el);
    void genASTDeclaration(AlternativeElement* el, const std::string& nodeType);
    void genASTDeclaration(AlternativeElement* el, const std::string& varName, const std::string& nodeType);
    void genLiteralsTest();
    void genLiteralsTestForPartialToken();
    void genMatch(GrammarAtom* atom);
    void genMatchUsingAtomText(GrammarAtom* atom);
    void genMatchUsingAtomTokenType(GrammarAtom* atom);
    void genNextToken();
    void genRule(RuleSymbol* s, bool startSymbol, int ruleNum, const std::string& prefix);
    void genRuleHeader(RuleSymbol* s, bool startSymbol);
    void genRuleWrapper(RuleSymbol* s, const std::string& prefix);
    static std::vector<std::string> splitParameters(const std::string& argAction);
    static std::string parameterName(const std::string& param);
    std::string implName(const std::string& rule) const;
    std::string failReturn() const;
    std::string noViable() const;
    void GenRuleInvocation(RuleRefElement* rr);
    void genSemPred(std::string pred, int line);
    void genSynPred(SynPredBlock* blk, const std::string& lookaheadExpr);
    void genTokenStrings(const std::string& prefix);
    void genTokenTypes(TokenManager* tm);
    std::string getLookaheadTestExpression(const std::vector<std::optional<Lookahead>>& look, int k);
    std::string getLookaheadTestExpression(Alternative* alt, int maxDepth);
    std::string getLookaheadTestTerm(int k, const BitSet& p);
    std::string getRangeExpression(int k, const std::vector<int>& elems);
    std::string getValueString(int value);
    bool lookaheadIsEmpty(Alternative* alt, int maxDepth);
    std::string lookaheadString(int k);
    OStr mangleLiteral(const std::string& s);
    void mapTreeVariable(AlternativeElement* e, const std::string& name);
    OStr processActionForSpecialSymbols(const std::string& actionStr, int line, RuleBlock* rule, ActionTransInfo* tInfo);
    std::string fixNameSpaceOption(std::string ns);
    void setupGrammarParameters(Grammar* g);
    void openOutput(const std::string& fileName);
    void closeOutput();

    // CodeGenerator state
    Tool* antlrTool = nullptr;
    int tabs = 0;
    std::ostream* currentOutput = nullptr;
    OutputFile* currentFile = nullptr;
    Grammar* grammar = nullptr;
    std::vector<BitSet> bitsetsUsed;
    MakeGrammar* behavior = nullptr;
    LLkAnalyzer* analyzer = nullptr;
    bool DEBUG_CODE_GENERATOR = false;
    int makeSwitchThreshold = 2;
    int bitsetTestThreshold = 4;

    // CppCodeGenerator state
    int syntacticPredLevel = 0;
    // parsers report failed guesses with a flag instead of an exception
    bool flagFailures = false;
    bool genAST = false;
    bool saveText = false;
    bool genHashLines = true;
    bool noConstructors = false;
    int outputLine = 0;
    std::string outputFile;
    bool usingCustomAST = false;
    std::string labeledElementType;
    std::string labeledElementASTType;
    std::string labeledElementASTInit;
    std::string labeledElementInit;
    std::string commonExtraArgs;
    std::string commonExtraParams;
    std::string commonLocalVars;
    std::string lt1Value;
    std::string exceptionThrown;
    std::string throwNoViable;
    RuleBlock* currentRule = nullptr;
    std::string currentASTResult;
    // value of nullopt marks a duplicate (NONUNIQUE)
    std::map<std::string, OStr> treeVariableMap;
    std::set<AlternativeElement*> declaredASTVariables;
    int astVarNumber = 1;
    static constexpr int caseSizeThreshold = 127;
    std::vector<std::string> semPreds;
    std::vector<OStr> astTypes;

    std::string namespaceStd = "ANTLR_USE_NAMESPACE(std)";
    std::string namespaceAntlr = "ANTLR_USE_NAMESPACE(antlr)";
    NameSpace* nameSpace = nullptr;
};

} // namespace antlrcpp

#endif
