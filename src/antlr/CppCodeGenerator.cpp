// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file CppCodeGenerator.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.CodeGenerator and antlr.CppCodeGenerator. The generated code
 * is identical to that of ANTLR 2.7.7, including the #line directives.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "CppCodeGenerator.hpp"
#include "LLkAnalyzer.hpp"
#include "MakeGrammar.hpp"
#include "ActionLexer.hpp"
#include <stdexcept>

namespace antlrcpp {

namespace {

const std::string preIncludeCpp = "pre_include_cpp";
const std::string preIncludeHpp = "pre_include_hpp";
const std::string postIncludeCpp = "post_include_cpp";
const std::string postIncludeHpp = "post_include_hpp";
const std::string TokenTypesFileSuffix = "TokenTypes";
const std::string TokenTypesFileExt = ".txt";

// CppCharFormatter
std::string escapeChar(int c, bool) {
    switch (c) {
    case '\n': return "\\n";
    case '\t': return "\\t";
    case '\r': return "\\r";
    case '\\': return "\\\\";
    case '\'': return "\\'";
    case '"': return "\\\"";
    default:
        if (c < ' ' || c > 126) {
            if (c > 255) {
                std::string s = toRadix(c, 16);
                while (s.size() < 4)
                    s = '0' + s;
                return "\\u" + s;
            }
            return "\\" + toRadix(c, 8);
        }
        return std::string(1, static_cast<char>(c));
    }
}

std::string escapeString(const std::string& s) {
    std::string retval;
    for (unsigned char c : s)
        retval += escapeChar(c, false);
    return retval;
}

std::string literalChar(int c) {
    std::string ret = "0x" + toRadix(c, 16);
    if (c >= 0 && c <= 126)
        ret += " /* '" + escapeChar(c, true) + "' */ ";
    return ret;
}

std::string literalString(const std::string& s) {
    return "\"" + escapeString(s) + "\"";
}

bool charIsDigit(const std::string& s, std::size_t i) {
    return i < s.size() && isJavaDigit(static_cast<unsigned char>(s[i]));
}

} // namespace

CppCodeGenerator::CppCodeGenerator() = default;

// CodeGenerator

void CppCodeGenerator::_print(const OStr& s) {
    if (s) {
        outputLine += countLines(*s);
        *currentOutput << *s;
    }
}

void CppCodeGenerator::_printAction(const OStr& os) {
    if (!os)
        return;
    const std::string& s = *os;
    outputLine += countLines(s) + 1;

    int start = 0;
    while (start < static_cast<int>(s.size()) && isJavaSpaceChar(static_cast<unsigned char>(s[static_cast<std::size_t>(start)])))
        start++;
    int end = static_cast<int>(s.size()) - 1;
    while (end > start && isJavaSpaceChar(static_cast<unsigned char>(s[static_cast<std::size_t>(end)])))
        end--;

    for (int i = start; i <= end;) {
        char c = s[static_cast<std::size_t>(i)];
        i++;
        bool newline = false;
        switch (c) {
        case '\n':
            newline = true;
            break;
        case '\r':
            if (i <= end && s[static_cast<std::size_t>(i)] == '\n')
                i++;
            newline = true;
            break;
        default:
            *currentOutput << c;
            break;
        }
        if (newline) {
            *currentOutput << '\n';
            printTabs();
            while (i <= end && isJavaSpaceChar(static_cast<unsigned char>(s[static_cast<std::size_t>(i)])))
                i++;
        }
    }
    *currentOutput << '\n';
}

void CppCodeGenerator::_println(const OStr& s) {
    if (s) {
        outputLine += countLines(*s) + 1;
        *currentOutput << *s << '\n';
    }
}

void CppCodeGenerator::print(const OStr& s) {
    if (s) {
        printTabs();
        *currentOutput << *s;
    }
}

void CppCodeGenerator::printAction(const OStr& s) {
    if (s) {
        printTabs();
        _printAction(s);
    }
}

void CppCodeGenerator::println(const OStr& s) {
    if (s) {
        printTabs();
        outputLine += countLines(*s) + 1;
        *currentOutput << *s << '\n';
    }
}

void CppCodeGenerator::printTabs() {
    for (int i = 1; i <= tabs; i++)
        *currentOutput << '\t';
}

bool CppCodeGenerator::elementsAreRange(const std::vector<int>& elems) {
    if (elems.empty())
        return false;
    int begin = elems[0];
    int end = elems.back();
    if (elems.size() <= 2)
        return false;
    if (end - begin + 1 > static_cast<int>(elems.size()))
        return false;
    int v = begin + 1;
    for (std::size_t i = 1; i < elems.size() - 1; i++) {
        if (v != elems[i])
            return false;
        v++;
    }
    return true;
}

std::string CppCodeGenerator::extractIdOfAction(const std::string& str, int line, int column) {
    std::string s = removeAssignmentFromDeclaration(str);
    for (int i = static_cast<int>(s.size()) - 2; i >= 0; i--) {
        int c = static_cast<unsigned char>(s[static_cast<std::size_t>(i)]);
        if (!isJavaLetterOrDigit(c) && c != '_')
            return s.substr(static_cast<std::size_t>(i + 1));
    }
    antlrTool->warning("Ill-formed action", grammar->getFilename(), line, column);
    return "";
}

std::string CppCodeGenerator::extractTypeOfAction(const std::string& str, int line, int column) {
    std::string s = removeAssignmentFromDeclaration(str);
    for (int i = static_cast<int>(s.size()) - 2; i >= 0; i--) {
        int c = static_cast<unsigned char>(s[static_cast<std::size_t>(i)]);
        if (!isJavaLetterOrDigit(c) && c != '_')
            return s.substr(0, static_cast<std::size_t>(i + 1));
    }
    antlrTool->warning("Ill-formed action", grammar->getFilename(), line, column);
    return "";
}

void CppCodeGenerator::genTokenInterchange(TokenManager* tm) {
    std::string fName = tm->getName() + TokenTypesFileSuffix + TokenTypesFileExt;
    openOutput(fName);
    println("// $ANTLR " + Tool::version + ": " + antlrTool->fileMinusPath(antlrTool->grammarFile) + " -> " + fName + "$");
    tabs = 0;
    println(tm->getName() + "    // output token vocab name");

    auto& v = tm->getVocabulary();
    for (int i = TokenType::MIN_USER_TYPE; i < static_cast<int>(v.size()); i++) {
        const OStr& s = v[static_cast<std::size_t>(i)];
        if (s && !startsWith(*s, "<")) {
            if (startsWith(*s, "\"")) {
                auto sl = dynamic_cast<StringLiteralSymbol*>(tm->getTokenSymbol(*s));
                if (sl && sl->label)
                    print(*sl->label + "=");
                println(*s + "=" + std::to_string(i));
            } else {
                print(*s);
                TokenSymbol* ts = tm->getTokenSymbol(*s);
                if (!ts)
                    antlrTool->warning("undefined token symbol: " + *s);
                else if (ts->paraphrase)
                    print("(" + *ts->paraphrase + ")");
                println("=" + std::to_string(i));
            }
        }
    }
    closeOutput();
}

int CppCodeGenerator::markBitsetForGen(const BitSet& p) {
    for (std::size_t i = 0; i < bitsetsUsed.size(); i++)
        if (p.equals(bitsetsUsed[i]))
            return static_cast<int>(i);
    bitsetsUsed.push_back(p);
    return static_cast<int>(bitsetsUsed.size()) - 1;
}

std::string CppCodeGenerator::removeAssignmentFromDeclaration(std::string d) {
    if (indexOf(d, '=') >= 0)
        d = trim(d.substr(0, static_cast<std::size_t>(indexOf(d, '='))));
    return d;
}

void CppCodeGenerator::reset() {
    tabs = 0;
    bitsetsUsed.clear();
    currentOutput = nullptr;
    grammar = nullptr;
    DEBUG_CODE_GENERATOR = false;
    makeSwitchThreshold = 2;
    bitsetTestThreshold = 4;
}

void CppCodeGenerator::setGrammar(Grammar* g) {
    reset();
    grammar = g;
    if (grammar->hasOption("codeGenMakeSwitchThreshold")) {
        try {
            makeSwitchThreshold = grammar->getIntegerOption("codeGenMakeSwitchThreshold");
        } catch (const std::exception&) {
            TokenRef tok = grammar->getOption("codeGenMakeSwitchThreshold");
            antlrTool->error("option 'codeGenMakeSwitchThreshold' must be an integer", grammar->getClassName(), tok->line, tok->column);
        }
    }
    if (grammar->hasOption("codeGenBitsetTestThreshold")) {
        try {
            bitsetTestThreshold = grammar->getIntegerOption("codeGenBitsetTestThreshold");
        } catch (const std::exception&) {
            TokenRef tok = grammar->getOption("codeGenBitsetTestThreshold");
            antlrTool->error("option 'codeGenBitsetTestThreshold' must be an integer", grammar->getClassName(), tok->line, tok->column);
        }
    }
    if (grammar->hasOption("codeGenDebug")) {
        TokenRef t = grammar->getOption("codeGenDebug");
        if (t->text == "true")
            DEBUG_CODE_GENERATOR = true;
        else if (t->text == "false")
            DEBUG_CODE_GENERATOR = false;
        else
            antlrTool->error("option 'codeGenDebug' must be true or false", grammar->getClassName(), t->line, t->column);
    }
}

OStr CppCodeGenerator::getFOLLOWBitSet(const std::string& ruleName, int k) {
    RuleSymbol* rs = grammar->getSymbol(ruleName);
    if (!rs)
        return std::nullopt;
    RuleBlock* blk = rs->getBlock();
    Lookahead follow = grammar->theLLkAnalyzer->FOLLOW(k, blk->endNode);
    return getBitsetName(markBitsetForGen(follow.fset));
}

OStr CppCodeGenerator::getFIRSTBitSet(const std::string& ruleName, int k) {
    RuleSymbol* rs = grammar->getSymbol(ruleName);
    if (!rs)
        return std::nullopt;
    RuleBlock* blk = rs->getBlock();
    Lookahead first = grammar->theLLkAnalyzer->look(k, blk);
    return getBitsetName(markBitsetForGen(first.fset));
}

void CppCodeGenerator::openOutput(const std::string& fileName) {
    currentFile = antlrTool->openOutputFile(fileName);
    currentOutput = &currentFile->out;
}

void CppCodeGenerator::closeOutput() {
    currentFile->close();
    delete currentFile;
    currentFile = nullptr;
    currentOutput = nullptr;
}

// CppCodeGenerator

void CppCodeGenerator::exitIfError() {
    if (antlrTool->hasError())
        antlrTool->fatalError("Exiting due to errors.");
}

int CppCodeGenerator::countLines(const std::string& s) {
    int lines = 0;
    for (char c : s)
        if (c == '\n')
            lines++;
    return lines;
}

void CppCodeGenerator::printAction(const TokenRef& t) {
    if (t) {
        genLineNo(t->line);
        printTabs();
        _printAction(processActionForSpecialSymbols(t->text, t->line, nullptr, nullptr));
        genLineNo2();
    }
}

void CppCodeGenerator::printHeaderAction(const std::string& name) {
    auto a = behavior->headerActions.get(name);
    if (a) {
        genLineNo((*a)->line);
        println(processActionForSpecialSymbols((*a)->text, (*a)->line, nullptr, nullptr));
        genLineNo2();
    }
}

void CppCodeGenerator::genLineNo(int line) {
    if (line == 0)
        line++;
    if (genHashLines)
        _println("#line " + std::to_string(line) + " \"" + antlrTool->fileMinusPath(antlrTool->grammarFile) + "\"");
}

void CppCodeGenerator::genLineNo(GrammarElement* el) {
    if (el)
        genLineNo(el->getLine());
}

void CppCodeGenerator::genLineNo(const TokenRef& t) {
    if (t)
        genLineNo(t->line);
}

void CppCodeGenerator::genLineNo2() {
    if (genHashLines)
        _println("#line " + std::to_string(outputLine + 1) + " \"" + outputFile + "\"");
}

std::string CppCodeGenerator::convertJavaToCppString(const std::string& lit, bool isCharLiteral) {
    std::string ret;
    std::size_t i = 0;
    int val = 0;

    if (isCharLiteral) {
        if (!startsWith(lit, "'") || !endsWith(lit, "'"))
            antlrTool->error("Invalid character literal: '" + lit + "'");
    } else {
        if (!startsWith(lit, "\"") || !endsWith(lit, "\""))
            antlrTool->error("Invalid character string: '" + lit + "'");
    }
    std::string s = lit.substr(1, lit.size() - 2);

    std::string prefix;
    int maxsize = 255;
    if (grammar->isLexer()) {
        maxsize = static_cast<LexerGrammar*>(grammar)->charVocabulary.size() - 1;
        if (maxsize > 255)
            prefix = "L";
    }

    while (i < s.size()) {
        if (s[i] == '\\') {
            if (s.size() == i + 1)
                antlrTool->error("Invalid escape in char literal: '" + lit + "' looking at '" + s.substr(i) + "'");

            switch (i + 1 < s.size() ? s[i + 1] : '\0') {
            case 'a': val = 7; i += 2; break;
            case 'b': val = 8; i += 2; break;
            case 't': val = 9; i += 2; break;
            case 'n': val = 10; i += 2; break;
            case 'f': val = 12; i += 2; break;
            case 'r': val = 13; i += 2; break;
            case '"':
            case '\'':
            case '\\':
                val = static_cast<unsigned char>(s[i + 1]);
                i += 2;
                break;
            case 'u':
                if (i + 5 < s.size()) {
                    val = hexDigit(s[i + 2]) * 16 * 16 * 16 + hexDigit(s[i + 3]) * 16 * 16 + hexDigit(s[i + 4]) * 16 + hexDigit(s[i + 5]);
                    i += 6;
                } else {
                    antlrTool->error("Invalid escape in char literal: '" + lit + "' looking at '" + s.substr(i) + "'");
                }
                break;
            case '0':
            case '1':
            case '2':
            case '3':
                if (charIsDigit(s, i + 2)) {
                    if (charIsDigit(s, i + 3)) {
                        val = (s[i + 1] - '0') * 8 * 8 + (s[i + 2] - '0') * 8 + (s[i + 3] - '0');
                        i += 4;
                    } else {
                        val = (s[i + 1] - '0') * 8 + (s[i + 2] - '0');
                        i += 3;
                    }
                } else {
                    val = s[i + 1] - '0';
                    i += 2;
                }
                break;
            case '4':
            case '5':
            case '6':
            case '7':
                if (charIsDigit(s, i + 2)) {
                    val = (s[i + 1] - '0') * 8 + (s[i + 2] - '0');
                    i += 3;
                } else {
                    val = s[i + 1] - '0';
                    i += 2;
                }
                // the original falls through to the error of the default case
                antlrTool->error("Unhandled escape in char literal: '" + lit + "' looking at '" + s.substr(std::min(i, s.size())) + "'");
                val = 0;
                break;
            default:
                antlrTool->error("Unhandled escape in char literal: '" + lit + "' looking at '" + s.substr(i) + "'");
                val = 0;
                // the original loops forever here
                i += 2;
            }
        } else {
            val = static_cast<unsigned char>(s[i++]);
        }

        if (grammar->isLexer()) {
            if (val > maxsize) {
                std::string offender;
                if (0x20 <= val && val < 0x7F)
                    offender = escapeChar(val, true);
                else
                    offender = "0x" + toRadix(val, 16);
                antlrTool->error("Character out of range in " + std::string(isCharLiteral ? "char literal" : "string constant") + ": '" + s + "'");
                antlrTool->error("Vocabulary size: " + std::to_string(maxsize) + " Character " + offender);
            }
        }

        if (isCharLiteral) {
            if (i != s.size())
                antlrTool->error("Invalid char literal: '" + lit + "'");
            if (maxsize <= 255) {
                if (val <= 255 && (val & 0x80) != 0)
                    ret = "static_cast<unsigned char>('" + escapeChar(val, true) + "')";
                else
                    ret = "'" + escapeChar(val, true) + "'";
            } else {
                ret = "L'" + escapeChar(val, true) + "'";
            }
        } else {
            ret += escapeChar(val, true);
        }
    }
    if (!isCharLiteral)
        ret = prefix + "\"" + ret + "\"";
    return ret;
}

void CppCodeGenerator::gen() {
    for (auto g : behavior->grammars.values()) {
        if (g->debuggingOutput)
            antlrTool->error(g->getFilename() + ": C++ mode does not support -debug");
        if (g->isTreeWalker())
            antlrTool->error(g->getFilename() + ": tree parsers are not supported");
        if (g->buildAST)
            antlrTool->error(g->getFilename() + ": buildAST is not supported");
        if (g->traceRules)
            antlrTool->error(g->getFilename() + ": tracing is not supported");
        exitIfError();
        g->theLLkAnalyzer = analyzer;
        g->generator = this;
        analyzer->setGrammar(g);
        setupGrammarParameters(g);
        g->generate();
        exitIfError();
    }

    for (auto tm : behavior->tokenManagers.values()) {
        if (!tm->isReadOnly()) {
            genTokenTypes(tm);
            genTokenInterchange(tm);
        }
        exitIfError();
    }
}

void CppCodeGenerator::gen(ActionElement* action) {
    if (action->isSemPred) {
        genSemPred(action->actionText, action->line);
    } else {
        if (grammar->hasSyntacticPredicate) {
            println("if ( inputState->guessing==0 ) {");
            tabs++;
        }

        ActionTransInfo tInfo;
        OStr actionStr = processActionForSpecialSymbols(action->actionText, action->getLine(), currentRule, &tInfo);

        if (tInfo.refRuleRoot)
            println(*tInfo.refRuleRoot + " = " + labeledElementASTType + "(currentAST.root);");

        genLineNo(action);
        printAction(actionStr);
        genLineNo2();

        if (tInfo.assignToRoot) {
            println("currentAST.root = " + *tInfo.refRuleRoot + ";");
            println("if ( " + *tInfo.refRuleRoot + "!=" + labeledElementASTInit + " &&");
            tabs++;
            println(*tInfo.refRuleRoot + "->getFirstChild() != " + labeledElementASTInit + " )");
            println("  currentAST.child = " + *tInfo.refRuleRoot + "->getFirstChild();");
            tabs--;
            println("else");
            tabs++;
            println("currentAST.child = " + *tInfo.refRuleRoot + ";");
            tabs--;
            println("currentAST.advanceChildToEnd();");
        }

        if (grammar->hasSyntacticPredicate) {
            tabs--;
            println("}");
        }
    }
}

void CppCodeGenerator::gen(AlternativeBlock* blk) {
    println("{");
    genBlockPreamble(blk);
    genBlockInitAction(blk);

    std::string saveCurrentASTResult = currentASTResult;
    if (blk->getLabel())
        currentASTResult = *blk->getLabel();

    grammar->theLLkAnalyzer->deterministic(blk);

    CppBlockFinishingInfo howToFinish = genCommonBlock(blk, true);
    genBlockFinish(howToFinish, throwNoViable);

    println("}");

    currentASTResult = saveCurrentASTResult;
}

void CppCodeGenerator::gen(CharLiteralElement* atom) {
    if (!grammar->isLexer())
        antlrTool->error("cannot ref character literals in grammar: " + atom->atomText);

    if (atom->getLabel())
        println(*atom->getLabel() + " = " + lt1Value + ";");

    bool oldsaveText = saveText;
    saveText = saveText && atom->getAutoGenType() == AUTO_GEN_NONE;

    if (!saveText || atom->getAutoGenType() == AUTO_GEN_BANG)
        println("_saveIndex = text.length();");

    print(atom->not_ ? "matchNot(" : "match(");
    _print(convertJavaToCppString(atom->atomText, true));
    _println(" /* charlit */ );");

    if (!saveText || atom->getAutoGenType() == AUTO_GEN_BANG)
        println("text.erase(_saveIndex);");

    saveText = oldsaveText;
}

void CppCodeGenerator::gen(CharRangeElement* r) {
    if (!grammar->isLexer())
        antlrTool->error("cannot ref character range in grammar: " + r->beginText + ".." + r->endText);

    if (r->getLabel() && syntacticPredLevel == 0)
        println(*r->getLabel() + " = " + lt1Value + ";");

    bool save = grammar->isLexer() && (!saveText || r->getAutoGenType() == AUTO_GEN_BANG);
    if (save)
        println("_saveIndex=text.length();");

    println("matchRange(" + convertJavaToCppString(r->beginText, true) + "," + convertJavaToCppString(r->endText, true) + ");");

    if (save)
        println("text.erase(_saveIndex);");
}

void CppCodeGenerator::gen(LexerGrammar* g) {
    if (g->charVocabulary.size() > 256)
        antlrTool->warning(g->getFilename() + ": Vocabularies of this size still experimental in C++ mode (vocabulary size now: " +
                           std::to_string(g->charVocabulary.size()) + ")");

    setGrammar(g);
    genBody(g);
    genInclude(g);
}

void CppCodeGenerator::gen(OneOrMoreBlock* blk) {
    std::string label;
    std::string cnt;
    println("{ // ( ... )+");
    genBlockPreamble(blk);
    if (blk->getLabel())
        cnt = "_cnt_" + *blk->getLabel();
    else
        cnt = "_cnt" + std::to_string(blk->ID);
    println("int " + cnt + "=0;");
    if (blk->getLabel())
        label = *blk->getLabel();
    else
        label = "_loop" + std::to_string(blk->ID);

    println("for (;;) {");
    tabs++;
    genBlockInitAction(blk);

    std::string saveCurrentASTResult = currentASTResult;
    if (blk->getLabel())
        currentASTResult = *blk->getLabel();

    grammar->theLLkAnalyzer->deterministic(blk);

    bool generateNonGreedyExitPath = false;
    int nonGreedyExitDepth = grammar->maxk;

    if (!blk->greedy && blk->exitLookaheadDepth <= grammar->maxk &&
        blk->exitCache[static_cast<std::size_t>(blk->exitLookaheadDepth)]->containsEpsilon()) {
        generateNonGreedyExitPath = true;
        nonGreedyExitDepth = blk->exitLookaheadDepth;
    } else if (!blk->greedy && blk->exitLookaheadDepth == NONDETERMINISTIC) {
        generateNonGreedyExitPath = true;
    }

    if (generateNonGreedyExitPath) {
        std::string predictExit = getLookaheadTestExpression(blk->exitCache, nonGreedyExitDepth);
        println("// nongreedy exit test");
        println("if ( " + cnt + ">=1 && " + predictExit + ") goto " + label + ";");
    }

    CppBlockFinishingInfo howToFinish = genCommonBlock(blk, false);
    genBlockFinish(howToFinish, "if ( " + cnt + ">=1 ) { goto " + label + "; } else {" + throwNoViable + "}");

    println(cnt + "++;");
    tabs--;
    println("}");
    println(label + ":;");
    println("}  // ( ... )+");

    currentASTResult = saveCurrentASTResult;
}

void CppCodeGenerator::gen(ParserGrammar* g) {
    setGrammar(g);
    genBody(g);
    genInclude(g);
}

void CppCodeGenerator::gen(RuleRefElement* rr) {
    RuleSymbol* rs = grammar->getSymbol(rr->targetRule);
    if (!rs || !rs->isDefined()) {
        antlrTool->error("Rule '" + rr->targetRule + "' is not defined", grammar->getFilename(), rr->getLine(), rr->getColumn());
        return;
    }

    genErrorTryForElement(rr);

    if (grammar->isTreeWalker() && rr->getLabel() && syntacticPredLevel == 0)
        println(*rr->getLabel() + " = (_t == ASTNULL) ? " + labeledElementASTInit + " : " + lt1Value + ";");

    if (grammar->isLexer() && (!saveText || rr->getAutoGenType() == AUTO_GEN_BANG))
        println("_saveIndex = text.length();");

    printTabs();
    if (rr->idAssign) {
        if (!rs->block->returnAction)
            antlrTool->warning("Rule '" + rr->targetRule + "' has no return type", grammar->getFilename(), rr->getLine(), rr->getColumn());
        _print(*rr->idAssign + "=");
    } else {
        if (!grammar->isLexer() && syntacticPredLevel == 0 && rs->block->returnAction)
            antlrTool->warning("Rule '" + rr->targetRule + "' returns a value", grammar->getFilename(), rr->getLine(), rr->getColumn());
    }

    GenRuleInvocation(rr);

    if (grammar->isLexer() && (!saveText || rr->getAutoGenType() == AUTO_GEN_BANG))
        println("text.erase(_saveIndex);");

    if (syntacticPredLevel == 0) {
        bool doNoGuessTest = grammar->hasSyntacticPredicate &&
                             ((grammar->buildAST && rr->getLabel()) || (genAST && rr->getAutoGenType() == AUTO_GEN_NONE));

        if (doNoGuessTest) {
            println("if (inputState->guessing==0) {");
            tabs++;
        }

        if (grammar->buildAST && rr->getLabel())
            println(*rr->getLabel() + "_AST = returnAST;");

        if (genAST) {
            switch (rr->getAutoGenType()) {
            case AUTO_GEN_NONE:
                if (usingCustomAST)
                    println("astFactory->addASTChild(currentAST, " + namespaceAntlr + "RefAST(returnAST));");
                else
                    println("astFactory->addASTChild( currentAST, returnAST );");
                break;
            case AUTO_GEN_CARET:
                antlrTool->error("Internal: encountered ^ after rule reference");
                break;
            default:
                break;
            }
        }

        if (grammar->isLexer() && rr->getLabel())
            println(*rr->getLabel() + "=_returnToken;");

        if (doNoGuessTest) {
            tabs--;
            println("}");
        }
    }
    genErrorCatchForElement(rr);
}

void CppCodeGenerator::gen(StringLiteralElement* atom) {
    if (atom->getLabel() && syntacticPredLevel == 0)
        println(*atom->getLabel() + " = " + lt1Value + ";");

    genElementAST(atom);

    bool oldsaveText = saveText;
    saveText = saveText && atom->getAutoGenType() == AUTO_GEN_NONE;

    genMatch(atom);

    saveText = oldsaveText;

    if (grammar->isTreeWalker())
        println("_t = _t->getNextSibling();");
}

void CppCodeGenerator::gen(TokenRangeElement* r) {
    genErrorTryForElement(r);
    if (r->getLabel() && syntacticPredLevel == 0)
        println(*r->getLabel() + " = " + lt1Value + ";");

    genElementAST(r);

    println("matchRange(" + r->beginText + "," + r->endText + ");");
    genErrorCatchForElement(r);
}

void CppCodeGenerator::gen(TokenRefElement* atom) {
    if (grammar->isLexer())
        antlrTool->panic("Token reference found in lexer");
    genErrorTryForElement(atom);
    if (atom->getLabel() && syntacticPredLevel == 0)
        println(*atom->getLabel() + " = " + lt1Value + ";");

    genElementAST(atom);
    genMatch(atom);
    genErrorCatchForElement(atom);

    if (grammar->isTreeWalker())
        println("_t = _t->getNextSibling();");
}

void CppCodeGenerator::gen(TreeElement* t) {
    println(labeledElementType + " __t" + std::to_string(t->ID) + " = _t;");

    if (t->root->getLabel())
        println(*t->root->getLabel() + " = (_t == " + labeledElementType + "(ASTNULL)) ? " + labeledElementASTInit + " : _t;");

    if (t->root->getAutoGenType() == AUTO_GEN_BANG) {
        antlrTool->error("Suffixing a root node with '!' is not implemented", grammar->getFilename(), t->getLine(), t->getColumn());
        t->root->setAutoGenType(AUTO_GEN_NONE);
    }
    if (t->root->getAutoGenType() == AUTO_GEN_CARET) {
        antlrTool->warning("Suffixing a root node with '^' is redundant; already a root", grammar->getFilename(), t->getLine(), t->getColumn());
        t->root->setAutoGenType(AUTO_GEN_NONE);
    }

    genElementAST(t->root);
    if (grammar->buildAST) {
        println(namespaceAntlr + "ASTPair __currentAST" + std::to_string(t->ID) + " = currentAST;");
        println("currentAST.root = currentAST.child;");
        println("currentAST.child = " + labeledElementASTInit + ";");
    }

    if (dynamic_cast<WildcardElement*>(t->root))
        println("if ( _t == ASTNULL ) throw " + namespaceAntlr + "MismatchedTokenException();");
    else
        genMatch(t->root);
    println("_t = _t->getFirstChild();");

    for (int i = 0; i < t->numAlternatives(); i++) {
        Alternative* a = t->getAlternativeAt(i);
        for (AlternativeElement* e = a->head; e; e = e->next)
            e->generate();
    }

    if (grammar->buildAST)
        println("currentAST = __currentAST" + std::to_string(t->ID) + ";");
    println("_t = __t" + std::to_string(t->ID) + ";");
    println("_t = _t->getNextSibling();");
}

void CppCodeGenerator::gen(TreeWalkerGrammar* g) {
    setGrammar(g);
    genBody(g);
    genInclude(g);
}

void CppCodeGenerator::gen(WildcardElement* wc) {
    if (wc->getLabel() && syntacticPredLevel == 0)
        println(*wc->getLabel() + " = " + lt1Value + ";");

    genElementAST(wc);
    if (grammar->isTreeWalker()) {
        println("if ( _t == " + labeledElementASTInit + " ) throw " + namespaceAntlr + "MismatchedTokenException();");
    } else if (grammar->isLexer()) {
        if (!saveText || wc->getAutoGenType() == AUTO_GEN_BANG)
            println("_saveIndex = text.length();");
        println("matchNot(EOF/*_CHAR*/);");
        if (!saveText || wc->getAutoGenType() == AUTO_GEN_BANG)
            println("text.erase(_saveIndex);");
    } else {
        println("matchNot(" + getValueString(TokenType::EOF_TYPE) + ");");
    }

    if (grammar->isTreeWalker())
        println("_t = _t->getNextSibling();");
}

void CppCodeGenerator::gen(ZeroOrMoreBlock* blk) {
    println("{ // ( ... )*");
    genBlockPreamble(blk);
    std::string label;
    if (blk->getLabel())
        label = *blk->getLabel();
    else
        label = "_loop" + std::to_string(blk->ID);
    println("for (;;) {");
    tabs++;
    genBlockInitAction(blk);

    std::string saveCurrentASTResult = currentASTResult;
    if (blk->getLabel())
        currentASTResult = *blk->getLabel();

    grammar->theLLkAnalyzer->deterministic(blk);

    bool generateNonGreedyExitPath = false;
    int nonGreedyExitDepth = grammar->maxk;

    if (!blk->greedy && blk->exitLookaheadDepth <= grammar->maxk &&
        blk->exitCache[static_cast<std::size_t>(blk->exitLookaheadDepth)]->containsEpsilon()) {
        generateNonGreedyExitPath = true;
        nonGreedyExitDepth = blk->exitLookaheadDepth;
    } else if (!blk->greedy && blk->exitLookaheadDepth == NONDETERMINISTIC) {
        generateNonGreedyExitPath = true;
    }
    if (generateNonGreedyExitPath) {
        std::string predictExit = getLookaheadTestExpression(blk->exitCache, nonGreedyExitDepth);
        println("// nongreedy exit test");
        println("if (" + predictExit + ") goto " + label + ";");
    }

    CppBlockFinishingInfo howToFinish = genCommonBlock(blk, false);
    genBlockFinish(howToFinish, "goto " + label + ";");

    tabs--;
    println("}");
    println(label + ":;");
    println("} // ( ... )*");

    currentASTResult = saveCurrentASTResult;
}

void CppCodeGenerator::genAlt(Alternative* alt, AlternativeBlock* blk) {
    bool savegenAST = genAST;
    genAST = genAST && alt->getAutoGen();

    bool oldsaveTest = saveText;
    saveText = saveText && alt->getAutoGen();

    auto saveMap = treeVariableMap;
    treeVariableMap.clear();

    if (alt->exceptionSpec) {
        println("try {      // for error handling");
        tabs++;
    }

    AlternativeElement* elem = alt->head;
    while (!dynamic_cast<BlockEndElement*>(elem)) {
        elem->generate();
        elem = elem->next;
    }

    if (genAST) {
        if (auto rblk = dynamic_cast<RuleBlock*>(blk)) {
            if (usingCustomAST)
                println(rblk->getRuleName() + "_AST = " + labeledElementASTType + "(currentAST.root);");
            else
                println(rblk->getRuleName() + "_AST = currentAST.root;");
        } else if (blk->getLabel()) {
            antlrTool->warning("Labeled subrules are not implemented", grammar->getFilename(), blk->getLine(), blk->getColumn());
        }
    }

    if (alt->exceptionSpec) {
        tabs--;
        println("}");
        genErrorHandler(alt->exceptionSpec);
    }

    genAST = savegenAST;
    saveText = oldsaveTest;

    treeVariableMap = saveMap;
}

void CppCodeGenerator::genBitsets(std::vector<BitSet>& bitsetList, int maxVocabulary, const std::string& prefix) {
    TokenManager* tm = grammar->tokenManager;

    println("");

    for (std::size_t i = 0; i < bitsetList.size(); i++) {
        BitSet& p = bitsetList[i];
        p.growToInclude(maxVocabulary);

        println("const unsigned long " + prefix + getBitsetName(static_cast<int>(i)) + "_data_" + "[] = { " + p.toStringOfHalfWords() + " };");

        std::string t = "// ";
        for (int j = 0; j < tm->vocabularySize(); j++) {
            if (p.member(j)) {
                if (grammar->isLexer()) {
                    if (0x20 <= j && j < 0x7F && j != '\\')
                        t += escapeChar(j, true) + " ";
                    else
                        t += "0x" + toRadix(j, 16) + " ";
                } else {
                    OStr s = tm->getTokenStringAt(j);
                    t += (s ? *s : std::string("null")) + " ";
                }

                if (t.size() > 70) {
                    println(t);
                    t = "// ";
                }
            }
        }
        if (t != "// ")
            println(t);

        println("const " + namespaceAntlr + "BitSet " + prefix + getBitsetName(static_cast<int>(i)) + "(" + getBitsetName(static_cast<int>(i)) +
                "_data_," + std::to_string(p.size() / 32) + ");");
    }
}

void CppCodeGenerator::genBitsetsHeader(std::vector<BitSet>& bitsetList, int maxVocabulary) {
    println("");
    for (std::size_t i = 0; i < bitsetList.size(); i++) {
        BitSet& p = bitsetList[i];
        p.growToInclude(maxVocabulary);
        println("static const unsigned long " + getBitsetName(static_cast<int>(i)) + "_data_" + "[];");
        println("static const " + namespaceAntlr + "BitSet " + getBitsetName(static_cast<int>(i)) + ";");
    }
}

void CppCodeGenerator::genBlockFinish(const CppBlockFinishingInfo& howToFinish, const std::string& noViableAction) {
    if (howToFinish.needAnErrorClause && (howToFinish.generatedAnIf || howToFinish.generatedSwitch)) {
        if (howToFinish.generatedAnIf)
            println("else {");
        else
            println("{");
        tabs++;
        println(noViableAction);
        tabs--;
        println("}");
    }

    if (howToFinish.postscript)
        println(*howToFinish.postscript);
}

void CppCodeGenerator::genBlockInitAction(AlternativeBlock* blk) {
    if (blk->initAction) {
        genLineNo(blk);
        printAction(processActionForSpecialSymbols(*blk->initAction, blk->line, currentRule, nullptr));
        genLineNo2();
    }
}

void CppCodeGenerator::genBlockPreamble(AlternativeBlock* blk) {
    auto rblk = dynamic_cast<RuleBlock*>(blk);
    if (!rblk)
        return;
    for (auto a : rblk->labeledElements) {
        auto ab = dynamic_cast<AlternativeBlock*>(a);
        if (dynamic_cast<RuleRefElement*>(a) || (ab && !dynamic_cast<RuleBlock*>(a) && !dynamic_cast<SynPredBlock*>(a))) {
            if (!dynamic_cast<RuleRefElement*>(a) && ab->not_ && analyzer->subruleCanBeInverted(ab, grammar->isLexer())) {
                println(labeledElementType + " " + *a->getLabel() + " = " + labeledElementInit + ";");
                if (grammar->buildAST)
                    genASTDeclaration(a);
            } else {
                if (grammar->buildAST)
                    genASTDeclaration(a);
                if (grammar->isLexer())
                    println(namespaceAntlr + "RefToken " + *a->getLabel() + ";");
                if (grammar->isTreeWalker())
                    println(labeledElementType + " " + *a->getLabel() + " = " + labeledElementInit + ";");
            }
        } else {
            println(labeledElementType + " " + *a->getLabel() + " = " + labeledElementInit + ";");
            if (grammar->buildAST) {
                auto ga = dynamic_cast<GrammarAtom*>(a);
                if (ga && ga->getASTNodeType())
                    genASTDeclaration(a, "Ref" + *ga->getASTNodeType());
                else
                    genASTDeclaration(a);
            }
        }
    }
}

void CppCodeGenerator::genBody(LexerGrammar* g) {
    outputFile = grammar->getClassName() + ".cpp";
    outputLine = 1;
    openOutput(outputFile);

    genAST = false;
    saveText = true;

    tabs = 0;

    genHeader(outputFile);

    printHeaderAction(preIncludeCpp);
    println("#include \"" + grammar->getClassName() + ".hpp\"");
    println("");
    printHeaderAction(postIncludeCpp);

    if (nameSpace)
        nameSpace->emitDeclarations(*currentOutput);

    printAction(grammar->preambleAction);

    std::string sup;
    if (grammar->superClass) {
        sup = *grammar->superClass;
    } else {
        sup = grammar->getSuperClass();
        if (lastIndexOf(sup, '.') != -1)
            sup = sup.substr(static_cast<std::size_t>(lastIndexOf(sup, '.') + 1));
        sup = namespaceAntlr + sup;
    }

    std::string caseSensitive = g->caseSensitive ? "true" : "false";

    if (noConstructors) {
        println("#if 0");
        println("// constructor creation turned of with 'noConstructor' option");
    }

    println(grammar->getClassName() + "::" + grammar->getClassName() + "(" + namespaceStd + "istream& in)");
    tabs++;
    println(": " + sup + "(new " + namespaceAntlr + "CharBuffer(in)," + caseSensitive + ")");
    tabs--;
    println("{");
    tabs++;
    println("initLiterals();");
    tabs--;
    println("}");
    println("");

    println(grammar->getClassName() + "::" + grammar->getClassName() + "(" + namespaceAntlr + "InputBuffer& ib)");
    tabs++;
    println(": " + sup + "(ib," + caseSensitive + ")");
    tabs--;
    println("{");
    tabs++;
    println("initLiterals();");
    tabs--;
    println("}");
    println("");

    println(grammar->getClassName() + "::" + grammar->getClassName() + "(const " + namespaceAntlr + "LexerSharedInputState& state)");
    tabs++;
    println(": " + sup + "(state," + caseSensitive + ")");
    tabs--;
    println("{");
    tabs++;
    println("initLiterals();");
    tabs--;
    println("}");
    println("");

    if (noConstructors) {
        println("// constructor creation turned of with 'noConstructor' option");
        println("#endif");
    }

    println("void " + grammar->getClassName() + "::initLiterals()");
    println("{");
    tabs++;
    for (auto& key : grammar->tokenManager->getTokenSymbolKeys()) {
        if (key[0] != '"')
            continue;
        TokenSymbol* sym = grammar->tokenManager->getTokenSymbol(key);
        if (auto s = dynamic_cast<StringLiteralSymbol*>(sym))
            println("literals[" + s->getId() + "] = " + std::to_string(s->getTokenType()) + ";");
    }
    tabs--;
    println("}");

    genNextToken();

    int ruleNum = 0;
    for (std::size_t i = 0; i < grammar->rules.size(); i++) {
        RuleSymbol* sym = grammar->rules[i];
        if (sym->getId() != "mnextToken")
            genRule(sym, false, ruleNum++, grammar->getClassName() + "::");
        exitIfError();
    }

    genBitsets(bitsetsUsed, static_cast<LexerGrammar*>(grammar)->charVocabulary.size(), grammar->getClassName() + "::");

    println("");
    if (nameSpace)
        nameSpace->emitClosures(*currentOutput);

    closeOutput();
}

void CppCodeGenerator::genInitFactory(Grammar* g) {
    std::string param_name = "factory ";
    if (!g->buildAST)
        param_name = "";

    println("void " + g->getClassName() + "::initializeASTFactory( " + namespaceAntlr + "ASTFactory& " + param_name + ")");
    println("{");
    tabs++;

    if (g->buildAST) {
        TokenManager* tm = grammar->tokenManager;
        for (auto& tok : tm->getTokenSymbolKeys()) {
            TokenSymbol* ts = tm->getTokenSymbol(tok);
            if (ts->ASTNodeType) {
                int tt = ts->getTokenType();
                if (static_cast<int>(astTypes.size()) <= tt)
                    astTypes.resize(static_cast<std::size_t>(tt + 1));
                OStr& type = astTypes[static_cast<std::size_t>(tt)];
                if (!type) {
                    type = ts->ASTNodeType;
                } else if (*ts->ASTNodeType != *type) {
                    antlrTool->warning("Token " + tok + " taking most specific AST type", grammar->getFilename(), 1, 1);
                    antlrTool->warning("  using " + *type + " ignoring " + *ts->ASTNodeType, grammar->getFilename(), 1, 1);
                }
            }
        }
        for (std::size_t i = 0; i < astTypes.size(); i++) {
            if (astTypes[i])
                println("factory.registerFactory(" + std::to_string(i) + ", \"" + *astTypes[i] + "\", " + *astTypes[i] + "::factory);");
        }
        println("factory.setMaxNodeType(" + std::to_string(grammar->tokenManager->maxTokenType()) + ");");
    }
    tabs--;
    println("}");
}

void CppCodeGenerator::genBody(ParserGrammar*) {
    outputFile = grammar->getClassName() + ".cpp";
    outputLine = 1;
    openOutput(outputFile);

    genAST = grammar->buildAST;

    tabs = 0;

    genHeader(outputFile);

    printHeaderAction(preIncludeCpp);

    println("#include \"" + grammar->getClassName() + ".hpp\"");

    printHeaderAction(postIncludeCpp);

    if (nameSpace)
        nameSpace->emitDeclarations(*currentOutput);

    printAction(grammar->preambleAction);

    std::string sup;
    if (grammar->superClass) {
        sup = *grammar->superClass;
    } else {
        sup = grammar->getSuperClass();
        if (lastIndexOf(sup, '.') != -1)
            sup = sup.substr(static_cast<std::size_t>(lastIndexOf(sup, '.') + 1));
        sup = namespaceAntlr + sup;
    }

    if (noConstructors) {
        println("#if 0");
        println("// constructor creation turned of with 'noConstructor' option");
    }

    std::string maxk = std::to_string(grammar->maxk);
    const std::string& cn = grammar->getClassName();

    print(cn + "::" + cn);
    println("(" + namespaceAntlr + "TokenBuffer& tokenBuf, int k)");
    println(": " + sup + "(tokenBuf,k)");
    println("{");
    println("}");
    println("");

    print(cn + "::" + cn);
    println("(" + namespaceAntlr + "TokenBuffer& tokenBuf)");
    println(": " + sup + "(tokenBuf," + maxk + ")");
    println("{");
    println("}");
    println("");

    print(cn + "::" + cn);
    println("(" + namespaceAntlr + "TokenStream& lexer, int k)");
    println(": " + sup + "(lexer,k)");
    println("{");
    println("}");
    println("");

    print(cn + "::" + cn);
    println("(" + namespaceAntlr + "TokenStream& lexer)");
    println(": " + sup + "(lexer," + maxk + ")");
    println("{");
    println("}");
    println("");

    print(cn + "::" + cn);
    println("(const " + namespaceAntlr + "ParserSharedInputState& state)");
    println(": " + sup + "(state," + maxk + ")");
    println("{");
    println("}");
    println("");

    if (noConstructors) {
        println("// constructor creation turned of with 'noConstructor' option");
        println("#endif");
    }

    astTypes.clear();

    int ruleNum = 0;
    for (std::size_t i = 0; i < grammar->rules.size(); i++) {
        RuleSymbol* rs = grammar->rules[i];
        genRule(rs, rs->references.empty(), ruleNum++, cn + "::");
        exitIfError();
    }

    genTokenStrings(cn + "::");

    genBitsets(bitsetsUsed, grammar->tokenManager->maxTokenType(), cn + "::");

    println("");
    println("");
    if (nameSpace)
        nameSpace->emitClosures(*currentOutput);

    closeOutput();
}

void CppCodeGenerator::genBody(TreeWalkerGrammar*) {
    outputFile = grammar->getClassName() + ".cpp";
    outputLine = 1;
    openOutput(outputFile);

    genAST = grammar->buildAST;
    tabs = 0;

    genHeader(outputFile);

    printHeaderAction(preIncludeCpp);

    println("#include \"" + grammar->getClassName() + ".hpp\"");
    println("#include <antlr/Token.hpp>");
    println("#include <antlr/AST.hpp>");
    println("#include <antlr/NoViableAltException.hpp>");
    println("#include <antlr/MismatchedTokenException.hpp>");
    println("#include <antlr/SemanticException.hpp>");
    println("#include <antlr/BitSet.hpp>");

    printHeaderAction(postIncludeCpp);

    if (nameSpace)
        nameSpace->emitDeclarations(*currentOutput);

    printAction(grammar->preambleAction);

    if (noConstructors) {
        println("#if 0");
        println("// constructor creation turned of with 'noConstructor' option");
    }

    println(grammar->getClassName() + "::" + grammar->getClassName() + "()");
    println("\t: " + namespaceAntlr + "TreeParser() {");
    tabs++;
    tabs--;
    println("}");

    if (noConstructors) {
        println("// constructor creation turned of with 'noConstructor' option");
        println("#endif");
    }
    println("");

    astTypes.clear();

    int ruleNum = 0;
    for (std::size_t i = 0; i < grammar->rules.size(); i++) {
        RuleSymbol* rs = grammar->rules[i];
        genRule(rs, rs->references.empty(), ruleNum++, grammar->getClassName() + "::");
        exitIfError();
    }

    genInitFactory(grammar);
    genTokenStrings(grammar->getClassName() + "::");

    genBitsets(bitsetsUsed, grammar->tokenManager->maxTokenType(), grammar->getClassName() + "::");

    println("");
    println("");

    if (nameSpace)
        nameSpace->emitClosures(*currentOutput);

    closeOutput();
}

void CppCodeGenerator::genCases(const BitSet& p) {
    auto elems = p.toArray();
    int wrap = 1;
    int j = 1;
    bool startOfLine = true;
    for (std::size_t i = 0; i < elems.size(); i++) {
        if (j == 1)
            print("");
        else
            _print("  ");
        _print("case " + getValueString(elems[i]) + ":");

        if (j == wrap) {
            _println("");
            startOfLine = true;
            j = 1;
        } else {
            j++;
            startOfLine = false;
        }
    }
    if (!startOfLine)
        _println("");
}

CppBlockFinishingInfo CppCodeGenerator::genCommonBlock(AlternativeBlock* blk, bool noTestForSingle) {
    int nIF = 0;
    bool createdLL1Switch = false;
    int closingBracesOfIFSequence = 0;
    CppBlockFinishingInfo finishingInfo;

    bool savegenAST = genAST;
    genAST = genAST && blk->getAutoGen();

    bool oldsaveTest = saveText;
    saveText = saveText && blk->getAutoGen();

    if (blk->not_ && analyzer->subruleCanBeInverted(blk, grammar->isLexer())) {
        Lookahead p = analyzer->look(1, blk);
        if (blk->getLabel() && syntacticPredLevel == 0)
            println(*blk->getLabel() + " = " + lt1Value + ";");

        genElementAST(blk);

        std::string astArgs;
        if (grammar->isTreeWalker()) {
            if (usingCustomAST)
                astArgs = namespaceAntlr + "RefAST" + "(_t),";
            else
                astArgs = "_t,";
        }

        println("match(" + astArgs + getBitsetName(markBitsetForGen(p.fset)) + ");");

        if (grammar->isTreeWalker())
            println("_t = _t->getNextSibling();");
        return finishingInfo;
    }

    if (blk->numAlternatives() == 1) {
        Alternative* alt = blk->getAlternativeAt(0);
        if (alt->synPred)
            antlrTool->warning("Syntactic predicate superfluous for single alternative", grammar->getFilename(),
                               blk->getAlternativeAt(0)->synPred->getLine(), blk->getAlternativeAt(0)->synPred->getColumn());
        if (noTestForSingle) {
            if (alt->semPred)
                genSemPred(*alt->semPred, blk->line);
            genAlt(alt, blk);
            return finishingInfo;
        }
    }

    int nLL1 = 0;
    for (int i = 0; i < blk->numAlternatives(); i++)
        if (suitableForCaseExpression(blk->getAlternativeAt(i)))
            nLL1++;

    if (nLL1 >= makeSwitchThreshold) {
        std::string testExpr = lookaheadString(1);
        createdLL1Switch = true;
        if (grammar->isTreeWalker()) {
            println("if (_t == " + labeledElementASTInit + " )");
            tabs++;
            println("_t = ASTNULL;");
            tabs--;
        }
        println("switch ( " + testExpr + ") {");
        for (int i = 0; i < blk->numAlternatives(); i++) {
            Alternative* alt = blk->getAlternativeAt(i);
            if (!suitableForCaseExpression(alt))
                continue;
            const Lookahead& p = *alt->cache[1];
            if (p.fset.degree() == 0 && !p.containsEpsilon()) {
                antlrTool->warning("Alternate omitted due to empty prediction set", grammar->getFilename(), alt->head->getLine(), alt->head->getColumn());
            } else {
                genCases(p.fset);
                println("{");
                tabs++;
                genAlt(alt, blk);
                println("break;");
                tabs--;
                println("}");
            }
        }
        println("default:");
        tabs++;
    }

    int startDepth = grammar->isLexer() ? grammar->maxk : 0;
    for (int altDepth = startDepth; altDepth >= 0; altDepth--) {
        for (int i = 0; i < blk->numAlternatives(); i++) {
            Alternative* alt = blk->getAlternativeAt(i);
            if (createdLL1Switch && suitableForCaseExpression(alt))
                continue;
            std::string e;

            bool unpredicted = false;

            if (grammar->isLexer()) {
                int effectiveDepth = alt->lookaheadDepth;
                if (effectiveDepth == NONDETERMINISTIC)
                    effectiveDepth = grammar->maxk;
                while (effectiveDepth >= 1 && alt->cache[static_cast<std::size_t>(effectiveDepth)]->containsEpsilon())
                    effectiveDepth--;
                if (effectiveDepth != altDepth)
                    continue;
                unpredicted = lookaheadIsEmpty(alt, effectiveDepth);
                e = getLookaheadTestExpression(alt, effectiveDepth);
            } else {
                unpredicted = lookaheadIsEmpty(alt, grammar->maxk);
                e = getLookaheadTestExpression(alt, grammar->maxk);
            }

            if (alt->cache[1]->fset.degree() > caseSizeThreshold && suitableForCaseExpression(alt)) {
                if (nIF == 0) {
                    if (grammar->isTreeWalker()) {
                        println("if (_t == " + labeledElementASTInit + " )");
                        tabs++;
                        println("_t = ASTNULL;");
                        tabs--;
                    }
                    println("if " + e + " {");
                } else {
                    println("else if " + e + " {");
                }
            } else if (unpredicted && !alt->semPred && !alt->synPred) {
                if (nIF == 0)
                    println("{");
                else
                    println("else {");
                finishingInfo.needAnErrorClause = false;
            } else {
                if (alt->semPred) {
                    ActionTransInfo tInfo;
                    OStr actionStr = processActionForSpecialSymbols(*alt->semPred, blk->line, currentRule, &tInfo);
                    e = "(" + e + "&&(" + (actionStr ? *actionStr : std::string("null")) + "))";
                }

                if (nIF > 0) {
                    if (alt->synPred) {
                        println("else {");
                        tabs++;
                        genSynPred(alt->synPred, e);
                        closingBracesOfIFSequence++;
                    } else {
                        println("else if " + e + " {");
                    }
                } else {
                    if (alt->synPred) {
                        genSynPred(alt->synPred, e);
                    } else {
                        if (grammar->isTreeWalker()) {
                            println("if (_t == " + labeledElementASTInit + " )");
                            tabs++;
                            println("_t = ASTNULL;");
                            tabs--;
                        }
                        println("if " + e + " {");
                    }
                }
            }

            nIF++;
            tabs++;
            genAlt(alt, blk);
            tabs--;
            println("}");
        }
    }
    std::string ps;
    for (int i = 1; i <= closingBracesOfIFSequence; i++) {
        tabs--;
        ps += "}";
    }

    genAST = savegenAST;

    saveText = oldsaveTest;

    if (createdLL1Switch) {
        tabs--;
        finishingInfo.postscript = ps + "}";
        finishingInfo.generatedSwitch = true;
        finishingInfo.generatedAnIf = nIF > 0;
    } else {
        finishingInfo.postscript = ps;
        finishingInfo.generatedSwitch = false;
        finishingInfo.generatedAnIf = nIF > 0;
    }
    return finishingInfo;
}

bool CppCodeGenerator::suitableForCaseExpression(Alternative* a) {
    return a->lookaheadDepth == 1 && !a->semPred && !a->cache[1]->containsEpsilon() && a->cache[1]->fset.degree() <= caseSizeThreshold;
}

void CppCodeGenerator::genElementAST(AlternativeElement* el) {
    auto ga = dynamic_cast<GrammarAtom*>(el);

    if (grammar->isTreeWalker() && !grammar->buildAST) {
        if (!el->getLabel()) {
            std::string elementRef = lt1Value;
            std::string astName = "tmp" + std::to_string(astVarNumber) + "_AST";
            astVarNumber++;
            mapTreeVariable(el, astName);
            println(labeledElementASTType + " " + astName + "_in = " + elementRef + ";");
        }
        return;
    }

    if (grammar->buildAST && syntacticPredLevel == 0) {
        bool needASTDecl = genAST && (el->getLabel() || el->getAutoGenType() != AUTO_GEN_BANG);

        if (el->getAutoGenType() != AUTO_GEN_BANG && dynamic_cast<TokenRefElement*>(el))
            needASTDecl = true;

        bool doNoGuessTest = grammar->hasSyntacticPredicate && needASTDecl;

        std::string elementRef;
        std::string astNameBase;

        if (el->getLabel()) {
            elementRef = *el->getLabel();
            astNameBase = *el->getLabel();
        } else {
            elementRef = lt1Value;
            astNameBase = "tmp" + std::to_string(astVarNumber);
            astVarNumber++;
        }

        if (needASTDecl) {
            if (ga && ga->getASTNodeType())
                genASTDeclaration(el, astNameBase, "Ref" + *ga->getASTNodeType());
            else
                genASTDeclaration(el, astNameBase, labeledElementASTType);
        }

        std::string astName = astNameBase + "_AST";

        mapTreeVariable(el, astName);
        if (grammar->isTreeWalker())
            println(labeledElementASTType + " " + astName + "_in = " + labeledElementASTInit + ";");

        if (doNoGuessTest) {
            println("if ( inputState->guessing == 0 ) {");
            tabs++;
        }

        if (el->getLabel()) {
            if (ga)
                println(astName + " = " + getASTCreateString(ga, elementRef) + ";");
            else
                println(astName + " = " + getASTCreateString(elementRef) + ";");
        }

        if (!el->getLabel() && needASTDecl) {
            elementRef = lt1Value;
            if (ga)
                println(astName + " = " + getASTCreateString(ga, elementRef) + ";");
            else
                println(astName + " = " + getASTCreateString(elementRef) + ";");
            if (grammar->isTreeWalker())
                println(astName + "_in = " + elementRef + ";");
        }

        if (genAST) {
            switch (el->getAutoGenType()) {
            case AUTO_GEN_NONE:
                if (usingCustomAST || (ga && ga->getASTNodeType()))
                    println("astFactory->addASTChild(currentAST, " + namespaceAntlr + "RefAST(" + astName + "));");
                else
                    println("astFactory->addASTChild(currentAST, " + astName + ");");
                break;
            case AUTO_GEN_CARET:
                if (usingCustomAST || (ga && ga->getASTNodeType()))
                    println("astFactory->makeASTRoot(currentAST, " + namespaceAntlr + "RefAST(" + astName + "));");
                else
                    println("astFactory->makeASTRoot(currentAST, " + astName + ");");
                break;
            default:
                break;
            }
        }
        if (doNoGuessTest) {
            tabs--;
            println("}");
        }
    }
}

void CppCodeGenerator::genErrorCatchForElement(AlternativeElement* el) {
    if (!el->getLabel())
        return;
    std::string r = el->enclosingRuleName;
    if (grammar->isLexer())
        r = encodeLexerRuleName(el->enclosingRuleName);
    RuleSymbol* rs = grammar->getSymbol(r);
    if (!rs)
        antlrTool->panic("Enclosing rule not found!");
    ExceptionSpec* ex = rs->block->findExceptionSpec(*el->getLabel());
    if (ex) {
        tabs--;
        println("}");
        genErrorHandler(ex);
    }
}

void CppCodeGenerator::genErrorHandler(ExceptionSpec* ex) {
    for (auto handler : ex->handlers) {
        println("catch (" + handler->exceptionTypeAndName->text + ") {");
        tabs++;
        if (grammar->hasSyntacticPredicate) {
            println("if (inputState->guessing==0) {");
            tabs++;
        }

        ActionTransInfo tInfo;
        genLineNo(handler->action);
        printAction(processActionForSpecialSymbols(handler->action->text, handler->action->line, currentRule, &tInfo));
        genLineNo2();

        if (grammar->hasSyntacticPredicate) {
            tabs--;
            println("} else {");
            tabs++;
            println("throw;");
            tabs--;
            println("}");
        }
        tabs--;
        println("}");
    }
}

void CppCodeGenerator::genErrorTryForElement(AlternativeElement* el) {
    if (!el->getLabel())
        return;
    std::string r = el->enclosingRuleName;
    if (grammar->isLexer())
        r = encodeLexerRuleName(el->enclosingRuleName);
    RuleSymbol* rs = grammar->getSymbol(r);
    if (!rs)
        antlrTool->panic("Enclosing rule not found!");
    ExceptionSpec* ex = rs->block->findExceptionSpec(*el->getLabel());
    if (ex) {
        println("try { // for error handling");
        tabs++;
    }
}

void CppCodeGenerator::genHeader(const std::string& fileName) {
    println("/* $ANTLR " + Tool::version + ": " + "\"" + antlrTool->fileMinusPath(antlrTool->grammarFile) + "\"" + " -> " + "\"" + fileName + "\"$ */");
}

void CppCodeGenerator::genInclude(LexerGrammar* g) {
    outputFile = grammar->getClassName() + ".hpp";
    outputLine = 1;
    openOutput(outputFile);

    genAST = false;
    saveText = true;

    tabs = 0;

    println("#ifndef INC_" + grammar->getClassName() + "_hpp_");
    println("#define INC_" + grammar->getClassName() + "_hpp_");
    println("");

    printHeaderAction(preIncludeHpp);

    println("#include \"ANTLRRuntime.hpp\"");

    genHeader(outputFile);

    println("#include \"" + grammar->tokenManager->getName() + TokenTypesFileSuffix + ".hpp\"");

    std::string sup;
    if (grammar->superClass) {
        sup = *grammar->superClass;
        println("\n// Include correct superclass header with a header statement for example:");
        println("// header \"post_include_hpp\" {");
        println("// #include \"" + sup + ".hpp\"");
        println("// }");
        println("// Or....");
        println("// header {");
        println("// #include \"" + sup + ".hpp\"");
        println("// }\n");
    } else {
        sup = grammar->getSuperClass();
        if (lastIndexOf(sup, '.') != -1)
            sup = sup.substr(static_cast<std::size_t>(lastIndexOf(sup, '.') + 1));
        sup = namespaceAntlr + sup;
    }

    printHeaderAction(postIncludeHpp);

    if (nameSpace)
        nameSpace->emitDeclarations(*currentOutput);

    printHeaderAction("");

    if (grammar->comment)
        _println(*grammar->comment);

    print("class CUSTOM_API " + grammar->getClassName() + " : public " + sup);
    println(", public " + grammar->tokenManager->getName() + TokenTypesFileSuffix);

    TokenRef tsuffix = grammar->getOption("classHeaderSuffix");
    if (tsuffix)
        print(", " + stripFrontBack(tsuffix->text, "\"", "\""));
    println("{");

    if (grammar->classMemberAction) {
        genLineNo(grammar->classMemberAction);
        print(processActionForSpecialSymbols(grammar->classMemberAction->text, grammar->classMemberAction->line, currentRule, nullptr));
        genLineNo2();
    }

    tabs = 0;
    println("private:");
    tabs = 1;
    println("void initLiterals();");

    tabs = 0;
    println("public:");
    tabs = 1;
    println("bool getCaseSensitiveLiterals() const");
    println("{");
    tabs++;
    println(std::string("return ") + (g->caseSensitiveLiterals ? "true" : "false") + ";");
    tabs--;
    println("}");

    tabs = 0;
    println("public:");
    tabs = 1;

    if (noConstructors) {
        tabs = 0;
        println("#if 0");
        println("// constructor creation turned of with 'noConstructor' option");
        tabs = 1;
    }

    println(grammar->getClassName() + "(" + namespaceStd + "istream& in);");
    println(grammar->getClassName() + "(" + namespaceAntlr + "InputBuffer& ib);");
    println(grammar->getClassName() + "(const " + namespaceAntlr + "LexerSharedInputState& state);");
    if (noConstructors) {
        tabs = 0;
        println("// constructor creation turned of with 'noConstructor' option");
        println("#endif");
        tabs = 1;
    }

    println(namespaceAntlr + "RefToken nextToken();");

    for (std::size_t i = 0; i < grammar->rules.size(); i++) {
        RuleSymbol* sym = grammar->rules[i];
        if (sym->getId() != "mnextToken")
            genRuleHeader(sym, false);
        exitIfError();
    }

    tabs = 0;
    println("private:");
    tabs = 1;

    genBitsetsHeader(bitsetsUsed, static_cast<LexerGrammar*>(grammar)->charVocabulary.size());

    tabs = 0;
    println("};");
    println("");
    if (nameSpace)
        nameSpace->emitClosures(*currentOutput);

    println("#endif /*INC_" + grammar->getClassName() + "_hpp_*/");

    closeOutput();
}

void CppCodeGenerator::genInclude(ParserGrammar*) {
    outputFile = grammar->getClassName() + ".hpp";
    outputLine = 1;
    openOutput(outputFile);

    genAST = grammar->buildAST;

    tabs = 0;

    const std::string& cn = grammar->getClassName();
    println("#ifndef INC_" + cn + "_hpp_");
    println("#define INC_" + cn + "_hpp_");
    println("");
    printHeaderAction(preIncludeHpp);
    println("#include \"ANTLRRuntime.hpp\"");

    genHeader(outputFile);

    println("#include \"" + grammar->tokenManager->getName() + TokenTypesFileSuffix + ".hpp\"");

    std::string sup;
    if (grammar->superClass) {
        sup = *grammar->superClass;
        println("\n// Include correct superclass header with a header statement for example:");
        println("// header \"post_include_hpp\" {");
        println("// #include \"" + sup + ".hpp\"");
        println("// }");
        println("// Or....");
        println("// header {");
        println("// #include \"" + sup + ".hpp\"");
        println("// }\n");
    } else {
        sup = grammar->getSuperClass();
        if (lastIndexOf(sup, '.') != -1)
            sup = sup.substr(static_cast<std::size_t>(lastIndexOf(sup, '.') + 1));
        sup = namespaceAntlr + sup;
    }
    println("");

    printHeaderAction(postIncludeHpp);

    if (nameSpace)
        nameSpace->emitDeclarations(*currentOutput);

    printHeaderAction("");

    if (grammar->comment)
        _println(*grammar->comment);

    print("class CUSTOM_API " + cn + " : public " + sup);
    println(", public " + grammar->tokenManager->getName() + TokenTypesFileSuffix);

    TokenRef tsuffix = grammar->getOption("classHeaderSuffix");
    if (tsuffix)
        print(", " + stripFrontBack(tsuffix->text, "\"", "\""));
    println("{");

    if (grammar->classMemberAction) {
        genLineNo(grammar->classMemberAction->line);
        print(processActionForSpecialSymbols(grammar->classMemberAction->text, grammar->classMemberAction->line, currentRule, nullptr));
        genLineNo2();
    }
    tabs = 0;
    if (noConstructors) {
        println("#if 0");
        println("// constructor creation turned of with 'noConstructor' option");
    }
    println("protected:");
    tabs = 1;
    println(cn + "(" + namespaceAntlr + "TokenBuffer& tokenBuf, int k);");
    tabs = 0;
    println("public:");
    tabs = 1;
    println(cn + "(" + namespaceAntlr + "TokenBuffer& tokenBuf);");

    tabs = 0;
    println("protected:");
    tabs = 1;
    println(cn + "(" + namespaceAntlr + "TokenStream& lexer, int k);");
    tabs = 0;
    println("public:");
    tabs = 1;
    println(cn + "(" + namespaceAntlr + "TokenStream& lexer);");

    println(cn + "(const " + namespaceAntlr + "ParserSharedInputState& state);");
    if (noConstructors) {
        tabs = 0;
        println("// constructor creation turned of with 'noConstructor' option");
        println("#endif");
        tabs = 1;
    }

    println("int getNumTokens() const");
    println("{");
    tabs++;
    println("return " + cn + "::NUM_TOKENS;");
    tabs--;
    println("}");
    println("const char* getTokenName( int type ) const");
    println("{");
    tabs++;
    println("if( type > getNumTokens() ) return 0;");
    println("return " + cn + "::tokenNames[type];");
    tabs--;
    println("}");
    println("const char* const* getTokenNames() const");
    println("{");
    tabs++;
    println("return " + cn + "::tokenNames;");
    tabs--;
    println("}");

    for (std::size_t i = 0; i < grammar->rules.size(); i++) {
        RuleSymbol* rs = grammar->rules[i];
        genRuleHeader(rs, rs->references.empty());
        exitIfError();
    }

    tabs = 0;
    println("private:");
    tabs = 1;

    println("static const char* tokenNames[];");
    _println("#ifndef NO_STATIC_CONSTS");
    println("static const int NUM_TOKENS = " + std::to_string(grammar->tokenManager->vocabularySize()) + ";");
    _println("#else");
    println("enum {");
    println("\tNUM_TOKENS = " + std::to_string(grammar->tokenManager->vocabularySize()));
    println("};");
    _println("#endif");

    genBitsetsHeader(bitsetsUsed, grammar->tokenManager->maxTokenType());

    tabs = 0;
    println("};");
    println("");
    if (nameSpace)
        nameSpace->emitClosures(*currentOutput);

    println("#endif /*INC_" + cn + "_hpp_*/");

    closeOutput();
}

void CppCodeGenerator::genInclude(TreeWalkerGrammar*) {
    outputFile = grammar->getClassName() + ".hpp";
    outputLine = 1;
    openOutput(outputFile);

    genAST = grammar->buildAST;
    tabs = 0;

    const std::string& cn = grammar->getClassName();
    println("#ifndef INC_" + cn + "_hpp_");
    println("#define INC_" + cn + "_hpp_");
    println("");
    printHeaderAction(preIncludeHpp);
    println("#include <antlr/config.hpp>");
    println("#include \"" + grammar->tokenManager->getName() + TokenTypesFileSuffix + ".hpp\"");

    genHeader(outputFile);

    std::string sup;
    if (grammar->superClass) {
        sup = *grammar->superClass;
        println("\n// Include correct superclass header with a header statement for example:");
        println("// header \"post_include_hpp\" {");
        println("// #include \"" + sup + ".hpp\"");
        println("// }");
        println("// Or....");
        println("// header {");
        println("// #include \"" + sup + ".hpp\"");
        println("// }\n");
    } else {
        sup = grammar->getSuperClass();
        if (lastIndexOf(sup, '.') != -1)
            sup = sup.substr(static_cast<std::size_t>(lastIndexOf(sup, '.') + 1));
        println("#include <antlr/" + sup + ".hpp>");
        sup = namespaceAntlr + sup;
    }
    println("");

    printHeaderAction(postIncludeHpp);

    if (nameSpace)
        nameSpace->emitDeclarations(*currentOutput);

    printHeaderAction("");

    if (grammar->comment)
        _println(*grammar->comment);

    print("class CUSTOM_API " + cn + " : public " + sup);
    println(", public " + grammar->tokenManager->getName() + TokenTypesFileSuffix);

    TokenRef tsuffix = grammar->getOption("classHeaderSuffix");
    if (tsuffix)
        print(", " + stripFrontBack(tsuffix->text, "\"", "\""));
    println("{");

    if (grammar->classMemberAction) {
        genLineNo(grammar->classMemberAction->line);
        print(processActionForSpecialSymbols(grammar->classMemberAction->text, grammar->classMemberAction->line, currentRule, nullptr));
        genLineNo2();
    }

    tabs = 0;
    println("public:");

    if (noConstructors) {
        println("#if 0");
        println("// constructor creation turned of with 'noConstructor' option");
    }
    tabs = 1;
    println(cn + "();");
    if (noConstructors) {
        tabs = 0;
        println("#endif");
        tabs = 1;
    }

    println("static void initializeASTFactory( " + namespaceAntlr + "ASTFactory& factory );");

    println("int getNumTokens() const");
    println("{");
    tabs++;
    println("return " + cn + "::NUM_TOKENS;");
    tabs--;
    println("}");
    println("const char* getTokenName( int type ) const");
    println("{");
    tabs++;
    println("if( type > getNumTokens() ) return 0;");
    println("return " + cn + "::tokenNames[type];");
    tabs--;
    println("}");
    println("const char* const* getTokenNames() const");
    println("{");
    tabs++;
    println("return " + cn + "::tokenNames;");
    tabs--;
    println("}");

    for (std::size_t i = 0; i < grammar->rules.size(); i++) {
        RuleSymbol* rs = grammar->rules[i];
        genRuleHeader(rs, rs->references.empty());
        exitIfError();
    }
    tabs = 0;
    println("public:");
    tabs = 1;
    println(namespaceAntlr + "RefAST getAST()");
    println("{");
    tabs++;
    if (usingCustomAST)
        println("return " + namespaceAntlr + "RefAST(returnAST);");
    else
        println("return returnAST;");
    tabs--;
    println("}");
    println("");

    tabs = 0;
    println("protected:");
    tabs = 1;
    println(labeledElementASTType + " returnAST;");
    println(labeledElementASTType + " _retTree;");

    tabs = 0;
    println("private:");
    tabs = 1;

    println("static const char* tokenNames[];");
    _println("#ifndef NO_STATIC_CONSTS");
    println("static const int NUM_TOKENS = " + std::to_string(grammar->tokenManager->vocabularySize()) + ";");
    _println("#else");
    println("enum {");
    println("\tNUM_TOKENS = " + std::to_string(grammar->tokenManager->vocabularySize()));
    println("};");
    _println("#endif");

    genBitsetsHeader(bitsetsUsed, grammar->tokenManager->maxTokenType());

    tabs = 0;
    println("};");
    println("");
    if (nameSpace)
        nameSpace->emitClosures(*currentOutput);

    println("#endif /*INC_" + cn + "_hpp_*/");

    closeOutput();
}

void CppCodeGenerator::genASTDeclaration(AlternativeElement* el) {
    genASTDeclaration(el, labeledElementASTType);
}

void CppCodeGenerator::genASTDeclaration(AlternativeElement* el, const std::string& nodeType) {
    genASTDeclaration(el, *el->getLabel(), nodeType);
}

void CppCodeGenerator::genASTDeclaration(AlternativeElement* el, const std::string& varName, const std::string& nodeType) {
    if (declaredASTVariables.count(el))
        return;

    std::string init = labeledElementASTInit;

    auto ga = dynamic_cast<GrammarAtom*>(el);
    if (ga && ga->getASTNodeType())
        init = "Ref" + *ga->getASTNodeType() + "(" + labeledElementASTInit + ")";

    println(nodeType + " " + varName + "_AST = " + init + ";");

    declaredASTVariables.insert(el);
}

void CppCodeGenerator::genLiteralsTest() {
    println("_ttype = testLiteralsTable(_ttype);");
}

void CppCodeGenerator::genLiteralsTestForPartialToken() {
    println("_ttype = testLiteralsTable(text.substr(_begin, text.length()-_begin),_ttype);");
}

void CppCodeGenerator::genMatch(GrammarAtom* atom) {
    if (dynamic_cast<StringLiteralElement*>(atom)) {
        if (grammar->isLexer())
            genMatchUsingAtomText(atom);
        else
            genMatchUsingAtomTokenType(atom);
    } else if (dynamic_cast<CharLiteralElement*>(atom)) {
        antlrTool->error("cannot ref character literals in grammar: " + atom->atomText);
    } else if (dynamic_cast<TokenRefElement*>(atom)) {
        genMatchUsingAtomTokenType(atom);
    } else if (auto wc = dynamic_cast<WildcardElement*>(atom)) {
        gen(wc);
    }
}

void CppCodeGenerator::genMatchUsingAtomText(GrammarAtom* atom) {
    std::string astArgs;
    if (grammar->isTreeWalker()) {
        if (usingCustomAST)
            astArgs = namespaceAntlr + "RefAST" + "(_t),";
        else
            astArgs = "_t,";
    }

    if (grammar->isLexer() && (!saveText || atom->getAutoGenType() == AUTO_GEN_BANG))
        println("_saveIndex = text.length();");

    print(atom->not_ ? "matchNot(" : "match(");
    _print(astArgs);

    if (atom->atomText == "EOF")
        _print(namespaceAntlr + "Token::EOF_TYPE");
    else if (grammar->isLexer())
        _print(convertJavaToCppString(atom->atomText, false));
    else
        _print(atom->atomText);

    _println(");");

    if (grammar->isLexer() && (!saveText || atom->getAutoGenType() == AUTO_GEN_BANG))
        println("text.erase(_saveIndex);");
}

void CppCodeGenerator::genMatchUsingAtomTokenType(GrammarAtom* atom) {
    std::string astArgs;
    if (grammar->isTreeWalker()) {
        if (usingCustomAST)
            astArgs = namespaceAntlr + "RefAST" + "(_t),";
        else
            astArgs = "_t,";
    }

    std::string s = astArgs + getValueString(atom->getType());
    println((atom->not_ ? "matchNot(" : "match(") + s + ");");
}

void CppCodeGenerator::genNextToken() {
    bool hasPublicRules = false;
    for (auto rs : grammar->rules) {
        if (rs->isDefined() && rs->access == "public") {
            hasPublicRules = true;
            break;
        }
    }
    const std::string& cn = grammar->getClassName();
    if (!hasPublicRules) {
        println("");
        println(namespaceAntlr + "RefToken " + cn + "::nextToken() { return " + namespaceAntlr + "RefToken(new " + namespaceAntlr +
                "CommonToken(" + namespaceAntlr + "Token::EOF_TYPE, \"\")); }");
        println("");
        return;
    }

    RuleBlock* nextTokenBlk = MakeGrammar::createNextTokenRule(grammar, grammar->rules, "nextToken");
    auto nextTokenRs = new RuleSymbol("mnextToken");
    nextTokenRs->setDefined();
    nextTokenRs->setBlock(nextTokenBlk);
    nextTokenRs->access = "private";
    grammar->define(nextTokenRs);
    grammar->theLLkAnalyzer->deterministic(static_cast<AlternativeBlock*>(nextTokenBlk));

    auto lg = static_cast<LexerGrammar*>(grammar);
    OStr filterRule;
    if (lg->filterMode)
        filterRule = lg->filterRule;

    println("");
    println(namespaceAntlr + "RefToken " + cn + "::nextToken()");
    println("{");
    tabs++;
    println(namespaceAntlr + "RefToken theRetToken;");
    println("for (;;) {");
    tabs++;
    println(namespaceAntlr + "RefToken theRetToken;");
    println("int _ttype = " + namespaceAntlr + "Token::INVALID_TYPE;");
    if (lg->filterMode) {
        println("setCommitToPath(false);");
        if (filterRule) {
            if (!grammar->isDefined(encodeLexerRuleName(*filterRule))) {
                antlrTool->error("Filter rule " + *filterRule + " does not exist in this lexer");
            } else {
                RuleSymbol* rs = grammar->getSymbol(encodeLexerRuleName(*filterRule));
                if (!rs->isDefined())
                    antlrTool->error("Filter rule " + *filterRule + " does not exist in this lexer");
                else if (rs->access == "public")
                    antlrTool->error("Filter rule " + *filterRule + " must be protected");
            }
            println("int _m;");
            println("_m = mark();");
        }
    }
    println("resetText();");

    println("try {   // for lexical and char stream error handling");
    tabs++;

    for (int i = 0; i < nextTokenBlk->numAlternatives(); i++) {
        Alternative* a = nextTokenBlk->getAlternativeAt(i);
        if (a->cache[1]->containsEpsilon())
            antlrTool->warning("found optional path in nextToken()");
    }

    const std::string newline = "\n";
    CppBlockFinishingInfo howToFinish = genCommonBlock(nextTokenBlk, false);
    std::string errFinish = "if (LA(1)==EOF_CHAR)" + newline + "\t\t\t\t{" + newline + "\t\t\t\t\tuponEOF();" + newline +
                            "\t\t\t\t\t_returnToken = makeToken(" + namespaceAntlr + "Token::EOF_TYPE);" + newline + "\t\t\t\t}";
    errFinish += newline + "\t\t\t\t";
    if (lg->filterMode) {
        if (!filterRule) {
            errFinish += "else {consume(); goto tryAgain;}";
        } else {
            errFinish += "else {" + newline + "\t\t\t\t\tcommit();" + newline + "\t\t\t\t\ttry {m" + *filterRule + "(false);}" + newline +
                         "\t\t\t\t\tcatch(" + namespaceAntlr + "RecognitionException& e) {" + newline + "\t\t\t\t\t	// catastrophic failure" +
                         newline + "\t\t\t\t\t	reportError(e);" + newline + "\t\t\t\t\t	consume();" + newline + "\t\t\t\t\t}" + newline +
                         "\t\t\t\t\tgoto tryAgain;" + newline + "\t\t\t\t}";
        }
    } else {
        errFinish += "else {" + throwNoViable + "}";
    }
    genBlockFinish(howToFinish, errFinish);

    if (lg->filterMode && filterRule)
        println("commit();");

    println("if ( !_returnToken )" + newline + "\t\t\t\tgoto tryAgain; // found SKIP token" + newline);
    println("_ttype = _returnToken->getType();");
    if (lg->getTestLiterals())
        genLiteralsTest();

    println("_returnToken->setType(_ttype);");
    println("return _returnToken;");

    tabs--;
    println("}");
    println("catch (" + namespaceAntlr + "RecognitionException& e) {");
    tabs++;
    if (lg->filterMode) {
        if (!filterRule) {
            println("if ( !getCommitToPath() ) {");
            tabs++;
            println("consume();");
            println("goto tryAgain;");
            tabs--;
            println("}");
        } else {
            println("if ( !getCommitToPath() ) {");
            tabs++;
            println("rewind(_m);");
            println("resetText();");
            println("try {m" + *filterRule + "(false);}");
            println("catch(" + namespaceAntlr + "RecognitionException& ee) {");
            println("	// horrendous failure: error in filter rule");
            println("	reportError(ee);");
            println("	consume();");
            println("}");
            tabs--;
            println("}");
            println("else");
        }
    }
    if (nextTokenBlk->getDefaultErrorHandler()) {
        println("{");
        tabs++;
        println("reportError(e);");
        println("consume();");
        tabs--;
        println("}");
    } else {
        tabs++;
        println("throw " + namespaceAntlr + "TokenStreamRecognitionException(e);");
        tabs--;
    }

    tabs--;
    println("}");
    println("catch (" + namespaceAntlr + "CharStreamIOException& csie) {");
    println("\tthrow " + namespaceAntlr + "TokenStreamIOException(csie.io);");
    println("}");
    println("catch (" + namespaceAntlr + "CharStreamException& cse) {");
    println("\tthrow " + namespaceAntlr + "TokenStreamException(cse.getMessage());");
    println("}");

    _println("tryAgain:;");
    tabs--;
    println("}");

    tabs--;
    println("}");
    println("");
}

void CppCodeGenerator::genRule(RuleSymbol* s, bool, int, const std::string& prefix) {
    if (!s->isDefined()) {
        antlrTool->error("undefined rule: " + s->getId());
        return;
    }

    RuleBlock* rblk = s->getBlock();

    currentRule = rblk;
    currentASTResult = s->getId();

    declaredASTVariables.clear();

    bool savegenAST = genAST;
    genAST = genAST && rblk->getAutoGen();

    saveText = rblk->getAutoGen();

    if (s->comment)
        _println(*s->comment);

    if (rblk->returnAction)
        _print(extractTypeOfAction(*rblk->returnAction, rblk->getLine(), rblk->getColumn()) + " ");
    else
        _print("void ");

    _print(prefix + s->getId() + "(");

    _print(commonExtraParams);
    if (!commonExtraParams.empty() && rblk->argAction)
        _print(",");

    if (rblk->argAction) {
        _println("");
        tabs++;

        std::string oldarg = *rblk->argAction;
        std::string newarg;
        std::string comma;
        int eqpos = indexOf(oldarg, '=');
        if (eqpos != -1) {
            int cmpos = 0;
            while (cmpos != -1 && eqpos != -1) {
                newarg = newarg + comma + trim(oldarg.substr(0, static_cast<std::size_t>(eqpos)));
                comma = ", ";
                cmpos = indexOf(oldarg, ',', eqpos);
                if (cmpos != -1) {
                    oldarg = trim(oldarg.substr(static_cast<std::size_t>(cmpos + 1)));
                    eqpos = indexOf(oldarg, '=');
                    if (eqpos == -1)
                        newarg = newarg + comma + oldarg;
                }
            }
        } else {
            newarg = oldarg;
        }

        println(newarg);

        tabs--;
        print(") ");
    } else {
        _print(") ");
    }
    _println("{");
    tabs++;

    if (grammar->traceRules) {
        if (grammar->isTreeWalker()) {
            if (usingCustomAST)
                println("Tracer traceInOut(this,\"" + s->getId() + "\"," + namespaceAntlr + "RefAST" + "(_t));");
            else
                println("Tracer traceInOut(this,\"" + s->getId() + "\",_t);");
        } else {
            println("Tracer traceInOut(this, \"" + s->getId() + "\");");
        }
    }

    if (rblk->returnAction) {
        genLineNo(rblk);
        println(*rblk->returnAction + ";");
        genLineNo2();
    }

    if (!commonLocalVars.empty())
        println(commonLocalVars);

    if (grammar->isLexer()) {
        if (s->getId() == "mEOF")
            println("_ttype = " + namespaceAntlr + "Token::EOF_TYPE;");
        else
            println("_ttype = " + s->getId().substr(1) + ";");
        println(namespaceStd + "string::size_type _saveIndex;");
    }

    if (grammar->isTreeWalker())
        println(labeledElementASTType + " " + s->getId() + "_AST_in = (_t == " + labeledElementASTType + "(ASTNULL)) ? " + labeledElementASTInit + " : _t;");
    if (grammar->buildAST) {
        println("returnAST = " + labeledElementASTInit + ";");
        println(namespaceAntlr + "ASTPair currentAST;");
        println(labeledElementASTType + " " + s->getId() + "_AST = " + labeledElementASTInit + ";");
    }

    genBlockPreamble(rblk);
    genBlockInitAction(rblk);
    println("");

    ExceptionSpec* unlabeledUserSpec = rblk->findExceptionSpec(std::string());

    if (unlabeledUserSpec || rblk->getDefaultErrorHandler()) {
        println("try {      // for error handling");
        tabs++;
    }

    if (rblk->numAlternatives() == 1) {
        Alternative* alt = rblk->getAlternativeAt(0);
        if (alt->semPred)
            genSemPred(*alt->semPred, currentRule->line);
        if (alt->synPred)
            antlrTool->warning("Syntactic predicate ignored for single alternative", grammar->getFilename(), alt->synPred->getLine(), alt->synPred->getColumn());
        genAlt(alt, rblk);
    } else {
        grammar->theLLkAnalyzer->deterministic(static_cast<AlternativeBlock*>(rblk));

        CppBlockFinishingInfo howToFinish = genCommonBlock(rblk, false);
        genBlockFinish(howToFinish, throwNoViable);
    }

    if (unlabeledUserSpec || rblk->getDefaultErrorHandler()) {
        tabs--;
        println("}");
    }

    if (unlabeledUserSpec) {
        genErrorHandler(unlabeledUserSpec);
    } else if (rblk->getDefaultErrorHandler()) {
        println("catch (" + exceptionThrown + "& ex) {");
        tabs++;
        if (grammar->hasSyntacticPredicate) {
            println("if( inputState->guessing == 0 ) {");
            tabs++;
        }
        println("reportError(ex);");
        if (!grammar->isTreeWalker()) {
            Lookahead follow = grammar->theLLkAnalyzer->FOLLOW(1, rblk->endNode);
            std::string followSetName = getBitsetName(markBitsetForGen(follow.fset));
            println("recover(ex," + followSetName + ");");
        } else {
            println("if ( _t != " + labeledElementASTInit + " )");
            tabs++;
            println("_t = _t->getNextSibling();");
            tabs--;
        }
        if (grammar->hasSyntacticPredicate) {
            tabs--;
            println("} else {");
            tabs++;
            println("throw;");
            tabs--;
            println("}");
        }
        tabs--;
        println("}");
    }

    if (grammar->buildAST)
        println("returnAST = " + s->getId() + "_AST;");

    if (grammar->isTreeWalker())
        println("_retTree = _t;");

    if (rblk->getTestLiterals()) {
        if (s->access == "protected")
            genLiteralsTestForPartialToken();
        else
            genLiteralsTest();
    }

    if (grammar->isLexer()) {
        println("if ( _createToken && _token==" + namespaceAntlr + "nullToken && _ttype!=" + namespaceAntlr + "Token::SKIP ) {");
        println("   _token = makeToken(_ttype);");
        println("   _token->setText(text.substr(_begin, text.length()-_begin));");
        println("}");
        println("_returnToken = _token;");
        println("_saveIndex=0;");
    }

    if (rblk->returnAction)
        println("return " + extractIdOfAction(*rblk->returnAction, rblk->getLine(), rblk->getColumn()) + ";");

    tabs--;
    println("}");
    println("");

    genAST = savegenAST;
}

void CppCodeGenerator::genRuleHeader(RuleSymbol* s, bool) {
    tabs = 1;
    if (!s->isDefined()) {
        antlrTool->error("undefined rule: " + s->getId());
        return;
    }

    RuleBlock* rblk = s->getBlock();
    currentRule = rblk;
    currentASTResult = s->getId();

    bool savegenAST = genAST;
    genAST = genAST && rblk->getAutoGen();

    saveText = rblk->getAutoGen();

    print(s->access + ": ");

    if (rblk->returnAction)
        _print(extractTypeOfAction(*rblk->returnAction, rblk->getLine(), rblk->getColumn()) + " ");
    else
        _print("void ");

    _print(s->getId() + "(");

    _print(commonExtraParams);
    if (!commonExtraParams.empty() && rblk->argAction)
        _print(",");

    if (rblk->argAction) {
        _println("");
        tabs++;
        println(*rblk->argAction);
        tabs--;
        print(")");
    } else {
        _print(")");
    }
    _println(";");

    tabs--;

    genAST = savegenAST;
}

void CppCodeGenerator::GenRuleInvocation(RuleRefElement* rr) {
    _print(rr->targetRule + "(");

    if (grammar->isLexer()) {
        if (rr->getLabel())
            _print("true");
        else
            _print("false");
        if (!commonExtraArgs.empty() || rr->args)
            _print(",");
    }

    _print(commonExtraArgs);
    if (!commonExtraArgs.empty() && rr->args)
        _print(",");

    RuleSymbol* rs = grammar->getSymbol(rr->targetRule);
    if (rr->args) {
        ActionTransInfo tInfo;
        OStr args = processActionForSpecialSymbols(*rr->args, rr->line, currentRule, &tInfo);
        if (tInfo.assignToRoot || tInfo.refRuleRoot)
            antlrTool->error("Arguments of rule reference '" + rr->targetRule + "' cannot set or ref #" + currentRule->getRuleName() +
                             " on line " + std::to_string(rr->getLine()));
        _print(args);

        if (!rs->block->argAction)
            antlrTool->warning("Rule '" + rr->targetRule + "' accepts no arguments", grammar->getFilename(), rr->getLine(), rr->getColumn());
    }
    _println(");");

    if (grammar->isTreeWalker())
        println("_t = _retTree;");
}

void CppCodeGenerator::genSemPred(std::string predIn, int line) {
    ActionTransInfo tInfo;
    OStr pred = processActionForSpecialSymbols(predIn, line, currentRule, &tInfo);
    std::string p = pred ? *pred : std::string();
    std::string escapedPred = escapeString(p);

    println("if (!(" + (pred ? p : std::string("null")) + "))");
    tabs++;
    println("throw " + namespaceAntlr + "SemanticException(\"" + escapedPred + "\");");
    tabs--;
}

void CppCodeGenerator::genSynPred(SynPredBlock* blk, const std::string& lookaheadExpr) {
    std::string id = std::to_string(blk->ID);
    println("bool synPredMatched" + id + " = false;");

    if (grammar->isTreeWalker()) {
        println("if (_t == " + labeledElementASTInit + " )");
        tabs++;
        println("_t = ASTNULL;");
        tabs--;
    }

    println("if (" + lookaheadExpr + ") {");
    tabs++;

    if (grammar->isTreeWalker())
        println(labeledElementType + " __t" + id + " = _t;");
    else
        println("int _m" + id + " = mark();");

    println("synPredMatched" + id + " = true;");
    println("inputState->guessing++;");

    syntacticPredLevel++;
    println("try {");
    tabs++;
    gen(static_cast<AlternativeBlock*>(blk));
    tabs--;
    println("}");
    println("catch (" + exceptionThrown + "& pe) {");
    tabs++;
    println("synPredMatched" + id + " = false;");
    tabs--;
    println("}");

    if (grammar->isTreeWalker())
        println("_t = __t" + id + ";");
    else
        println("rewind(_m" + id + ");");

    println("inputState->guessing--;");

    syntacticPredLevel--;
    tabs--;

    println("}");

    println("if ( synPredMatched" + id + " ) {");
}

void CppCodeGenerator::genTokenStrings(const std::string& prefix) {
    println("const char* " + prefix + "tokenNames[] = {");
    tabs++;

    auto& v = grammar->tokenManager->getVocabulary();
    for (std::size_t i = 0; i < v.size(); i++) {
        std::string s = v[i] ? *v[i] : "<" + std::to_string(i) + ">";
        if (!startsWith(s, "\"") && !startsWith(s, "<")) {
            TokenSymbol* ts = grammar->tokenManager->getTokenSymbol(s);
            if (ts && ts->paraphrase)
                s = stripFrontBack(*ts->paraphrase, "\"", "\"");
        }
        print(literalString(s));
        _println(",");
    }
    println("0");

    tabs--;
    println("};");
}

void CppCodeGenerator::genTokenTypes(TokenManager* tm) {
    outputFile = tm->getName() + TokenTypesFileSuffix + ".hpp";
    outputLine = 1;
    openOutput(outputFile);

    tabs = 0;

    println("#ifndef INC_" + tm->getName() + TokenTypesFileSuffix + "_hpp_");
    println("#define INC_" + tm->getName() + TokenTypesFileSuffix + "_hpp_");
    println("");

    if (nameSpace)
        nameSpace->emitDeclarations(*currentOutput);

    genHeader(outputFile);

    println("");
    println("#ifndef CUSTOM_API");
    println("# define CUSTOM_API");
    println("#endif");
    println("");
    println("#ifdef __cplusplus");
    println("struct CUSTOM_API " + tm->getName() + TokenTypesFileSuffix + " {");
    println("#endif");
    tabs++;
    println("enum {");
    tabs++;

    auto& v = tm->getVocabulary();

    println("EOF_ = " + std::to_string(TokenType::EOF_TYPE) + ",");

    for (int i = TokenType::MIN_USER_TYPE; i < static_cast<int>(v.size()); i++) {
        const OStr& s = v[static_cast<std::size_t>(i)];
        if (!s)
            continue;
        if (startsWith(*s, "\"")) {
            auto sl = dynamic_cast<StringLiteralSymbol*>(tm->getTokenSymbol(*s));
            if (!sl) {
                antlrTool->panic("String literal " + *s + " not in symbol table");
            } else if (sl->label) {
                println(*sl->label + " = " + std::to_string(i) + ",");
            } else {
                OStr mangledName = mangleLiteral(*s);
                if (mangledName) {
                    println(*mangledName + " = " + std::to_string(i) + ",");
                    sl->label = mangledName;
                } else {
                    println("// " + *s + " = " + std::to_string(i));
                }
            }
        } else if (!startsWith(*s, "<")) {
            println(*s + " = " + std::to_string(i) + ",");
        }
    }

    println("NULL_TREE_LOOKAHEAD = " + std::to_string(TokenType::NULL_TREE_LOOKAHEAD));

    tabs--;
    println("};");

    tabs--;
    println("#ifdef __cplusplus");
    println("};");
    println("#endif");

    if (nameSpace)
        nameSpace->emitClosures(*currentOutput);

    println("#endif /*INC_" + tm->getName() + TokenTypesFileSuffix + "_hpp_*/");

    closeOutput();
    exitIfError();
}

std::string CppCodeGenerator::processStringForASTConstructor(const std::string& str) {
    if (usingCustomAST && (grammar->isTreeWalker() || grammar->isParser()) && !grammar->tokenManager->tokenDefined(str))
        return namespaceAntlr + "RefAST(" + str + ")";
    return str;
}

std::string CppCodeGenerator::getASTCreateString(const std::vector<std::string>& v) {
    if (v.empty())
        return "";
    std::string buf = labeledElementASTType + "(astFactory->make((new " + namespaceAntlr + "ASTArray(" + std::to_string(v.size()) + "))";
    for (auto& e : v)
        buf += "->add(" + e + ")";
    buf += "))";
    return buf;
}

std::string CppCodeGenerator::getASTCreateString(GrammarAtom* atom, const std::string& str) {
    if (atom && atom->getASTNodeType()) {
        int tt = atom->getType();
        if (static_cast<int>(astTypes.size()) <= tt)
            astTypes.resize(static_cast<std::size_t>(tt + 1));
        OStr& type = astTypes[static_cast<std::size_t>(tt)];
        if (!type) {
            type = atom->getASTNodeType();
        } else if (*atom->getASTNodeType() != *type) {
            antlrTool->warning("Attempt to redefine AST type for " + atom->getText(), grammar->getFilename(), atom->getLine(), atom->getColumn());
            antlrTool->warning(" from \"" + *type + "\" to \"" + *atom->getASTNodeType() + "\" sticking to \"" + *type + "\"",
                               grammar->getFilename(), atom->getLine(), atom->getColumn());
        } else {
            type = atom->getASTNodeType();
        }
        return "astFactory->create(" + str + ")";
    }

    bool is_constructor = false;
    if (indexOf(str, ',') != -1)
        is_constructor = grammar->tokenManager->tokenDefined(str.substr(0, static_cast<std::size_t>(indexOf(str, ','))));

    if (usingCustomAST && grammar->isTreeWalker() && !grammar->tokenManager->tokenDefined(str) && !is_constructor)
        return "astFactory->create(" + namespaceAntlr + "RefAST(" + str + "))";
    return "astFactory->create(" + str + ")";
}

std::string CppCodeGenerator::getASTCreateString(const std::string& str) {
    if (usingCustomAST)
        return labeledElementASTType + "(astFactory->create(" + namespaceAntlr + "RefAST(" + str + ")))";
    return "astFactory->create(" + str + ")";
}

std::string CppCodeGenerator::getLookaheadTestExpression(const std::vector<std::optional<Lookahead>>& look, int k) {
    std::string e = "(";
    bool first = true;
    for (int i = 1; i <= k; i++) {
        const BitSet& p = look[static_cast<std::size_t>(i)]->fset;
        if (!first)
            e += ") && (";
        first = false;

        if (look[static_cast<std::size_t>(i)]->containsEpsilon())
            e += "true";
        else
            e += getLookaheadTestTerm(i, p);
    }
    e += ")";
    return e;
}

std::string CppCodeGenerator::getLookaheadTestExpression(Alternative* alt, int maxDepth) {
    int depth = alt->lookaheadDepth;
    if (depth == NONDETERMINISTIC)
        depth = grammar->maxk;

    if (maxDepth == 0)
        return "true";

    return "(" + getLookaheadTestExpression(alt->cache, depth) + ")";
}

std::string CppCodeGenerator::getLookaheadTestTerm(int k, const BitSet& p) {
    std::string ts = lookaheadString(k);

    auto elems = p.toArray();
    if (elementsAreRange(elems))
        return getRangeExpression(k, elems);

    int degree = p.degree();
    if (degree == 0)
        return "true";

    if (degree >= bitsetTestThreshold) {
        int bitsetIdx = markBitsetForGen(p);
        return getBitsetName(bitsetIdx) + ".member(" + ts + ")";
    }

    std::string e;
    for (std::size_t i = 0; i < elems.size(); i++) {
        std::string cs = getValueString(elems[i]);
        if (i > 0)
            e += " || ";
        e += ts;
        e += " == ";
        e += cs;
    }
    return e;
}

std::string CppCodeGenerator::getRangeExpression(int k, const std::vector<int>& elems) {
    if (!elementsAreRange(elems))
        antlrTool->panic("getRangeExpression called with non-range");
    int begin = elems[0];
    int end = elems.back();
    return "(" + lookaheadString(k) + " >= " + getValueString(begin) + " && " + lookaheadString(k) + " <= " + getValueString(end) + ")";
}

std::string CppCodeGenerator::getValueString(int value) {
    if (grammar->isLexer())
        return literalChar(value);

    TokenSymbol* ts = grammar->tokenManager->getTokenSymbolAt(value);
    if (!ts)
        return std::to_string(value);
    const std::string& tId = ts->getId();
    if (auto sl = dynamic_cast<StringLiteralSymbol*>(ts)) {
        if (sl->label)
            return *sl->label;
        OStr cs = mangleLiteral(tId);
        if (!cs)
            return std::to_string(value);
        return *cs;
    }
    if (tId == "EOF")
        return namespaceAntlr + "Token::EOF_TYPE";
    return tId;
}

bool CppCodeGenerator::lookaheadIsEmpty(Alternative* alt, int maxDepth) {
    int depth = alt->lookaheadDepth;
    if (depth == NONDETERMINISTIC)
        depth = grammar->maxk;
    for (int i = 1; i <= depth && i <= maxDepth; i++)
        if (alt->cache[static_cast<std::size_t>(i)]->fset.degree() != 0)
            return false;
    return true;
}

std::string CppCodeGenerator::lookaheadString(int k) {
    if (grammar->isTreeWalker())
        return "_t->getType()";
    return "LA(" + std::to_string(k) + ")";
}

OStr CppCodeGenerator::mangleLiteral(const std::string& s) {
    std::string mangled = antlrTool->literalsPrefix;
    for (std::size_t i = 1; i + 1 < s.size(); i++) {
        int c = static_cast<unsigned char>(s[i]);
        if (!isJavaLetter(c) && c != '_')
            return std::nullopt;
        mangled += s[i];
    }
    if (antlrTool->upperCaseMangledLiterals)
        mangled = toUpperAscii(mangled);
    return mangled;
}

OStr CppCodeGenerator::mapTreeId(const std::string& idParam, ActionTransInfo* transInfo) {
    if (!currentRule)
        return idParam;

    bool in_var = false;
    std::string id = idParam;
    if (grammar->isTreeWalker()) {
        if (!grammar->buildAST)
            in_var = true;
        if (id.size() > 3 && lastIndexOf(id, "_in") == static_cast<int>(id.size()) - 3) {
            id = id.substr(0, id.size() - 3);
            in_var = true;
        }
    }

    for (auto elt : currentRule->labeledElements) {
        if (*elt->getLabel() == id)
            return in_var ? id : id + "_AST";
    }

    auto it = treeVariableMap.find(id);
    if (it != treeVariableMap.end()) {
        if (!it->second) {
            antlrTool->error("Ambiguous reference to AST element " + id + " in rule " + currentRule->getRuleName());
            return std::nullopt;
        } else if (*it->second == currentRule->getRuleName()) {
            antlrTool->error("Ambiguous reference to AST element " + id + " in rule " + currentRule->getRuleName());
            return std::nullopt;
        } else {
            return in_var ? *it->second + "_in" : *it->second;
        }
    }

    if (id == currentRule->getRuleName()) {
        std::string r = in_var ? id + "_AST_in" : id + "_AST";
        if (transInfo && !in_var)
            transInfo->refRuleRoot = r;
        return r;
    }
    return id;
}

void CppCodeGenerator::mapTreeVariable(AlternativeElement* e, const std::string& name) {
    if (auto te = dynamic_cast<TreeElement*>(e)) {
        mapTreeVariable(te->root, name);
        return;
    }

    OStr elName;
    if (!e->getLabel()) {
        if (auto tr = dynamic_cast<TokenRefElement*>(e))
            elName = tr->atomText;
        else if (auto rr = dynamic_cast<RuleRefElement*>(e))
            elName = rr->targetRule;
    }
    if (elName) {
        auto it = treeVariableMap.find(*elName);
        if (it != treeVariableMap.end())
            it->second = std::nullopt;
        else
            treeVariableMap[*elName] = name;
    }
}

OStr CppCodeGenerator::processActionForSpecialSymbols(const std::string& actionStr, int line, RuleBlock* rule, ActionTransInfo* tInfo) {
    if (actionStr.empty())
        return std::nullopt;

    if (!grammar)
        return actionStr;

    if ((grammar->buildAST && indexOf(actionStr, '#') != -1) || grammar->isTreeWalker() ||
        ((grammar->isLexer() || grammar->isParser()) && indexOf(actionStr, '$') != -1)) {
        ActionLexer lexer(actionStr, rule, this, tInfo);
        lexer.setLineOffset(line);
        lexer.filename = grammar->getFilename();
        lexer.setTool(antlrTool);

        try {
            lexer.mACTION(true);
            return lexer.returnToken->text;
        } catch (const RecognitionException& ex) {
            lexer.reportError(ex);
            return actionStr;
        }
    }
    return actionStr;
}

std::string CppCodeGenerator::fixNameSpaceOption(std::string ns) {
    ns = stripFrontBack(ns, "\"", "\"");
    if (ns.size() > 2 && ns.substr(ns.size() - 2) != "::")
        ns += "::";
    return ns;
}

void CppCodeGenerator::setupGrammarParameters(Grammar* g) {
    if (antlrTool->nameSpace)
        nameSpace = antlrTool->nameSpace;

    if (antlrTool->namespaceStd)
        namespaceStd = fixNameSpaceOption(*antlrTool->namespaceStd);

    if (antlrTool->namespaceAntlr)
        namespaceAntlr = fixNameSpaceOption(*antlrTool->namespaceAntlr);

    genHashLines = antlrTool->genHashLines;

    if (g->hasOption("namespace")) {
        TokenRef t = g->getOption("namespace");
        if (t)
            nameSpace = new NameSpace(t->text);
    }
    if (g->hasOption("namespaceAntlr")) {
        TokenRef t = g->getOption("namespaceAntlr");
        if (t)
            namespaceAntlr = fixNameSpaceOption(t->text);
    }
    if (g->hasOption("namespaceStd")) {
        TokenRef t = g->getOption("namespaceStd");
        if (t)
            namespaceStd = fixNameSpaceOption(t->text);
    }
    if (g->hasOption("genHashLines")) {
        TokenRef t = g->getOption("genHashLines");
        if (t)
            genHashLines = stripFrontBack(t->text, "\"", "\"") == "true";
    }
    noConstructors = antlrTool->noConstructors;
    if (g->hasOption("noConstructors")) {
        TokenRef t = g->getOption("noConstructors");
        if (t && !(t->text == "true" || t->text == "false"))
            antlrTool->error("noConstructors option must be true or false", antlrTool->grammarFile, t->line, t->column);
        noConstructors = t->text == "true";
    }

    if (g->isParser()) {
        labeledElementASTType = namespaceAntlr + "RefAST";
        labeledElementASTInit = namespaceAntlr + "nullAST";
        if (g->hasOption("ASTLabelType")) {
            TokenRef tsuffix = g->getOption("ASTLabelType");
            if (tsuffix) {
                std::string suffix = stripFrontBack(tsuffix->text, "\"", "\"");
                usingCustomAST = true;
                labeledElementASTType = suffix;
                labeledElementASTInit = suffix + "(" + namespaceAntlr + "nullAST)";
            }
        }
        labeledElementType = namespaceAntlr + "RefToken ";
        labeledElementInit = namespaceAntlr + "nullToken";
        commonExtraArgs = "";
        commonExtraParams = "";
        commonLocalVars = "";
        lt1Value = "LT(1)";
        exceptionThrown = namespaceAntlr + "RecognitionException";
        throwNoViable = "throw " + namespaceAntlr + "NoViableAltException(LT(1), getFilename());";
    } else if (g->isLexer()) {
        labeledElementType = "char ";
        labeledElementInit = "'\\0'";
        commonExtraArgs = "";
        commonExtraParams = "bool _createToken";
        commonLocalVars = "int _ttype; " + namespaceAntlr + "RefToken _token; " + namespaceStd + "string::size_type _begin = text.length();";
        lt1Value = "LA(1)";
        exceptionThrown = namespaceAntlr + "RecognitionException";
        throwNoViable = "throw " + namespaceAntlr + "NoViableAltForCharException(LA(1), getFilename(), getLine(), getColumn());";
    } else if (g->isTreeWalker()) {
        labeledElementInit = namespaceAntlr + "nullAST";
        labeledElementASTInit = namespaceAntlr + "nullAST";
        labeledElementASTType = namespaceAntlr + "RefAST";
        labeledElementType = namespaceAntlr + "RefAST";
        commonExtraParams = namespaceAntlr + "RefAST _t";
        throwNoViable = "throw " + namespaceAntlr + "NoViableAltException(_t);";
        lt1Value = "_t";
        if (g->hasOption("ASTLabelType")) {
            TokenRef tsuffix = g->getOption("ASTLabelType");
            if (tsuffix) {
                std::string suffix = stripFrontBack(tsuffix->text, "\"", "\"");
                usingCustomAST = true;
                labeledElementASTType = suffix;
                labeledElementType = suffix;
                labeledElementInit = suffix + "(" + namespaceAntlr + "nullAST)";
                labeledElementASTInit = labeledElementInit;
                commonExtraParams = suffix + " _t";
                throwNoViable = "throw " + namespaceAntlr + "NoViableAltException(" + namespaceAntlr + "RefAST(_t));";
                lt1Value = "_t";
            }
        }
        if (!g->hasOption("ASTLabelType"))
            g->setOption("ASTLabelType", makeTokenRef(ANTLR_STRING_LITERAL, namespaceAntlr + "RefAST"));
        commonExtraArgs = "_t";
        commonLocalVars = "";
        exceptionThrown = namespaceAntlr + "RecognitionException";
    } else {
        antlrTool->panic("Unknown grammar type");
    }
}

} // namespace antlrcpp
