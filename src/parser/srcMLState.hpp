// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file srcMLState.hpp
 *
 * @copyright Copyright (C) 2003-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#ifndef SRCMLSTATE_HPP
#define SRCMLSTATE_HPP

#include <stack>
#include <vector>
#include <algorithm>

#include <srcMLException.hpp>
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
    typedef std::bitset<256> MODE_TYPE;

    /**
     * stack of open elements
     * Inline storage for shallow stacks avoids an allocation for each new stack.
     * Elements are accessed by position, since pointers to them do not stay valid.
     */
    class OpenElements {
    public:
        bool empty() const { return count == 0; }
        size_t size() const { return count; }
        int& top() { return data()[count - 1]; }
        int& operator[](size_t pos) { return data()[pos]; }

        void push(int id) {
            if (count < INLINE_SIZE) {
                local[count] = id;
            } else {
                // move inline elements to the heap when first needed
                if (count == INLINE_SIZE)
                    heap.assign(local, local + INLINE_SIZE);
                heap.push_back(id);
            }
            ++count;
        }

        void pop() {
            --count;
            if (count >= INLINE_SIZE) {
                heap.pop_back();

                // back to inline elements
                if (count == INLINE_SIZE) {
                    std::copy(heap.begin(), heap.end(), local);
                    heap.clear();
                }
            }
        }

    private:
        static constexpr size_t INLINE_SIZE = 4;

        int* data() { return count > INLINE_SIZE ? heap.data() : local; }

        size_t count = 0;
        int local[INLINE_SIZE] = {};
        std::vector<int> heap;
    };

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

    // copy constructor
    srcMLState(const srcMLState&) = default;

    // copy assignment
    srcMLState& operator=(const srcMLState&) = default;

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
    OpenElements openelements;

private:

    /** open parenthesis count */
    int parencount;

    /** open curly brace count */
    int curlycount;

    /** current type count */
    int typecount;

};

#endif
