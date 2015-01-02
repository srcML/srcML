/**
 * @file srcMLState.hpp
 *
 * @copyright Copyright (C) 2003-2014 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef SRCMLSTATE_HPP
#define SRCMLSTATE_HPP

#include <stack>

#include "srcMLException.hpp"
#include <bitset>

/**
 * srcMLState
 *
 * Class to hold srcML state e.g. parenthesis/curly braces/type counts.
 * Also, holds modes.
 */
class srcMLState {

public:

    /** type of mode for GNU */
    typedef std::bitset<128> MODE_TYPE;

    /** 
     * srcMLState
     * @param mode current mode to create
     * @param transmode the transparent mode
     * @param prevmode previous mode.
     *
     * Constructor.  Create a new mode with the perscribed mode, transparent mode, and previous mode.
     */
    srcMLState(const MODE_TYPE& mode = 0, const MODE_TYPE& transmode = 0, const MODE_TYPE& prevmode = 0)
        : flags(mode), flags_prev(prevmode), flags_all(transmode | mode), parencount(0), curlycount(0), typecount(0)
    {}

    /**
     * size
     *
     * Get number of open elements.
     *
     * @returns the number of open elements.
     */
    int size() const {
        return (int)openelements.size();
    }

    /**
     * push
     * @param id number of tag to add to open elements
     *
     * Add open element to stack.
     */
    void push(int id) {
        openelements.push(id);
    }

    /**
     * pop
     *
     * Remove an open element from stack.
     */
    void pop() {
        if (openelements.empty())
            throw Segmentation_Fault();

        openelements.pop();
    }

    // mode methods

    /**
     * inMode
     * @param m a mode to test if in
     *
     * Predicate to test if in modes m.
     *
     * @returns if in modes m.
     */
    bool inMode(const MODE_TYPE& m) const {

        return (flags & m) == m;
    }

    /**
     * inPrevMode
     * @param m a mode to test if in
     *
     * Predicate to test if in the previous modes m.
     *
     * @returns if in previous modes m.
     */
     bool inPrevMode(const MODE_TYPE& m) const {

        return (flags_prev & m) == m;
    }
    /**
     * inTransaprentMode
     * @param m a mode to test if in
     *
     * Predicate to test if in transparent modes m.
     *
     * @returns if in transparent modes m.
     */
    bool inTransparentMode(const MODE_TYPE& m) const {

        return (flags_all & m) == m;
    }

    /**
     * getMode
     *
     * Get the current mode.
     *
     * @returns the current mode.
     */
    const MODE_TYPE& getMode() const {
        return flags;
    }

    /**
     * getTransparentMode
     *
     * Get the current transparent mode.
     *
     * @returns the current transparent mode.
     */
    const MODE_TYPE& getTransparentMode() const {
        return flags_all;
    }

    /**
     * setMode
     * @param m1 mode to set in addition to current mdoes
     *
     * Set the mode and transparent mode to have m1 modes in addition
     * to the modes they already have.
     */
    void setMode(const MODE_TYPE& m1) {

        flags |= m1;
        flags_all |= m1;
    }

    /**
     * clearMode
     * @param m modes to clear/unset
     *
     * Clear/unset only the modes m from mode and transparent mode.
     */
     void clearMode(const MODE_TYPE& m) {

        flags &= ~m;
        flags_all &= ~m;
    }

    /**
     * getParen
     *
     * Get the open parenthesis count
     *
     * @returns the current open parenthesis count.
     */
    int getParen() const {
        return parencount;
    }

    /**
     * incParen
     *
     * Increment the open parenthesis count
     */
    void incParen() {
        ++parencount;
    }

    /**
     * decParen
     *
     * Decrement the open parenthesis count
     */
    void decParen() {
        --parencount;
    }

    /**
     * getCurly
     *
     * Get the open curly brace count
     *
     * @returns the current open curly brace count.
     */
    int getCurly() const {
        return curlycount;
    }

    /**
     * incCurly
     *
     * Increment the open curly brace count
     */
    void incCurly() {
        ++curlycount;
    }

    /**
     * decCurly
     *
     * Decrement the open curly brace count
     */
    void decCurly() {
        --curlycount;
    }

    /**
     * getTypeCount
     *
     * Get the type count
     *
     * @returns the current type count.
     */
    int getTypeCount() const {
        return typecount;
    }

    /**
     * setTypeCount
     * @param n number of types to set
     *
     * Set the type count
     */
    void setTypeCount(int n) {
        typecount = n;
    }

    /**
     * incTypeCount
     *
     * Increment the type count
     */
    void incTypeCount() {
        ++typecount;
    }

    /**
     * decTypeCount
     *
     * Decrement the type count
     */
    void decTypeCount() {
        --typecount;
    }

    /**
     * ~srcMLState
     *
     * Destructor.
     */
    ~srcMLState() {
    }

public:

    /** current mode */
    MODE_TYPE flags;

    /** previous mode */
    MODE_TYPE flags_prev;

    /** transparent (all) modes */
    MODE_TYPE flags_all;

    /** stack of open elements */
    std::stack<int> openelements;

private:

    /** open parenthesis count */
    int parencount;

    /** open curly brace count */
    int curlycount;

    /** current type count */
    int typecount;
    
};

#endif
