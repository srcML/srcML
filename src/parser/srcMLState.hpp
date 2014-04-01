/**
 * @file srcMLState.hpp
 *
 * @copyright Copyright (C) 2003-2014 SDML (www.srcML.org)
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

#include "State.hpp"

/**
 * srcMLState
 *
 * Class to hold srcML state e.g. parenthesis/curly braces/type counts.
 * Builds off of state holding a mode.
 */
class srcMLState : public State {
public:

    /** 
     * srcMLState
     * @param mode current mode to create
     * @param transmode the transparent mode
     * @param prevmode previous mode.
     *
     * Constructor.  Create a new mode with the perscribed mode, transparent mode, and previous mode.
     */
    srcMLState(const State::MODE_TYPE& mode, const State::MODE_TYPE& transmode, const State::MODE_TYPE& prevmode)
        : State(mode, transmode, prevmode), parencount(0), curlycount(0), typecount(0)
    {}

    /**
     * srcMLState
     *
     * Constructor.  Default.
     */
    srcMLState()
        : State(), parencount(0), curlycount(0), typecount(0)
    {}

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

private:

    /** open parenthesis count */
    int parencount;

    /** open curly brace count */
    int curlycount;

    /** current type count */
    int typecount;
    
};

#endif
