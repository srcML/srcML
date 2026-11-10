// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLToken.hpp
 *
 * @copyright Copyright (C) 2004-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * Specialized token for srcML markup elements.
 */

#ifndef INCLUDED_SRCMLTOKEN_HPP
#define INCLUDED_SRCMLTOKEN_HPP

#include <ANTLRRuntime.hpp>
#include <vector>

/** anonymous enum for srcML token categories (xml based) */
enum { STARTTOKEN = 0, ENDTOKEN = 50, EMPTYTOKEN = 75 };

/**
 * srcMLToken
 *
 * Class providing addition information over antlr token (srcML tags).
 */
class srcMLToken : public antlr::Token {

    /** isstart is friend function */
    friend bool isstart(const antlr::RefToken& token);

    /** isempty is friend function */
    friend bool isempty(const antlr::RefToken& token);

public:

    /**
     * srcMLToken
     *
     * Constructor.  By default not xml tag.
     */
    srcMLToken()
        : Token(), category(-1) {
    }

    /**
     * srcMLToken
     * @param t token number
     * @param cat token categor
     *
     * Constructor.  By default not xml tag.
     */
    srcMLToken(int t, int cat)
        : Token(t), category(cat) {
    }

    /**
     * factory
     *
     * Factory method for creating srcMLToken.
     *
     * @returns the created srcMLToken.
     */
    static antlr::RefToken factory() {
        return new srcMLToken();
    }

    /**
     * setLine
     * @param l line number
     *
     * Set the line number to l.
     */
    virtual void setLine(int l) final { line = l; }

    /**
     * getLine
     *
     * Get the line number.
     *
     * @returns the line number.
     */
    virtual int getLine() const final { return line; }
    virtual int getEndLine() const final { return endline; }

    /**
     * setColumn
     * @param c column number
     *
     * Set the column number to c.
     */
    virtual void setColumn(int c) final { column = c; }
    virtual int getEndColumn() const final { return endcolumn; }

    /**
     * getColumn
     *
     * Get the column number.
     *
     * @returns the column number.
     */
    virtual int getColumn() const final { return column; }

    /**
     * getText
     *
     * Get the current text of the token.
     *
     * @returns the current text of the token.
     */
    virtual const std::string& getText() const final { return text; }

    /**
     * setText
     * @param s the text to set on the token
     *
     * Set the current text of the token.
     */
    virtual void setText(std::string_view s) final { text = s; }

    /**
     * ~srcMLToken
     *
     * Destructor.
     */
    virtual ~srcMLToken() {}

    /**
     * operator new
     * @param size size of the token
     *
     * Reuse the memory of deleted tokens, since tokens are created and deleted constantly.
     */
    static void* operator new(std::size_t size) {
        auto& pool = freeTokens();
        if (size == sizeof(srcMLToken) && !pool.blocks.empty()) {
            void* p = pool.blocks.back();
            pool.blocks.pop_back();
            return p;
        }
        return ::operator new(size);
    }

    /**
     * operator delete
     * @param p memory of the token
     * @param size size of the token
     *
     * Keep the memory of deleted tokens for reuse.
     */
    static void operator delete(void* p, std::size_t size) {
        auto& pool = freeTokens();
        if (size == sizeof(srcMLToken) && pool.blocks.size() < MAX_FREE_TOKENS) {
            pool.blocks.push_back(p);
            return;
        }
        ::operator delete(p);
    }

    /** tokens categeory */
    int category = -1;

    /** The current line number */
    int line = 0;

    /** the current column */
    int column = 0;

    int endline = 0;
    int endcolumn = 0;

    /** the tokens text */
    std::string text;

private:

    /** maximum number of deleted tokens kept for reuse */
    static constexpr std::size_t MAX_FREE_TOKENS = 4096;

    /** memory of deleted tokens, per thread */
    struct FreeTokens {
        std::vector<void*> blocks;

        ~FreeTokens() {
            for (auto p : blocks)
                ::operator delete(p);
        }
    };

    static FreeTokens& freeTokens() {
        thread_local FreeTokens pool;
        return pool;
    }
};

/**
 * EndTokenFactory
 *
 * Factory for creating an end element tag token.
 *
 * @returns the created end token.
 */
inline srcMLToken* EndTokenFactory(int token) {

    return new srcMLToken(token, ENDTOKEN);
}

/**
 * EmptyTokenFactory
 *
 * Factory for creating an empty element tag token.
 *
 * @returns the created empty token.
 */
inline srcMLToken* EmptyTokenFactory(int token) {

    return new srcMLToken(token, EMPTYTOKEN);
}

/**
 * StartTokenFactory
 *
 * Factory for creating a start element tag token.
 *
 * @returns the created start token.
 */
inline srcMLToken* StartTokenFactory(int token) {

    return new srcMLToken(token, STARTTOKEN);
}

/**
 * isstart
 *
 * Predicate to query if token is a start element tag token.
 *
 * @returns if is a start token.
 */
inline bool isstart(const antlr::RefToken& token) {

    return static_cast<const srcMLToken*>(&(*token))->category == STARTTOKEN ||
           static_cast<const srcMLToken*>(&(*token))->category == EMPTYTOKEN;
}

inline bool ispurestart(const antlr::RefToken& token) {

    return static_cast<const srcMLToken*>(&(*token))->category == STARTTOKEN;
}

/**
 * isempty
 *
 * Predicate to query if token is an empty element tag token.
 *
 * @returns if is an empty token.
 */
inline bool isempty(const antlr::RefToken& token) {

    return static_cast<const srcMLToken*>(&(*token))->category == EMPTYTOKEN;
}

inline bool isend(const antlr::RefToken& token) {

    return static_cast<const srcMLToken*>(&(*token))->category == ENDTOKEN;
}

#endif
