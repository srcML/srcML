/**
 * @file srcMLState.hpp
 *
 * @copyright Copyright (C) 2003-2014  SDML (www.srcML.org)
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

class srcMLState : public State {
public:

    srcMLState(const State::MODE_TYPE& mode, const State::MODE_TYPE& transmode, const State::MODE_TYPE& prevmode)
        : State(mode, transmode, prevmode), parencount(0), curlycount(0), typecount(0)
    {}

    srcMLState()
        : State(), parencount(0), curlycount(0), typecount(0)
    {}

    // parentheses count
    int getParen() const {
        return parencount;
    }

    // increment the parentheses count
    void incParen() {
        ++parencount;
    }

    // decrement the parentheses count
    void decParen() {
        --parencount;
    }

    // curly count
    int getCurly() const {
        return curlycount;
    }

    // increment the curly count
    void incCurly() {
        ++curlycount;
    }

    // decrement the curly count
    void decCurly() {
        --curlycount;
    }

    // type count
    int getTypeCount() const {
        return typecount;
    }

    // set type count
    void setTypeCount(int n) {
        typecount = n;
    }

    // increment the type count
    void incTypeCount() {
        ++typecount;
    }

    // decrement the type count
    void decTypeCount() {
        --typecount;
    }

    ~srcMLState() {
    }

private:
    int parencount;
    int curlycount;
    int typecount;
};

#endif
