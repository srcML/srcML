// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file Preprocessor.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of antlr.preprocessor (preproc.g and its support classes).
 * The lexer decisions follow the code that ANTLR 2.7.7 generated for preproc.g.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "Preprocessor.hpp"
#include "Tool.hpp"
#include "Scanner.hpp"
#include "Util.hpp"
#include <fstream>
#include <sstream>
#include <map>

namespace antlrcpp {

namespace {

enum PPTokenType {
    PP_EOF = 1,
    PP_LITERAL_tokens = 4,
    PP_HEADER_ACTION = 5,
    PP_SUBRULE_BLOCK = 6,
    PP_ACTION = 7,
    PP_LITERAL_class = 8,
    PP_ID = 9,
    PP_LITERAL_extends = 10,
    PP_SEMI = 11,
    PP_TOKENS_SPEC = 12,
    PP_OPTIONS_START = 13,
    PP_ASSIGN_RHS = 14,
    PP_RCURLY = 15,
    PP_LITERAL_protected = 16,
    PP_LITERAL_private = 17,
    PP_LITERAL_public = 18,
    PP_BANG = 19,
    PP_ARG_ACTION = 20,
    PP_LITERAL_returns = 21,
    PP_RULE_BLOCK = 22,
    PP_LITERAL_throws = 23,
    PP_COMMA = 24,
    PP_LITERAL_exception = 25,
    PP_LITERAL_catch = 26,
    PP_LPAREN = 29,
    PP_RPAREN = 30,
    PP_ID_OR_KEYWORD = 31,
    PP_STRING_LITERAL = 39,
    PP_CHAR_LITERAL = 38,
};

bool inRange(int c) {
    return c >= 3 && c <= 255;
}

// ~';'
bool tokenSet0(int c) {
    return inRange(c) && c != ';';
}

bool tokenSet1(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// ~')'
bool tokenSet2(int c) {
    return inRange(c) && c != ')';
}

// ~(')'|';')
bool tokenSet3(int c) {
    return inRange(c) && c != ')' && c != ';';
}

bool tokenSet4(int c) {
    return tokenSet1(c) || c == '|';
}

// ~('\n'|'\r'|'('|')'|'/'|'{'|'"'|'\''|';')
bool tokenSet5(int c) {
    return inRange(c) && c != '\n' && c != '\r' && c != '(' && c != ')' && c != '/' && c != '{' && c != '"' && c != '\'' && c != ';';
}

// ~'\''
bool tokenSet6(int c) {
    return inRange(c) && c != '\'';
}

// ~'"' without '\\'
bool tokenSet7(int c) {
    return inRange(c) && c != '"' && c != '\\';
}

// ~'\'' without '\\'
bool tokenSet8(int c) {
    return inRange(c) && c != '\'' && c != '\\';
}

bool tokenSet9(int c) {
    return tokenSet1(c) || c == '"' || c == '/' || c == '{';
}

bool tokenSet10(int c) {
    return tokenSet1(c) || c == '/' || c == '{';
}

bool isIdStart(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool isIdChar(int c) {
    return isIdStart(c) || (c >= '0' && c <= '9');
}

class PreprocessorLexer : public CharScanner {
public:
    explicit PreprocessorLexer(const std::string& input) : CharScanner(input) {
        literals.put("public", PP_LITERAL_public);
        literals.put("class", PP_LITERAL_class);
        literals.put("throws", PP_LITERAL_throws);
        literals.put("catch", PP_LITERAL_catch);
        literals.put("private", PP_LITERAL_private);
        literals.put("extends", PP_LITERAL_extends);
        literals.put("protected", PP_LITERAL_protected);
        literals.put("returns", PP_LITERAL_returns);
        literals.put("tokens", PP_LITERAL_tokens);
        literals.put("exception", PP_LITERAL_exception);
    }

    TokenRef nextToken() {
        for (;;) {
            resetText();
            int ttype;
            int c = LA(1);
            switch (c) {
            case ':': mRULE_BLOCK(); ttype = PP_RULE_BLOCK; break;
            case '\t': case '\n': case '\r': case ' ': mWS(); ttype = TokenType::SKIP; break;
            case '/': mCOMMENT(); ttype = TokenType::SKIP; break;
            case '{': mACTION(); ttype = PP_ACTION; break;
            case '"': mSTRING_LITERAL(); ttype = PP_STRING_LITERAL; break;
            case '\'': mCHAR_LITERAL(); ttype = PP_CHAR_LITERAL; break;
            case '!': match('!'); ttype = PP_BANG; break;
            case ';': match(';'); ttype = PP_SEMI; break;
            case ',': match(','); ttype = PP_COMMA; break;
            case '}': match('}'); ttype = PP_RCURLY; break;
            case ')': match(')'); ttype = PP_RPAREN; break;
            case '=': mASSIGN_RHS(); ttype = PP_ASSIGN_RHS; break;
            case '[': mARG_ACTION(); ttype = PP_ARG_ACTION; break;
            default:
                if (isIdStart(c)) {
                    ttype = mID_OR_KEYWORD();
                } else if (c == '(' && tokenSet0(LA(2))) {
                    mSUBRULE_BLOCK();
                    ttype = PP_SUBRULE_BLOCK;
                } else if (c == '(') {
                    match('(');
                    ttype = PP_LPAREN;
                } else if (c == EOF_CHAR) {
                    return makeToken(TokenType::EOF_TYPE);
                } else {
                    throwNoViable();
                }
            }
            if (ttype == TokenType::SKIP)
                continue;
            ttype = testLiteralsTable(ttype);
            auto t = makeToken(ttype);
            t->text = text;
            return t;
        }
    }

private:
    void mRULE_BLOCK() {
        match(':');
        optionalStrippedWS();
        mALT();
        if (tokenSet1(LA(1))) {
            std::size_t saveIndex = text.size();
            mWS();
            text.resize(saveIndex);
        } else if (LA(1) != ';' && LA(1) != '|') {
            throwNoViable();
        }
        while (LA(1) == '|') {
            match('|');
            optionalStrippedWS();
            mALT();
            if (tokenSet1(LA(1))) {
                std::size_t saveIndex = text.size();
                mWS();
                text.resize(saveIndex);
            } else if (LA(1) != ';' && LA(1) != '|') {
                throwNoViable();
            }
        }
        match(';');
    }

    void optionalStrippedWS() {
        if (tokenSet1(LA(1)) && tokenSet2(LA(2))) {
            std::size_t saveIndex = text.size();
            mWS();
            text.resize(saveIndex);
        } else if (!tokenSet2(LA(1))) {
            throwNoViable();
        }
    }

    void mWS() {
        int cnt = 0;
        for (;;) {
            if (LA(1) == ' ') {
                match(' ');
            } else if (LA(1) == '\t') {
                match('\t');
            } else if (LA(1) == '\n' || LA(1) == '\r') {
                mNEWLINE();
            } else {
                if (cnt >= 1)
                    break;
                throwNoViable();
            }
            ++cnt;
        }
    }

    void mALT() {
        while (tokenSet3(LA(1)) && inRange(LA(2)))
            mELEMENT();
    }

    void mSUBRULE_BLOCK() {
        match('(');
        if (tokenSet1(LA(1)) && tokenSet0(LA(2)))
            mWS();
        else if (!tokenSet0(LA(1)))
            throwNoViable();
        mALT();
        while (tokenSet4(LA(1)) && tokenSet0(LA(2))) {
            if (tokenSet1(LA(1)))
                mWS();
            else if (LA(1) != '|')
                throwNoViable();
            match('|');
            if (tokenSet1(LA(1)) && tokenSet0(LA(2)))
                mWS();
            else if (!tokenSet0(LA(1)))
                throwNoViable();
            mALT();
        }
        if (tokenSet1(LA(1)))
            mWS();
        else if (LA(1) != ')')
            throwNoViable();
        match(')');
        if (LA(1) == '=' && LA(2) == '>')
            match("=>");
        else if (LA(1) == '*')
            match('*');
        else if (LA(1) == '+')
            match('+');
        else if (LA(1) == '?')
            match('?');
    }

    void mELEMENT() {
        switch (LA(1)) {
        case '/': mCOMMENT(); break;
        case '{': mACTION(); break;
        case '"': mSTRING_LITERAL(); break;
        case '\'': mCHAR_LITERAL(); break;
        case '(': mSUBRULE_BLOCK(); break;
        case '\n': case '\r': mNEWLINE(); break;
        default:
            if (tokenSet5(LA(1)))
                consume();
            else
                throwNoViable();
        }
    }

    void mCOMMENT() {
        if (LA(1) == '/' && LA(2) == '/')
            mSL_COMMENT();
        else if (LA(1) == '/' && LA(2) == '*')
            mML_COMMENT();
        else
            throwNoViable();
    }

    void mACTION() {
        match('{');
        for (;;) {
            if (LA(1) == '}')
                break;
            if ((LA(1) == '\n' || LA(1) == '\r') && inRange(LA(2)))
                mNEWLINE();
            else if (LA(1) == '{' && inRange(LA(2)))
                mACTION();
            else if (LA(1) == '\'' && tokenSet6(LA(2)))
                mCHAR_LITERAL();
            else if (LA(1) == '/' && (LA(2) == '*' || LA(2) == '/'))
                mCOMMENT();
            else if (LA(1) == '"' && inRange(LA(2)))
                mSTRING_LITERAL();
            else if (inRange(LA(1)) && inRange(LA(2)))
                matchNot(EOF_CHAR);
            else
                break;
        }
        match('}');
    }

    void mSTRING_LITERAL() {
        match('"');
        for (;;) {
            if (LA(1) == '\\')
                mESC();
            else if (tokenSet7(LA(1)))
                matchNot('"');
            else
                break;
        }
        match('"');
    }

    void mCHAR_LITERAL() {
        match('\'');
        if (LA(1) == '\\')
            mESC();
        else if (tokenSet8(LA(1)))
            matchNot('\'');
        else
            throwNoViable();
        match('\'');
    }

    void mNEWLINE() {
        if (LA(1) == '\r' && LA(2) == '\n') {
            match('\r');
            match('\n');
            newline();
        } else if (LA(1) == '\r') {
            match('\r');
            newline();
        } else if (LA(1) == '\n') {
            match('\n');
            newline();
        } else {
            throwNoViable();
        }
    }

    void wsOrCommentLoop() {
        for (;;) {
            if (tokenSet1(LA(1)))
                mWS();
            else if (LA(1) == '/')
                mCOMMENT();
            else
                break;
        }
    }

    int mID_OR_KEYWORD() {
        std::size_t begin = text.size();
        mID();
        std::string id = text.substr(begin);
        int ttype = testLiteralsTable(id, PP_ID);
        if (tokenSet9(LA(1)) && inRange(LA(2)) && id == "header") {
            if (tokenSet1(LA(1)) && tokenSet9(LA(2)))
                mWS();
            else if (!(tokenSet9(LA(1)) && inRange(LA(2))))
                throwNoViable();
            if (LA(1) == '"')
                mSTRING_LITERAL();
            else if (!tokenSet10(LA(1)))
                throwNoViable();
            wsOrCommentLoop();
            mACTION();
            ttype = PP_HEADER_ACTION;
        } else if (tokenSet10(LA(1)) && inRange(LA(2)) && id == "tokens") {
            wsOrCommentLoop();
            mCURLY_BLOCK_SCARF();
            ttype = PP_TOKENS_SPEC;
        } else if (tokenSet10(LA(1)) && id == "options") {
            wsOrCommentLoop();
            match('{');
            ttype = PP_OPTIONS_START;
        }
        return ttype;
    }

    void mID() {
        if (!isIdStart(LA(1)))
            throwNoViable();
        consume();
        while (isIdChar(LA(1)))
            consume();
    }

    void mCURLY_BLOCK_SCARF() {
        match('{');
        for (;;) {
            if (LA(1) == '}')
                break;
            if ((LA(1) == '\n' || LA(1) == '\r') && inRange(LA(2)))
                mNEWLINE();
            else if (LA(1) == '"' && inRange(LA(2)))
                mSTRING_LITERAL();
            else if (LA(1) == '\'' && tokenSet6(LA(2)))
                mCHAR_LITERAL();
            else if (LA(1) == '/' && (LA(2) == '*' || LA(2) == '/'))
                mCOMMENT();
            else if (inRange(LA(1)) && inRange(LA(2)))
                matchNot(EOF_CHAR);
            else
                break;
        }
        match('}');
    }

    void mASSIGN_RHS() {
        std::size_t saveIndex = text.size();
        match('=');
        text.resize(saveIndex);
        for (;;) {
            if (LA(1) == ';')
                break;
            if (LA(1) == '"' && inRange(LA(2)))
                mSTRING_LITERAL();
            else if (LA(1) == '\'' && tokenSet6(LA(2)))
                mCHAR_LITERAL();
            else if ((LA(1) == '\n' || LA(1) == '\r') && inRange(LA(2)))
                mNEWLINE();
            else if (inRange(LA(1)) && inRange(LA(2)))
                matchNot(EOF_CHAR);
            else
                break;
        }
        match(';');
    }

    void mSL_COMMENT() {
        match("//");
        for (;;) {
            if (LA(1) == '\n' || LA(1) == '\r')
                break;
            if (inRange(LA(1)) && inRange(LA(2)))
                matchNot(EOF_CHAR);
            else
                break;
        }
        mNEWLINE();
    }

    void mML_COMMENT() {
        match("/*");
        for (;;) {
            if (LA(1) == '*' && LA(2) == '/')
                break;
            if ((LA(1) == '\n' || LA(1) == '\r') && inRange(LA(2)))
                mNEWLINE();
            else if (inRange(LA(1)) && inRange(LA(2)))
                matchNot(EOF_CHAR);
            else
                break;
        }
        match("*/");
    }

    void mESC() {
        match('\\');
        switch (LA(1)) {
        case 'n': case 'r': case 't': case 'b': case 'f': case 'w': case 'a': case '"': case '\'': case '\\':
            consume();
            break;
        case '0': case '1': case '2': case '3':
            matchRange('0', '3');
            if (LA(1) >= '0' && LA(1) <= '9' && inRange(LA(2))) {
                matchRange('0', '9');
                if (LA(1) >= '0' && LA(1) <= '9' && inRange(LA(2)))
                    matchRange('0', '9');
                else if (!inRange(LA(1)))
                    throwNoViable();
            } else if (!inRange(LA(1))) {
                throwNoViable();
            }
            break;
        case '4': case '5': case '6': case '7':
            matchRange('4', '7');
            if (LA(1) >= '0' && LA(1) <= '9' && inRange(LA(2)))
                matchRange('0', '9');
            else if (!inRange(LA(1)))
                throwNoViable();
            break;
        case 'u':
            match('u');
            for (int i = 0; i < 4; ++i) {
                if (hexDigit(LA(1)) < 0)
                    throwNoViable();
                consume();
            }
            break;
        default:
            throwNoViable();
        }
    }

    void mARG_ACTION() {
        match('[');
        for (;;) {
            if (LA(1) == ']')
                break;
            if (LA(1) == '[' && inRange(LA(2)))
                mARG_ACTION();
            else if ((LA(1) == '\n' || LA(1) == '\r') && inRange(LA(2)))
                mNEWLINE();
            else if (LA(1) == '\'' && tokenSet6(LA(2)))
                mCHAR_LITERAL();
            else if (LA(1) == '"' && inRange(LA(2)))
                mSTRING_LITERAL();
            else if (inRange(LA(1)) && inRange(LA(2)))
                matchNot(EOF_CHAR);
            else
                break;
        }
        match(']');
    }
};

// Grammar hierarchy (Hierarchy, Grammar, Rule, Option, GrammarFile)

struct PGrammar;
struct Hierarchy;

struct POption {
    std::string name;
    std::string rhs;
    std::string toString() const { return "\t" + name + "=" + rhs; }
};

// IndexedVector
template <typename T>
struct IndexedVector {
    std::vector<T*> elements;
    std::map<std::string, T*> index;

    void appendElement(const std::string& key, T* value) {
        elements.push_back(value);
        index[key] = value;
    }

    T* getElement(const std::string& key) const {
        auto it = index.find(key);
        return it == index.end() ? nullptr : it->second;
    }
};

struct PRule {
    std::string name;
    std::string block;
    OStr args;
    OStr returnValue;
    std::string throwsSpec;
    OStr initAction;
    IndexedVector<POption>* options = nullptr;
    OStr visibility;
    PGrammar* enclosingGrammar = nullptr;
    bool bang = false;

    bool sameSignature(const PRule& rule) const {
        bool nSame = name == rule.name;
        bool aSame = true;
        bool rSame = true;
        if (args)
            aSame = rule.args && *args == *rule.args;
        if (returnValue)
            rSame = rule.returnValue && *returnValue == *rule.returnValue;
        return nSame && aSame && rSame;
    }

    std::string toString() const {
        std::string s;
        std::string retString = returnValue ? "returns " + *returnValue : std::string();
        std::string argString = args ? *args : std::string();
        s += visibility ? *visibility + " " : std::string();
        s += name + (bang ? "!" : "") + argString + " " + retString + throwsSpec;
        if (options) {
            s += "\noptions {\n";
            for (auto o : options->elements)
                s += o->toString() + "\n";
            s += "}\n";
        }
        if (initAction)
            s += *initAction + "\n";
        s += block;
        return s;
    }
};

std::string optionsToString(const IndexedVector<POption>& options) {
    std::string s = "options {\n";
    for (auto o : options.elements)
        s += o->toString() + "\n";
    s += "}\n\n";
    return s;
}

struct GrammarFile {
    std::string fileName;
    std::string headerAction;
    IndexedVector<POption>* options = nullptr;
    IndexedVector<PGrammar> grammars;
    bool expanded = false;
    Tool* tool = nullptr;

    std::string nameForExpandedGrammarFile(const std::string& f) const {
        if (expanded)
            return "expanded" + tool->fileMinusPath(f);
        return f;
    }

    void generateExpandedFile();
    std::string toString() const;
};

struct PGrammar {
    std::string name;
    std::string fileName;
    OStr superGrammar;
    std::string type;
    IndexedVector<PRule>* rules = nullptr;
    IndexedVector<POption>* options = nullptr;
    OStr tokenSection;
    OStr preambleAction;
    OStr memberAction;
    Hierarchy* hier = nullptr;
    bool predefined = false;
    bool alreadyExpanded = false;
    bool specifiedVocabulary = false;
    OStr superClass;
    OStr importVocab;
    OStr exportVocab;
    Tool* antlrTool = nullptr;

    void addOption(POption* o) {
        if (!options)
            options = new IndexedVector<POption>();
        options->appendElement(o->name, o);
    }

    void addRule(PRule* r) { rules->appendElement(r->name, r); }

    PGrammar* getSuperGrammar() const;
    void expandInPlace();
    void inherit(POption* o);
    void inherit(PRule* r, PGrammar* superG);
    void inheritMemberAction(const OStr& action) {
        if (memberAction)
            return;
        if (action)
            memberAction = action;
    }
    std::string toString() const;
};

struct Hierarchy {
    explicit Hierarchy(Tool* tool) : antlrTool(tool) {
        for (const char* name : { "Lexer", "Parser", "TreeParser" }) {
            auto g = new PGrammar();
            g->name = name;
            g->antlrTool = tool;
            g->predefined = true;
            g->rules = new IndexedVector<PRule>();
            symbols[name] = g;
            symbolOrder.push_back(g);
        }
    }

    void addGrammar(PGrammar* gr) {
        gr->hier = this;
        symbols[gr->name] = gr;
        symbolOrder.push_back(gr);
        getFile(gr->fileName)->grammars.appendElement(gr->name, gr);
    }

    void addGrammarFile(GrammarFile* gf) { files[gf->fileName] = gf; }

    void expandGrammarsInFile(const std::string& fileName) {
        for (auto g : getFile(fileName)->grammars.elements)
            g->expandInPlace();
    }

    PGrammar* findRoot(PGrammar* g) {
        if (!g->superGrammar)
            return g;
        PGrammar* sg = g->getSuperGrammar();
        if (!sg)
            return g;
        return findRoot(sg);
    }

    GrammarFile* getFile(const std::string& fileName) {
        auto it = files.find(fileName);
        return it == files.end() ? nullptr : it->second;
    }

    PGrammar* getGrammar(const std::string& gr) const {
        auto it = symbols.find(gr);
        return it == symbols.end() ? nullptr : it->second;
    }

    bool readGrammarFile(const std::string& file);

    bool verifyThatHierarchyIsComplete() {
        bool complete = true;
        for (auto c : std::vector<PGrammar*>(symbolOrder)) {
            if (!c->superGrammar)
                continue;
            if (!c->getSuperGrammar()) {
                antlrTool->toolError("grammar " + *c->superGrammar + " not defined");
                complete = false;
                symbols.erase(c->name);
            }
        }
        if (!complete)
            return false;
        for (auto c : symbolOrder) {
            if (!c->superGrammar)
                continue;
            c->type = findRoot(c)->name;
        }
        return true;
    }

    Tool* antlrTool;
    std::map<std::string, PGrammar*> symbols;
    std::vector<PGrammar*> symbolOrder;
    std::map<std::string, GrammarFile*> files;
};

PGrammar* PGrammar::getSuperGrammar() const {
    if (!superGrammar)
        return nullptr;
    return hier->getGrammar(*superGrammar);
}

void PGrammar::expandInPlace() {
    if (alreadyExpanded)
        return;
    PGrammar* superG = getSuperGrammar();
    if (!superG)
        return;
    if (!exportVocab)
        exportVocab = name;
    if (superG->predefined)
        return;
    superG->expandInPlace();

    alreadyExpanded = true;
    hier->getFile(fileName)->expanded = true;

    for (auto r : superG->rules->elements)
        inherit(r, superG);

    if (superG->options)
        for (auto o : superG->options->elements)
            inherit(o);

    if (!options || !options->getElement("importVocab")) {
        auto inputV = new POption{ "importVocab", *superG->exportVocab + ";" };
        addOption(inputV);
        std::string originatingGrFileName = superG->fileName;
        std::string path = antlrTool->pathToFile(originatingGrFileName);
        std::string superExportVocabFileName = path + *superG->exportVocab + "TokenTypes.txt";
        std::string newImportVocabFileName = antlrTool->fileMinusPath(superExportVocabFileName);
        if (path != "./") {
            try {
                antlrTool->copyFile(superExportVocabFileName, newImportVocabFileName);
            } catch (const std::exception&) {
                antlrTool->toolError("cannot find/copy importVocab file " + superExportVocabFileName);
                return;
            }
        }
    }

    inheritMemberAction(superG->memberAction);
}

void PGrammar::inherit(POption* o) {
    if (o->name == "importVocab" || o->name == "exportVocab")
        return;
    POption* overriddenOption = options ? options->getElement(o->name) : nullptr;
    if (!overriddenOption)
        addOption(o);
}

void PGrammar::inherit(PRule* r, PGrammar* superG) {
    PRule* overriddenRule = rules->getElement(r->name);
    if (overriddenRule) {
        if (!overriddenRule->sameSignature(*r))
            antlrTool->warning("rule " + name + "." + overriddenRule->name + " has different signature than " + superG->name + "." + overriddenRule->name);
    } else {
        addRule(r);
    }
}

std::string PGrammar::toString() const {
    std::string s;
    if (preambleAction)
        s += *preambleAction;
    if (!superGrammar)
        return "class " + name + ";";
    if (superClass)
        s += "class " + name + " extends " + *superClass + ";";
    else
        s += "class " + name + " extends " + type + ";";
    s += "\n\n";
    if (options)
        s += optionsToString(*options);
    if (tokenSection)
        s += *tokenSection + "\n";
    if (memberAction)
        s += *memberAction + "\n";
    for (auto r : rules->elements) {
        if (name != r->enclosingGrammar->name)
            s += "// inherited from grammar " + r->enclosingGrammar->name + "\n";
        s += r->toString() + "\n\n";
    }
    return s;
}

std::string GrammarFile::toString() const {
    std::string s = headerAction;
    if (options)
        s += optionsToString(*options);
    for (auto g : grammars.elements)
        s += g->toString();
    return s;
}

void GrammarFile::generateExpandedFile() {
    if (!expanded)
        return;
    OutputFile* expF = tool->openOutputFile(nameForExpandedGrammarFile(fileName));
    expF->out << toString() << "\n";
    expF->close();
}

// Preprocessor parser (preproc.g)
class PreprocessorParser : public TokenStreamParser {
public:
    PreprocessorParser(PreprocessorLexer& lexer, Tool* tool) : TokenStreamParser([&lexer]() { return lexer.nextToken(); }), antlrTool(tool) {
        tokenNames = { "<0>", "EOF", "<2>", "NULL_TREE_LOOKAHEAD", "\"tokens\"", "HEADER_ACTION", "SUBRULE_BLOCK", "ACTION",
                       "\"class\"", "ID", "\"extends\"", "SEMI", "TOKENS_SPEC", "OPTIONS_START", "ASSIGN_RHS", "RCURLY",
                       "\"protected\"", "\"private\"", "\"public\"", "BANG", "ARG_ACTION", "\"returns\"", "RULE_BLOCK",
                       "\"throws\"", "COMMA", "\"exception\"", "\"catch\"" };
    }

    void grammarFile(Hierarchy* hier, const std::string& file) {
        IndexedVector<POption>* opt = nullptr;
        while (LA(1) == PP_HEADER_ACTION) {
            hier->getFile(file)->headerAction += LT(1)->text + "\n";
            match(PP_HEADER_ACTION);
        }
        if (LA(1) == PP_OPTIONS_START)
            opt = optionSpec(nullptr);
        while (LA(1) == PP_ACTION || LA(1) == PP_LITERAL_class) {
            PGrammar* gr = class_def(file, hier);
            if (gr && opt)
                hier->getFile(file)->options = opt;
            if (gr) {
                gr->fileName = file;
                hier->addGrammar(gr);
            }
        }
        match(TokenType::EOF_TYPE);
    }

private:
    IndexedVector<POption>* optionSpec(PGrammar* gr) {
        auto options = new IndexedVector<POption>();
        match(PP_OPTIONS_START);
        while (LA(1) == PP_ID) {
            TokenRef op = LT(1);
            match(PP_ID);
            TokenRef rhs = LT(1);
            match(PP_ASSIGN_RHS);
            auto newOp = new POption{ op->text, rhs->text };
            options->appendElement(newOp->name, newOp);
            if (gr && op->text == "importVocab") {
                gr->specifiedVocabulary = true;
                gr->importVocab = rhs->text;
            } else if (gr && op->text == "exportVocab") {
                gr->exportVocab = trim(rhs->text.substr(0, rhs->text.size() - 1));
            }
        }
        match(PP_RCURLY);
        return options;
    }

    PGrammar* class_def(const std::string& file, Hierarchy* hier) {
        TokenRef preamble;
        OStr sc;
        auto rules = new IndexedVector<PRule>();
        if (LA(1) == PP_ACTION) {
            preamble = LT(1);
            match(PP_ACTION);
        }
        match(PP_LITERAL_class);
        TokenRef sub = LT(1);
        match(PP_ID);
        match(PP_LITERAL_extends);
        TokenRef sup = LT(1);
        match(PP_ID);
        if (LA(1) == PP_SUBRULE_BLOCK) {
            sc = LT(1)->text;
            match(PP_SUBRULE_BLOCK);
        }
        match(PP_SEMI);
        if (hier->getGrammar(sub->text))
            throw RecognitionException{ "redefinition of grammar " + sub->text, file, sub->line, sub->column };
        auto gr = new PGrammar();
        gr->name = sub->text;
        gr->superGrammar = sup->text;
        gr->rules = rules;
        gr->antlrTool = antlrTool;
        gr->superClass = sc;
        if (preamble)
            gr->preambleAction = preamble->text;
        IndexedVector<POption>* classOptions = nullptr;
        if (LA(1) == PP_OPTIONS_START)
            classOptions = optionSpec(gr);
        gr->options = classOptions;
        if (LA(1) == PP_TOKENS_SPEC) {
            gr->tokenSection = LT(1)->text;
            match(PP_TOKENS_SPEC);
        }
        if (LA(1) == PP_ACTION) {
            gr->memberAction = LT(1)->text;
            match(PP_ACTION);
        }
        int cnt = 0;
        while (LA(1) == PP_ID || LA(1) == PP_LITERAL_protected || LA(1) == PP_LITERAL_private || LA(1) == PP_LITERAL_public) {
            rule(gr);
            ++cnt;
        }
        if (cnt == 0)
            throwNoViable();
        return gr;
    }

    void rule(PGrammar* gr) {
        IndexedVector<POption>* o = nullptr;
        OStr vis;
        bool bang = false;
        std::string thr;
        TokenRef arg;
        TokenRef ret;
        TokenRef init;
        switch (LA(1)) {
        case PP_LITERAL_protected: match(PP_LITERAL_protected); vis = "protected"; break;
        case PP_LITERAL_private: match(PP_LITERAL_private); vis = "private"; break;
        case PP_LITERAL_public: match(PP_LITERAL_public); vis = "public"; break;
        default: break;
        }
        TokenRef r = LT(1);
        match(PP_ID);
        if (LA(1) == PP_BANG) {
            match(PP_BANG);
            bang = true;
        }
        if (LA(1) == PP_ARG_ACTION) {
            arg = LT(1);
            match(PP_ARG_ACTION);
        }
        if (LA(1) == PP_LITERAL_returns) {
            match(PP_LITERAL_returns);
            ret = LT(1);
            match(PP_ARG_ACTION);
        }
        if (LA(1) == PP_LITERAL_throws) {
            match(PP_LITERAL_throws);
            thr = "throws " + LT(1)->text;
            match(PP_ID);
            while (LA(1) == PP_COMMA) {
                match(PP_COMMA);
                thr += "," + LT(1)->text;
                match(PP_ID);
            }
        }
        if (LA(1) == PP_OPTIONS_START)
            o = optionSpec(nullptr);
        if (LA(1) == PP_ACTION) {
            init = LT(1);
            match(PP_ACTION);
        }
        TokenRef blk = LT(1);
        match(PP_RULE_BLOCK);
        std::string eg = exceptionGroup();

        auto ppr = new PRule();
        ppr->name = r->text;
        ppr->block = blk->text + eg;
        ppr->options = o;
        ppr->enclosingGrammar = gr;
        ppr->throwsSpec = thr;
        if (arg)
            ppr->args = arg->text;
        if (ret)
            ppr->returnValue = ret->text;
        if (init)
            ppr->initAction = init->text;
        ppr->bang = bang;
        ppr->visibility = vis;
        if (gr)
            gr->addRule(ppr);
    }

    std::string exceptionGroup() {
        std::string g;
        while (LA(1) == PP_LITERAL_exception) {
            std::string es = "\nexception ";
            match(PP_LITERAL_exception);
            if (LA(1) == PP_ARG_ACTION) {
                es += LT(1)->text;
                match(PP_ARG_ACTION);
            }
            while (LA(1) == PP_LITERAL_catch) {
                match(PP_LITERAL_catch);
                TokenRef a1 = LT(1);
                match(PP_ARG_ACTION);
                TokenRef a2 = LT(1);
                match(PP_ACTION);
                es += "\ncatch " + a1->text + " " + a2->text;
            }
            g += es;
        }
        return g;
    }

    Tool* antlrTool;
};

bool Hierarchy::readGrammarFile(const std::string& file) {
    std::ifstream in(file, std::ios::binary);
    if (!in)
        return false;
    std::stringstream buf;
    buf << in.rdbuf();

    auto gf = new GrammarFile();
    gf->fileName = file;
    gf->tool = antlrTool;
    addGrammarFile(gf);

    PreprocessorLexer lexer(buf.str());
    lexer.filename = file;
    PreprocessorParser parser(lexer, antlrTool);
    parser.filename = file;
    try {
        parser.grammarFile(this, file);
    } catch (const RecognitionException& ex) {
        antlrTool->error(ex.message, ex.filename, ex.line, ex.column);
    } catch (const TokenStreamException& ex) {
        antlrTool->toolError("Token stream error reading grammar(s):\n" + ex.error.filename + ":" + std::to_string(ex.error.line) +
                             ":" + std::to_string(ex.error.column) + ": " + ex.error.message);
    }
    return true;
}

} // namespace

PreprocessorTool::PreprocessorTool(Tool* tool, const std::vector<std::string>& args) : antlrTool(tool) {
    processArguments(args);
}

void PreprocessorTool::processArguments(const std::vector<std::string>& incomingArgs) {
    for (std::size_t i = 0; i < incomingArgs.size(); i++) {
        const std::string& arg = incomingArgs[i];
        if (arg.empty()) {
            antlrTool->warning("Zero length argument ignoring...");
            continue;
        }
        if (arg == "-glib") {
            if (i + 1 < incomingArgs.size()) {
                grammars = Tool::parseSeparatedList(incomingArgs[i + 1], ';');
                haveGrammars = !grammars.empty();
            }
            i++;
        } else if (arg == "-o") {
            args.push_back(arg);
            if (i + 1 >= incomingArgs.size()) {
                antlrTool->error("missing output directory with -o option; ignoring");
            } else {
                i++;
                args.push_back(incomingArgs[i]);
                antlrTool->setOutputDirectory(incomingArgs[i]);
            }
        } else if (arg[0] == '-') {
            args.push_back(arg);
        } else {
            grammarFileName = arg;
            if (!haveGrammars)
                grammars.clear();
            haveGrammars = true;
            grammars.push_back(grammarFileName);
            if (i + 1 < incomingArgs.size()) {
                antlrTool->warning("grammar file must be last; ignoring other arguments...");
                break;
            }
        }
    }
}

bool PreprocessorTool::preprocess() {
    if (grammarFileName.empty()) {
        antlrTool->toolError("no grammar file specified");
        return false;
    }
    Hierarchy* theHierarchy = new Hierarchy(antlrTool);
    for (auto& f : grammars) {
        if (!theHierarchy->readGrammarFile(f)) {
            antlrTool->toolError("file " + f + " not found");
            return false;
        }
    }

    if (!theHierarchy->verifyThatHierarchyIsComplete())
        return false;

    theHierarchy->expandGrammarsInFile(grammarFileName);
    GrammarFile* gf = theHierarchy->getFile(grammarFileName);
    std::string expandedFileName = gf->nameForExpandedGrammarFile(grammarFileName);

    if (expandedFileName == grammarFileName) {
        args.push_back(grammarFileName);
    } else {
        gf->generateExpandedFile();
        args.push_back(antlrTool->getOutputDirectory() + "/" + expandedFileName);
    }
    return true;
}

} // namespace antlrcpp
