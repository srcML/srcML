// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ANTLRParser.cpp
 *
 * @copyright Copyright (C) 2026 srcML, LLC. (www.srcML.org)
 *
 * Port of ANTLRLexer and ANTLRParser (antlr.g). The decisions follow the code
 * that ANTLR 2.7.7 generated for these rules.
 *
 * Derived from ANTLR 2.7.7, which is in the public domain:
 *
 *   ANTLR Translator Generator
 *   Project led by Terence Parr at http://www.cs.usfca.edu
 *   Software rights: http://www.antlr.org/license.html
 */

#include "ANTLRParser.hpp"
#include "MakeGrammar.hpp"
#include "Tool.hpp"
#include "Scanner.hpp"
#include "ANTLRTokenTypes.hpp"

namespace antlrcpp {

namespace {

bool inRange(int c) {
    return c >= 3 && c <= 255;
}

// ~('\n'|'\r')
bool tokenSet0(int c) {
    return inRange(c) && c != '\n' && c != '\r';
}

// ~'\'' without '\\'
bool tokenSet1(int c) {
    return inRange(c) && c != '\'' && c != '\\';
}

// ~'"' without '\\'
bool tokenSet2(int c) {
    return inRange(c) && c != '"' && c != '\\';
}

// ~']' without the other alternatives of NESTED_ARG_ACTION
bool tokenSet3(int c) {
    return inRange(c) && c != '\n' && c != '\r' && c != '"' && c != '\'' && c != '[' && c != ']';
}

// ~'\''
bool tokenSet4(int c) {
    return inRange(c) && c != '\'';
}

bool isWS(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

bool isLower(int c) {
    return c >= 'a' && c <= 'z';
}

bool isUpper(int c) {
    return c >= 'A' && c <= 'Z';
}

bool isIdChar(int c) {
    return isLower(c) || isUpper(c) || c == '_' || (c >= '0' && c <= '9');
}

class ANTLRLexer : public CharScanner {
public:
    explicit ANTLRLexer(const std::string& input) : CharScanner(input) {
        literals.put("public", ANTLR_LITERAL_public);
        literals.put("class", ANTLR_LITERAL_class);
        literals.put("header", ANTLR_LITERAL_header);
        literals.put("throws", ANTLR_LITERAL_throws);
        literals.put("lexclass", ANTLR_LITERAL_lexclass);
        literals.put("catch", ANTLR_LITERAL_catch);
        literals.put("private", ANTLR_LITERAL_private);
        literals.put("options", ANTLR_LITERAL_options);
        literals.put("extends", ANTLR_LITERAL_extends);
        literals.put("protected", ANTLR_LITERAL_protected);
        literals.put("TreeParser", ANTLR_LITERAL_TreeParser);
        literals.put("Parser", ANTLR_LITERAL_Parser);
        literals.put("Lexer", ANTLR_LITERAL_Lexer);
        literals.put("returns", ANTLR_LITERAL_returns);
        literals.put("charVocabulary", ANTLR_LITERAL_charVocabulary);
        literals.put("tokens", ANTLR_LITERAL_tokens);
        literals.put("exception", ANTLR_LITERAL_exception);
    }

    TokenRef nextToken() {
        for (;;) {
            resetText();
            returnToken = nullptr;
            int c = LA(1);
            switch (c) {
            case '\t': case '\n': case '\r': case ' ': mWS(true); break;
            case '/': mCOMMENT(true); break;
            case '<': simple(ANTLR_OPEN_ELEMENT_OPTION); break;
            case '>': simple(ANTLR_CLOSE_ELEMENT_OPTION); break;
            case ',': simple(ANTLR_COMMA); break;
            case '?': simple(ANTLR_QUESTION); break;
            case '#':
                match("#(");
                returnToken = makeTextToken(ANTLR_TREE_BEGIN);
                break;
            case '(': simple(ANTLR_LPAREN); break;
            case ')': simple(ANTLR_RPAREN); break;
            case ':': simple(ANTLR_COLON); break;
            case '*': simple(ANTLR_STAR); break;
            case '+': simple(ANTLR_PLUS); break;
            case ';': simple(ANTLR_SEMI); break;
            case '^': simple(ANTLR_CARET); break;
            case '!': simple(ANTLR_BANG); break;
            case '|': simple(ANTLR_OR); break;
            case '~': simple(ANTLR_NOT_OP); break;
            case '}': simple(ANTLR_RCURLY); break;
            case '\'':
                mCHAR_LITERAL();
                returnToken = makeTextToken(ANTLR_CHAR_LITERAL);
                break;
            case '"':
                mSTRING_LITERAL();
                returnToken = makeTextToken(ANTLR_STRING_LITERAL);
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                while (LA(1) >= '0' && LA(1) <= '9')
                    consume();
                returnToken = makeTextToken(ANTLR_INT);
                break;
            case '[': mARG_ACTION(); break;
            case '{': mACTION(); break;
            default:
                if (isUpper(c)) {
                    mTOKEN_REF();
                } else if (isLower(c)) {
                    mRULE_REF();
                } else if (c == '=' && LA(2) == '>') {
                    match("=>");
                    returnToken = makeTextToken(ANTLR_IMPLIES);
                } else if (c == '.' && LA(2) == '.') {
                    match("..");
                    returnToken = makeTextToken(ANTLR_RANGE);
                } else if (c == '=') {
                    simple(ANTLR_ASSIGN);
                } else if (c == '.') {
                    simple(ANTLR_WILDCARD);
                } else if (c == EOF_CHAR) {
                    returnToken = makeToken(TokenType::EOF_TYPE);
                } else {
                    throwNoViable();
                }
            }
            if (!returnToken)
                continue;
            return returnToken;
        }
    }

private:
    TokenRef makeTextToken(int type) {
        auto t = makeToken(type);
        t->text = text;
        return t;
    }

    void simple(int type) {
        consume();
        returnToken = makeTextToken(type);
    }

    void mWS(bool createToken) {
        switch (LA(1)) {
        case ' ': match(' '); break;
        case '\t': match('\t'); break;
        case '\n':
            match('\n');
            newline();
            break;
        default:
            if (LA(1) == '\r' && LA(2) == '\n') {
                match('\r');
                match('\n');
                newline();
            } else if (LA(1) == '\r') {
                match('\r');
                newline();
            } else {
                throwNoViable();
            }
        }
        if (createToken)
            returnToken = nullptr;
    }

    // returns the token type (COMMENT is SKIP unless DOC_COMMENT)
    void mCOMMENT(bool createToken) {
        std::size_t begin = text.size();
        int ttype = ANTLR_COMMENT;
        if (LA(1) == '/' && LA(2) == '/') {
            mSL_COMMENT();
        } else if (LA(1) == '/' && LA(2) == '*') {
            ttype = mML_COMMENT();
        } else {
            throwNoViable();
        }
        if (ttype != ANTLR_DOC_COMMENT)
            ttype = TokenType::SKIP;
        if (createToken) {
            TokenRef token;
            finishToken(createToken, token, ttype, begin);
        }
    }

    void mSL_COMMENT() {
        match("//");
        while (tokenSet0(LA(1)))
            consume();
        if (LA(1) == '\r' && LA(2) == '\n') {
            match('\r');
            match('\n');
        } else if (LA(1) == '\r') {
            match('\r');
        } else if (LA(1) == '\n') {
            match('\n');
        } else {
            throwNoViable();
        }
        newline();
    }

    int mML_COMMENT() {
        int ttype = ANTLR_ML_COMMENT;
        match("/*");
        if (LA(1) == '*' && inRange(LA(2)) && LA(2) != '/') {
            match('*');
            ttype = ANTLR_DOC_COMMENT;
        } else if (inRange(LA(1)) && inRange(LA(2))) {
        } else {
            throwNoViable();
        }
        for (;;) {
            if (LA(1) == '*' && LA(2) == '/')
                break;
            if (LA(1) == '\r' && LA(2) == '\n') {
                match('\r');
                match('\n');
                newline();
            } else if (LA(1) == '\r' && inRange(LA(2))) {
                match('\r');
                newline();
            } else if (tokenSet0(LA(1)) && inRange(LA(2))) {
                consume();
            } else if (LA(1) == '\n') {
                match('\n');
                newline();
            } else {
                break;
            }
        }
        match("*/");
        return ttype;
    }

    void mCHAR_LITERAL() {
        match('\'');
        if (LA(1) == '\\')
            mESC();
        else if (tokenSet1(LA(1)))
            matchNot('\'');
        else
            throwNoViable();
        match('\'');
    }

    void mSTRING_LITERAL() {
        match('"');
        for (;;) {
            if (LA(1) == '\\')
                mESC();
            else if (tokenSet2(LA(1)))
                matchNot('"');
            else
                break;
        }
        match('"');
    }

    void mESC() {
        match('\\');
        switch (LA(1)) {
        case 'n': case 'r': case 't': case 'b': case 'f': case 'w': case 'a': case '"': case '\'': case '\\':
            consume();
            break;
        case '0': case '1': case '2': case '3':
            matchRange('0', '3');
            if (LA(1) >= '0' && LA(1) <= '7' && inRange(LA(2))) {
                matchRange('0', '7');
                if (LA(1) >= '0' && LA(1) <= '7' && inRange(LA(2)))
                    matchRange('0', '7');
                else if (!inRange(LA(1)))
                    throwNoViable();
            } else if (!inRange(LA(1))) {
                throwNoViable();
            }
            break;
        case '4': case '5': case '6': case '7':
            matchRange('4', '7');
            if (LA(1) >= '0' && LA(1) <= '7' && inRange(LA(2)))
                matchRange('0', '7');
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
        mNESTED_ARG_ACTION();
        setText(stripFrontBack(text, "[", "]"));
        returnToken = makeTextToken(ANTLR_ARG_ACTION);
    }

    void mNESTED_ARG_ACTION() {
        match('[');
        for (;;) {
            switch (LA(1)) {
            case '[': mNESTED_ARG_ACTION(); continue;
            case '\n':
                match('\n');
                newline();
                continue;
            case '\'': mCHAR_LITERAL(); continue;
            case '"': mSTRING_LITERAL(); continue;
            default:
                if (LA(1) == '\r' && LA(2) == '\n') {
                    match('\r');
                    match('\n');
                    newline();
                    continue;
                } else if (LA(1) == '\r' && inRange(LA(2))) {
                    match('\r');
                    newline();
                    continue;
                } else if (tokenSet3(LA(1))) {
                    matchNot(']');
                    continue;
                }
            }
            break;
        }
        match(']');
    }

    void mACTION() {
        int ttype = ANTLR_ACTION;
        int actionLine = line;
        int actionColumn = column;
        mNESTED_ACTION();
        if (LA(1) == '?') {
            match('?');
            ttype = ANTLR_SEMPRED;
        }
        if (ttype == ANTLR_ACTION)
            setText(stripFrontBack(text, "{", "}"));
        else
            setText(stripFrontBack(text, "{", "}?"));
        auto t = makeTokenRef(ttype, text, actionLine, actionColumn);
        returnToken = t;
    }

    void mNESTED_ACTION() {
        match('{');
        for (;;) {
            if (LA(1) == '}')
                break;
            if ((LA(1) == '\n' || LA(1) == '\r') && inRange(LA(2))) {
                if (LA(1) == '\r' && LA(2) == '\n') {
                    match('\r');
                    match('\n');
                    newline();
                } else if (LA(1) == '\r' && inRange(LA(2))) {
                    match('\r');
                    newline();
                } else if (LA(1) == '\n') {
                    match('\n');
                    newline();
                } else {
                    throwNoViable();
                }
            } else if (LA(1) == '{' && inRange(LA(2))) {
                mNESTED_ACTION();
            } else if (LA(1) == '\'' && tokenSet4(LA(2))) {
                mCHAR_LITERAL();
            } else if (LA(1) == '/' && (LA(2) == '*' || LA(2) == '/')) {
                mCOMMENT(false);
            } else if (LA(1) == '"' && inRange(LA(2))) {
                mSTRING_LITERAL();
            } else if (inRange(LA(1)) && inRange(LA(2))) {
                matchNot(EOF_CHAR);
            } else {
                break;
            }
        }
        match('}');
    }

    void mTOKEN_REF() {
        matchRange('A', 'Z');
        while (isIdChar(LA(1)))
            consume();
        int ttype = testLiteralsTable(ANTLR_TOKEN_REF);
        returnToken = makeTextToken(ttype);
    }

    void mRULE_REF() {
        matchRange('a', 'z');
        while (isIdChar(LA(1)))
            consume();
        int t = testLiteralsTable(ANTLR_RULE_REF);
        int ttype = t;
        if (t == ANTLR_LITERAL_options || t == ANTLR_LITERAL_tokens) {
            mWS_LOOP();
            if (LA(1) == '{') {
                match('{');
                ttype = t == ANTLR_LITERAL_options ? ANTLR_OPTIONS : ANTLR_TOKENS;
            }
        }
        returnToken = makeTextToken(ttype);
    }

    void mWS_LOOP() {
        for (;;) {
            if (isWS(LA(1)))
                mWS(false);
            else if (LA(1) == '/')
                mCOMMENT(false);
            else
                break;
        }
    }
};

// token sets of the generated ANTLRParser
constexpr std::uint64_t tokenSet_0 = 2206556225792ULL;
constexpr std::uint64_t tokenSet_1 = 2472844214400ULL;
constexpr std::uint64_t tokenSet_2 = 1158885407195328ULL;
constexpr std::uint64_t tokenSet_3 = 1159461236965568ULL;
constexpr std::uint64_t tokenSet_4 = 1132497128128576ULL;
constexpr std::uint64_t tokenSet_5 = 1722479914074304ULL;
constexpr std::uint64_t tokenSet_6 = 1722411194597568ULL;
constexpr std::uint64_t tokenSet_7 = 1125899924144192ULL;
constexpr std::uint64_t tokenSet_8 = 1722411190386880ULL;
constexpr std::uint64_t tokenSet_9 = 1159444023476416ULL;
constexpr std::uint64_t tokenSet_10 = 2251345007067328ULL;
constexpr std::uint64_t tokenSet_11 = 1721861130420416ULL;

bool member(std::uint64_t set, int t) {
    return t >= 0 && t < 64 && ((set >> t) & 1) != 0;
}

const std::vector<std::string> tokenNames = {
    "<0>", "EOF", "<2>", "NULL_TREE_LOOKAHEAD", "\"tokens\"", "\"header\"", "STRING_LITERAL", "ACTION", "DOC_COMMENT",
    "\"lexclass\"", "\"class\"", "\"extends\"", "\"Lexer\"", "\"TreeParser\"", "OPTIONS", "ASSIGN", "SEMI", "RCURLY",
    "\"charVocabulary\"", "CHAR_LITERAL", "INT", "OR", "RANGE", "TOKENS", "TOKEN_REF", "OPEN_ELEMENT_OPTION",
    "CLOSE_ELEMENT_OPTION", "LPAREN", "RPAREN", "\"Parser\"", "\"protected\"", "\"public\"", "\"private\"", "BANG",
    "ARG_ACTION", "\"returns\"", "COLON", "\"throws\"", "COMMA", "\"exception\"", "\"catch\"", "RULE_REF", "NOT_OP",
    "SEMPRED", "TREE_BEGIN", "QUESTION", "STAR", "PLUS", "IMPLIES", "CARET", "WILDCARD", "\"options\"",
};

class ANTLRParser : public TokenStreamParser {
public:
    ANTLRParser(ANTLRLexer& lexer, MakeGrammar* behavior, Tool* tool)
        : TokenStreamParser([&lexer]() { return lexer.nextToken(); }), behavior(behavior), antlrTool(tool) {
        tokenNames = antlrcpp::tokenNames;
    }

    void grammar() {
        try {
            while (LA(1) == ANTLR_LITERAL_header) {
                TokenRef n;
                match(ANTLR_LITERAL_header);
                if (LA(1) == ANTLR_STRING_LITERAL) {
                    n = LT(1);
                    match(ANTLR_STRING_LITERAL);
                } else if (LA(1) != ANTLR_ACTION) {
                    throwNoViable();
                }
                TokenRef h = LT(1);
                match(ANTLR_ACTION);
                behavior->refHeaderAction(n, h);
            }
            if (LA(1) == ANTLR_OPTIONS)
                fileOptionsSpec();
            while (LA(1) >= ANTLR_ACTION && LA(1) <= ANTLR_LITERAL_class)
                classDef();
            match(TokenType::EOF_TYPE);
        } catch (const RecognitionException& ex) {
            reportError(ex, "rule grammar trapped:\n" + ex.toString());
            while (LA(1) != TokenType::EOF_TYPE)
                consume();
        }
    }

private:
    void reportError(const RecognitionException& ex, const std::string& s) {
        antlrTool->error(s, ex.filename, ex.line, ex.column);
    }

    bool lastInRule() {
        return blockNesting == 0 && (LA(1) == ANTLR_SEMI || LA(1) == ANTLR_LITERAL_exception || LA(1) == ANTLR_OR);
    }

    void checkForMissingEndRule(const TokenRef& label) {
        if (label->column == 1)
            antlrTool->warning("did you forget to terminate previous rule?", filename, label->line, label->column);
    }

    void fileOptionsSpec() {
        match(ANTLR_OPTIONS);
        while (LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) {
            TokenRef idTok = id();
            match(ANTLR_ASSIGN);
            TokenRef value = optionValue();
            behavior->setFileOption(idTok, value, filename);
            match(ANTLR_SEMI);
        }
        match(ANTLR_RCURLY);
    }

    void classDef() {
        try {
            OStr doc;
            if (LA(1) == ANTLR_ACTION) {
                behavior->refPreambleAction(LT(1));
                match(ANTLR_ACTION);
            }
            if (LA(1) == ANTLR_DOC_COMMENT) {
                doc = LT(1)->text;
                match(ANTLR_DOC_COMMENT);
            }
            bool isId2 = LA(2) == ANTLR_TOKEN_REF || LA(2) == ANTLR_RULE_REF;
            if ((LA(1) == ANTLR_LITERAL_lexclass && isId2) ||
                (LA(1) == ANTLR_LITERAL_class && isId2 && LA(3) == ANTLR_LITERAL_extends && LA(4) == ANTLR_LITERAL_Lexer)) {
                lexerSpec(doc);
            } else if (LA(1) == ANTLR_LITERAL_class && isId2 && LA(3) == ANTLR_LITERAL_extends && LA(4) == ANTLR_LITERAL_TreeParser) {
                treeParserSpec(doc);
            } else if (LA(1) == ANTLR_LITERAL_class && isId2) {
                parserSpec(doc);
            } else {
                throwNoViable();
            }
            rules();
            behavior->endGrammar();
        } catch (const RecognitionException& ex) {
            if (LA(1) == ANTLR_DOC_COMMENT)
                reportError(ex, "JAVADOC comments may only prefix rules and grammars");
            else
                reportError(ex, "rule classDef trapped:\n" + ex.toString());
            behavior->abortGrammar();
            for (;;) {
                consume();
                int t = LA(1);
                if (t == ANTLR_LITERAL_class || t == ANTLR_LITERAL_lexclass || t == TokenType::EOF_TYPE)
                    break;
            }
        }
    }

    TokenRef id() {
        if (LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) {
            TokenRef t = LT(1);
            consume();
            return t;
        }
        throwNoViable();
    }

    OStr superClass() {
        match(ANTLR_LPAREN);
        std::string sup = stripFrontBack(LT(1)->text, "\"", "\"");
        match(ANTLR_STRING_LITERAL);
        match(ANTLR_RPAREN);
        return sup;
    }

    // the part of lexerSpec, parserSpec, and treeParserSpec after "startXXX"
    void specRest(bool lexer) {
        match(ANTLR_SEMI);
        if (LA(1) == ANTLR_OPTIONS) {
            if (lexer)
                lexerOptionsSpec();
            else
                parserOptionsSpec();
        }
        behavior->endOptions();
        if (LA(1) == ANTLR_TOKENS)
            tokensSpec();
        if (LA(1) == ANTLR_ACTION) {
            behavior->refMemberAction(LT(1));
            match(ANTLR_ACTION);
        }
    }

    void lexerSpec(const OStr& doc) {
        TokenRef idTok;
        OStr sup;
        if (LA(1) == ANTLR_LITERAL_lexclass) {
            TokenRef lc = LT(1);
            match(ANTLR_LITERAL_lexclass);
            idTok = id();
            antlrTool->warning("lexclass' is deprecated; use 'class X extends Lexer'", filename, lc->line, lc->column);
        } else {
            match(ANTLR_LITERAL_class);
            idTok = id();
            match(ANTLR_LITERAL_extends);
            match(ANTLR_LITERAL_Lexer);
            if (LA(1) == ANTLR_LPAREN)
                sup = superClass();
        }
        behavior->startLexer(filename, idTok, sup, doc);
        specRest(true);
    }

    void treeParserSpec(const OStr& doc) {
        OStr sup;
        match(ANTLR_LITERAL_class);
        TokenRef idTok = id();
        match(ANTLR_LITERAL_extends);
        match(ANTLR_LITERAL_TreeParser);
        if (LA(1) == ANTLR_LPAREN)
            sup = superClass();
        behavior->startTreeWalker(filename, idTok, sup, doc);
        specRest(false);
    }

    void parserSpec(const OStr& doc) {
        OStr sup;
        match(ANTLR_LITERAL_class);
        TokenRef idTok = id();
        if (LA(1) == ANTLR_LITERAL_extends) {
            match(ANTLR_LITERAL_extends);
            match(ANTLR_LITERAL_Parser);
            if (LA(1) == ANTLR_LPAREN)
                sup = superClass();
        } else if (LA(1) == ANTLR_SEMI) {
            antlrTool->warning("use 'class X extends Parser'", filename, idTok->line, idTok->column);
        } else {
            throwNoViable();
        }
        behavior->startParser(filename, idTok, sup, doc);
        specRest(false);
    }

    void rules() {
        int cnt = 0;
        while (member(tokenSet_0, LA(1)) && member(tokenSet_1, LA(2))) {
            rule();
            ++cnt;
        }
        if (cnt == 0)
            throwNoViable();
    }

    TokenRef optionValue() {
        switch (LA(1)) {
        case ANTLR_TOKEN_REF:
        case ANTLR_RULE_REF:
            return qualifiedID();
        case ANTLR_STRING_LITERAL:
        case ANTLR_CHAR_LITERAL:
        case ANTLR_INT: {
            TokenRef t = LT(1);
            consume();
            return t;
        }
        default:
            throwNoViable();
        }
    }

    void parserOptionsSpec() {
        match(ANTLR_OPTIONS);
        while (LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) {
            TokenRef idTok = id();
            match(ANTLR_ASSIGN);
            TokenRef value = optionValue();
            behavior->setGrammarOption(idTok, value);
            match(ANTLR_SEMI);
        }
        match(ANTLR_RCURLY);
    }

    void lexerOptionsSpec() {
        match(ANTLR_OPTIONS);
        for (;;) {
            if (LA(1) == ANTLR_LITERAL_charVocabulary) {
                match(ANTLR_LITERAL_charVocabulary);
                match(ANTLR_ASSIGN);
                BitSet b = charSet();
                match(ANTLR_SEMI);
                behavior->setCharVocabulary(b);
            } else if (LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) {
                TokenRef idTok = id();
                match(ANTLR_ASSIGN);
                TokenRef value = optionValue();
                behavior->setGrammarOption(idTok, value);
                match(ANTLR_SEMI);
            } else {
                break;
            }
        }
        match(ANTLR_RCURLY);
    }

    BitSet charSet() {
        BitSet b = setBlockElement();
        while (LA(1) == ANTLR_OR) {
            match(ANTLR_OR);
            BitSet tmpSet = setBlockElement();
            b.orInPlace(tmpSet);
        }
        return b;
    }

    void subruleOptionsSpec() {
        match(ANTLR_OPTIONS);
        while (LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) {
            TokenRef idTok = id();
            match(ANTLR_ASSIGN);
            TokenRef value = optionValue();
            behavior->setSubruleOption(idTok, value);
            match(ANTLR_SEMI);
        }
        match(ANTLR_RCURLY);
    }

    TokenRef qualifiedID() {
        TokenRef a = id();
        std::string buf = a->text;
        while (LA(1) == ANTLR_WILDCARD) {
            match(ANTLR_WILDCARD);
            a = id();
            buf += '.';
            buf += a->text;
        }
        auto qidTok = makeTokenRef(ANTLR_TOKEN_REF, buf);
        qidTok->line = a->line;
        return qidTok;
    }

    BitSet setBlockElement() {
        TokenRef c1 = LT(1);
        match(ANTLR_CHAR_LITERAL);
        int rangeMin = tokenTypeForCharLiteral(c1->text);
        BitSet b = BitSet::of(rangeMin);
        if (LA(1) == ANTLR_RANGE) {
            match(ANTLR_RANGE);
            TokenRef c2 = LT(1);
            match(ANTLR_CHAR_LITERAL);
            int rangeMax = tokenTypeForCharLiteral(c2->text);
            if (rangeMax < rangeMin)
                antlrTool->error("Malformed range line ", filename, c1->line, c1->column);
            for (int i = rangeMin + 1; i <= rangeMax; i++)
                b.add(i);
        }
        return b;
    }

    void tokensSpec() {
        match(ANTLR_TOKENS);
        int cnt = 0;
        while (LA(1) == ANTLR_STRING_LITERAL || LA(1) == ANTLR_TOKEN_REF) {
            if (LA(1) == ANTLR_TOKEN_REF) {
                TokenRef s1;
                TokenRef t1 = LT(1);
                match(ANTLR_TOKEN_REF);
                if (LA(1) == ANTLR_ASSIGN) {
                    match(ANTLR_ASSIGN);
                    s1 = LT(1);
                    match(ANTLR_STRING_LITERAL);
                }
                behavior->defineToken(t1, s1);
                if (LA(1) == ANTLR_OPEN_ELEMENT_OPTION)
                    tokensSpecOptions(t1);
            } else {
                TokenRef s3 = LT(1);
                match(ANTLR_STRING_LITERAL);
                behavior->defineToken(nullptr, s3);
                if (LA(1) == ANTLR_OPEN_ELEMENT_OPTION)
                    tokensSpecOptions(s3);
            }
            match(ANTLR_SEMI);
            ++cnt;
        }
        if (cnt == 0)
            throwNoViable();
        match(ANTLR_RCURLY);
    }

    void tokensSpecOptions(const TokenRef& t) {
        match(ANTLR_OPEN_ELEMENT_OPTION);
        TokenRef o = id();
        match(ANTLR_ASSIGN);
        TokenRef v = optionValue();
        behavior->refTokensSpecElementOption(t, o, v);
        while (LA(1) == ANTLR_SEMI) {
            match(ANTLR_SEMI);
            o = id();
            match(ANTLR_ASSIGN);
            v = optionValue();
            behavior->refTokensSpecElementOption(t, o, v);
        }
        match(ANTLR_CLOSE_ELEMENT_OPTION);
    }

    void rule() {
        std::string access = "public";
        OStr doc;
        bool ruleAutoGen = true;
        blockNesting = -1;

        if (LA(1) == ANTLR_DOC_COMMENT) {
            doc = LT(1)->text;
            match(ANTLR_DOC_COMMENT);
        }
        if (LA(1) == ANTLR_LITERAL_protected || LA(1) == ANTLR_LITERAL_public || LA(1) == ANTLR_LITERAL_private) {
            access = LT(1)->text;
            consume();
        }
        TokenRef idTok = id();
        if (LA(1) == ANTLR_BANG) {
            match(ANTLR_BANG);
            ruleAutoGen = false;
        }
        behavior->defineRuleName(idTok, access, ruleAutoGen, doc);
        if (LA(1) == ANTLR_ARG_ACTION) {
            behavior->refArgAction(LT(1));
            match(ANTLR_ARG_ACTION);
        }
        if (LA(1) == ANTLR_LITERAL_returns) {
            match(ANTLR_LITERAL_returns);
            TokenRef rt = LT(1);
            match(ANTLR_ARG_ACTION);
            behavior->refReturnAction(rt);
        }
        if (LA(1) == ANTLR_LITERAL_throws)
            throwsSpec();
        if (LA(1) == ANTLR_OPTIONS)
            ruleOptionsSpec();
        if (LA(1) == ANTLR_ACTION) {
            behavior->refInitAction(LT(1));
            match(ANTLR_ACTION);
        }
        match(ANTLR_COLON);
        block();
        match(ANTLR_SEMI);
        if (LA(1) == ANTLR_LITERAL_exception)
            exceptionGroup();
        behavior->endRule(idTok->text);
    }

    void throwsSpec() {
        match(ANTLR_LITERAL_throws);
        TokenRef a = id();
        std::string t = a->text;
        while (LA(1) == ANTLR_COMMA) {
            match(ANTLR_COMMA);
            TokenRef b = id();
            t += "," + b->text;
        }
        behavior->setUserExceptions(t);
    }

    void ruleOptionsSpec() {
        match(ANTLR_OPTIONS);
        while (LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) {
            TokenRef idTok = id();
            match(ANTLR_ASSIGN);
            TokenRef value = optionValue();
            behavior->setRuleOption(idTok, value);
            match(ANTLR_SEMI);
        }
        match(ANTLR_RCURLY);
    }

    void block() {
        blockNesting++;
        alternative();
        while (LA(1) == ANTLR_OR) {
            match(ANTLR_OR);
            alternative();
        }
        blockNesting--;
    }

    void exceptionGroup() {
        behavior->beginExceptionGroup();
        while (LA(1) == ANTLR_LITERAL_exception)
            exceptionSpec();
        behavior->endExceptionGroup();
    }

    void alternative() {
        bool altAutoGen = true;
        if (LA(1) == ANTLR_BANG) {
            match(ANTLR_BANG);
            altAutoGen = false;
        }
        behavior->beginAlt(altAutoGen);
        while (member(tokenSet_2, LA(1)))
            element();
        if (LA(1) == ANTLR_LITERAL_exception)
            exceptionSpecNoLabel();
        else if (LA(1) != ANTLR_SEMI && LA(1) != ANTLR_OR && LA(1) != ANTLR_RPAREN)
            throwNoViable();
        behavior->endAlt();
    }

    void element() {
        elementNoOptionSpec();
        if (LA(1) == ANTLR_OPEN_ELEMENT_OPTION)
            elementOptionSpec();
    }

    void exceptionSpecNoLabel() {
        match(ANTLR_LITERAL_exception);
        behavior->beginExceptionSpec(nullptr);
        while (LA(1) == ANTLR_LITERAL_catch)
            exceptionHandler();
        behavior->endExceptionSpec();
    }

    void exceptionSpec() {
        TokenRef labelAction;
        match(ANTLR_LITERAL_exception);
        if (LA(1) == ANTLR_ARG_ACTION) {
            labelAction = LT(1);
            match(ANTLR_ARG_ACTION);
        }
        behavior->beginExceptionSpec(labelAction);
        while (LA(1) == ANTLR_LITERAL_catch)
            exceptionHandler();
        behavior->endExceptionSpec();
    }

    void exceptionHandler() {
        match(ANTLR_LITERAL_catch);
        TokenRef a1 = LT(1);
        match(ANTLR_ARG_ACTION);
        TokenRef a2 = LT(1);
        match(ANTLR_ACTION);
        behavior->refExceptionHandler(a1, a2);
    }

    // optional '!' after a rule reference
    int optionalBang() {
        if (LA(1) == ANTLR_BANG) {
            match(ANTLR_BANG);
            return AUTO_GEN_BANG;
        }
        return AUTO_GEN_NONE;
    }

    void elementNoOptionSpec() {
        TokenRef label;
        TokenRef assignId;
        TokenRef args;
        int autoGen = AUTO_GEN_NONE;

        switch (LA(1)) {
        case ANTLR_ACTION:
            behavior->refAction(LT(1));
            match(ANTLR_ACTION);
            return;
        case ANTLR_SEMPRED:
            behavior->refSemPred(LT(1));
            match(ANTLR_SEMPRED);
            return;
        case ANTLR_TREE_BEGIN:
            tree();
            return;
        default:
            break;
        }

        if ((LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) && LA(2) == ANTLR_ASSIGN) {
            assignId = id();
            match(ANTLR_ASSIGN);
            if ((LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) && LA(2) == ANTLR_COLON) {
                label = id();
                match(ANTLR_COLON);
                checkForMissingEndRule(label);
            } else if (!((LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) && member(tokenSet_3, LA(2)))) {
                throwNoViable();
            }
            if (LA(1) == ANTLR_RULE_REF) {
                TokenRef rr = LT(1);
                match(ANTLR_RULE_REF);
                if (LA(1) == ANTLR_ARG_ACTION) {
                    args = LT(1);
                    match(ANTLR_ARG_ACTION);
                }
                autoGen = optionalBang();
                behavior->refRule(assignId, rr, label, args, autoGen);
            } else if (LA(1) == ANTLR_TOKEN_REF) {
                TokenRef tr = LT(1);
                match(ANTLR_TOKEN_REF);
                if (LA(1) == ANTLR_ARG_ACTION) {
                    args = LT(1);
                    match(ANTLR_ARG_ACTION);
                }
                behavior->refToken(assignId, tr, label, args, false, autoGen, lastInRule());
            } else {
                throwNoViable();
            }
        } else if (member(tokenSet_4, LA(1)) && member(tokenSet_5, LA(2))) {
            if ((LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) && LA(2) == ANTLR_COLON) {
                label = id();
                match(ANTLR_COLON);
                checkForMissingEndRule(label);
            } else if (!(member(tokenSet_4, LA(1)) && member(tokenSet_6, LA(2)))) {
                throwNoViable();
            }
            switch (LA(1)) {
            case ANTLR_RULE_REF: {
                TokenRef r2 = LT(1);
                match(ANTLR_RULE_REF);
                if (LA(1) == ANTLR_ARG_ACTION) {
                    args = LT(1);
                    match(ANTLR_ARG_ACTION);
                }
                autoGen = optionalBang();
                behavior->refRule(assignId, r2, label, args, autoGen);
                break;
            }
            case ANTLR_NOT_OP:
                match(ANTLR_NOT_OP);
                if (LA(1) == ANTLR_CHAR_LITERAL || LA(1) == ANTLR_TOKEN_REF)
                    notTerminal(label);
                else if (LA(1) == ANTLR_LPAREN)
                    ebnf(label, true);
                else
                    throwNoViable();
                break;
            case ANTLR_LPAREN:
                ebnf(label, false);
                break;
            default:
                if ((LA(1) == ANTLR_STRING_LITERAL || LA(1) == ANTLR_CHAR_LITERAL || LA(1) == ANTLR_TOKEN_REF) && LA(2) == ANTLR_RANGE)
                    range(label);
                else if (member(tokenSet_7, LA(1)) && member(tokenSet_8, LA(2)))
                    terminal(label);
                else
                    throwNoViable();
            }
        } else {
            throwNoViable();
        }
    }

    void elementOptionSpec() {
        match(ANTLR_OPEN_ELEMENT_OPTION);
        TokenRef o = id();
        match(ANTLR_ASSIGN);
        TokenRef v = optionValue();
        behavior->refElementOption(o, v);
        while (LA(1) == ANTLR_SEMI) {
            match(ANTLR_SEMI);
            o = id();
            match(ANTLR_ASSIGN);
            v = optionValue();
            behavior->refElementOption(o, v);
        }
        match(ANTLR_CLOSE_ELEMENT_OPTION);
    }

    void range(const TokenRef& label) {
        if (LA(1) == ANTLR_CHAR_LITERAL) {
            TokenRef crLeft = LT(1);
            match(ANTLR_CHAR_LITERAL);
            match(ANTLR_RANGE);
            TokenRef crRight = LT(1);
            match(ANTLR_CHAR_LITERAL);
            int autoGen = optionalBang();
            behavior->refCharRange(crLeft, crRight, label, autoGen, lastInRule());
        } else {
            TokenRef trLeft = LT(1);
            if (LA(1) != ANTLR_TOKEN_REF && LA(1) != ANTLR_STRING_LITERAL)
                throwNoViable();
            consume();
            match(ANTLR_RANGE);
            TokenRef trRight = LT(1);
            if (LA(1) != ANTLR_TOKEN_REF && LA(1) != ANTLR_STRING_LITERAL)
                throwNoViable();
            consume();
            int autoGen = ast_type_spec();
            behavior->refTokenRange(trLeft, trRight, label, autoGen, lastInRule());
        }
    }

    void terminal(const TokenRef& label) {
        int autoGen = AUTO_GEN_NONE;
        TokenRef args;
        switch (LA(1)) {
        case ANTLR_CHAR_LITERAL: {
            TokenRef cl = LT(1);
            match(ANTLR_CHAR_LITERAL);
            autoGen = optionalBang();
            behavior->refCharLiteral(cl, label, false, autoGen, lastInRule());
            break;
        }
        case ANTLR_TOKEN_REF: {
            TokenRef tr = LT(1);
            match(ANTLR_TOKEN_REF);
            autoGen = ast_type_spec();
            if (LA(1) == ANTLR_ARG_ACTION) {
                args = LT(1);
                match(ANTLR_ARG_ACTION);
            }
            behavior->refToken(nullptr, tr, label, args, false, autoGen, lastInRule());
            break;
        }
        case ANTLR_STRING_LITERAL: {
            TokenRef sl = LT(1);
            match(ANTLR_STRING_LITERAL);
            autoGen = ast_type_spec();
            behavior->refStringLiteral(sl, label, autoGen, lastInRule());
            break;
        }
        case ANTLR_WILDCARD: {
            TokenRef wi = LT(1);
            match(ANTLR_WILDCARD);
            autoGen = ast_type_spec();
            behavior->refWildcard(wi, label, autoGen);
            break;
        }
        default:
            throwNoViable();
        }
    }

    void notTerminal(const TokenRef& label) {
        if (LA(1) == ANTLR_CHAR_LITERAL) {
            TokenRef cl = LT(1);
            match(ANTLR_CHAR_LITERAL);
            int autoGen = optionalBang();
            behavior->refCharLiteral(cl, label, true, autoGen, lastInRule());
        } else if (LA(1) == ANTLR_TOKEN_REF) {
            TokenRef tr = LT(1);
            match(ANTLR_TOKEN_REF);
            int autoGen = ast_type_spec();
            behavior->refToken(nullptr, tr, label, nullptr, true, autoGen, lastInRule());
        } else {
            throwNoViable();
        }
    }

    void ebnf(const TokenRef& label, bool not_) {
        TokenRef lp = LT(1);
        match(ANTLR_LPAREN);
        behavior->beginSubRule(label, lp, not_);
        if (LA(1) == ANTLR_OPTIONS) {
            subruleOptionsSpec();
            if (LA(1) == ANTLR_ACTION) {
                behavior->refInitAction(LT(1));
                match(ANTLR_ACTION);
            }
            match(ANTLR_COLON);
        } else if (LA(1) == ANTLR_ACTION && LA(2) == ANTLR_COLON) {
            behavior->refInitAction(LT(1));
            match(ANTLR_ACTION);
            match(ANTLR_COLON);
        } else if (!(member(tokenSet_9, LA(1)) && member(tokenSet_10, LA(2)))) {
            throwNoViable();
        }
        block();
        match(ANTLR_RPAREN);
        if (LA(1) == ANTLR_IMPLIES) {
            match(ANTLR_IMPLIES);
            behavior->synPred();
        } else {
            if (LA(1) == ANTLR_QUESTION) {
                match(ANTLR_QUESTION);
                behavior->optionalSubRule();
            } else if (LA(1) == ANTLR_STAR) {
                match(ANTLR_STAR);
                behavior->zeroOrMoreSubRule();
            } else if (LA(1) == ANTLR_PLUS) {
                match(ANTLR_PLUS);
                behavior->oneOrMoreSubRule();
            }
            if (LA(1) == ANTLR_BANG) {
                match(ANTLR_BANG);
                behavior->noASTSubRule();
            }
        }
        behavior->endSubRule();
    }

    void tree() {
        TokenRef lp = LT(1);
        match(ANTLR_TREE_BEGIN);
        behavior->beginTree(lp);
        rootNode();
        behavior->beginChildList();
        int cnt = 0;
        while (member(tokenSet_2, LA(1))) {
            element();
            ++cnt;
        }
        if (cnt == 0)
            throwNoViable();
        behavior->endChildList();
        match(ANTLR_RPAREN);
        behavior->endTree();
    }

    void rootNode() {
        TokenRef label;
        if ((LA(1) == ANTLR_TOKEN_REF || LA(1) == ANTLR_RULE_REF) && LA(2) == ANTLR_COLON) {
            label = id();
            match(ANTLR_COLON);
            checkForMissingEndRule(label);
        } else if (!(member(tokenSet_7, LA(1)) && member(tokenSet_11, LA(2)))) {
            throwNoViable();
        }
        terminal(label);
    }

    int ast_type_spec() {
        if (LA(1) == ANTLR_CARET) {
            match(ANTLR_CARET);
            return AUTO_GEN_CARET;
        }
        if (LA(1) == ANTLR_BANG) {
            match(ANTLR_BANG);
            return AUTO_GEN_BANG;
        }
        return AUTO_GEN_NONE;
    }

    MakeGrammar* behavior;
    Tool* antlrTool;
    int blockNesting = -1;
};

} // namespace

void parseGrammar(const std::string& input, const std::string& filename, MakeGrammar* behavior, Tool* tool) {
    ANTLRLexer lexer(input);
    lexer.filename = filename;
    ANTLRParser parser(lexer, behavior, tool);
    parser.filename = filename;
    try {
        parser.grammar();
    } catch (const RecognitionException& ex) {
        tool->fatalError("Unhandled parser error: " + ex.message);
    } catch (const TokenStreamException& ex) {
        tool->error(ex.error.message, ex.error.filename, ex.error.line, ex.error.column);
        tool->fatalError("Exiting due to errors.");
    }
}

} // namespace antlrcpp
