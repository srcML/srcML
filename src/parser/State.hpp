/**
 * @file State.hpp
 *
 * @copyright Copyright (C) 2004-2010 SDML (www.srcML.org)
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

#ifndef STATE_HPP
#define STATE_HPP

#include <stack>

#include "srcMLException.hpp"

class State {
public:

#ifdef __GNUC__
    typedef unsigned long long MODE_TYPE;
#else
    typedef unsigned __int64 MODE_TYPE;
#endif

    State(const MODE_TYPE& mode = 0, const MODE_TYPE& transmode = 0, const MODE_TYPE& prevmode = 0)
        : flags(mode), flags_prev(prevmode), flags_all(transmode | mode)
    {}

    // openelements methods
    int size() const {
        return (int)openelements.size();
    }

    void push(int id) {
        openelements.push(id);
    }

    void pop() {
        if (openelements.empty())
            throw Segmentation_Fault();

        openelements.pop();
    }

    // mode methods

    bool inMode(const MODE_TYPE& m) const {

        return (flags & m) == m;
    }

    bool inPrevMode(const MODE_TYPE& m) const {

        return (flags_prev & m) == m;
    }

    bool inTransparentMode(const MODE_TYPE& m) const {

        return (flags_all & m) == m;
    }

    const MODE_TYPE& getMode() const {
        return flags;
    }

    const MODE_TYPE& getTransparentMode() const {
        return flags_all;
    }

    void setMode(const MODE_TYPE& m1) {

        flags |= m1;
        flags_all |= m1;
    }

    void clearMode(const MODE_TYPE& m) {

        flags &= ~m;
        flags_all &= ~m;
    }

    ~State() {
    }

public:
    MODE_TYPE flags;
    MODE_TYPE flags_prev;
    MODE_TYPE flags_all;

    std::stack<int> openelements;
};

#endif
