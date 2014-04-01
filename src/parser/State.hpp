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

/**
 * State
 *
 * Class representing a state of modes (mode, transparent mode, and previous mode).
 * Also, associates open elements with this mode. 
 */
class State {
public:

#ifdef __GNUC__

    /** type of mode for GNU */
    typedef unsigned long long MODE_TYPE;

#else

    /** type of mode for non-GNU */
    typedef unsigned __int64 MODE_TYPE;

#endif

    /**
     * State
     * @param mode the current mode
     * @param transmode the transparent mode (all active modes)
     * @param prevmode the previous mode.
     *
     * Constructor.  Create state with mode, transparent mode and previous mode if provided.
     */
    State(const MODE_TYPE& mode = 0, const MODE_TYPE& transmode = 0, const MODE_TYPE& prevmode = 0)
        : flags(mode), flags_prev(prevmode), flags_all(transmode | mode)
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
     * ~State
     *
     * Destructor.
     */
    ~State() {
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

};

#endif
