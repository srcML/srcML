// SPDX-License-Identifier: GPL-3.0-only
/**
 * @file ModeStack.hpp
 *
 * @copyright Copyright (C) 2004-2024 srcML, LLC. (www.srcML.org)
 *
 * This file is part of the srcML Toolkit.
 */

#ifndef MODE_STACK_HPP
#define MODE_STACK_HPP

#include <TokenParser.hpp>
#include <srcMLState.hpp>
#include <vector>

/**
 * ModeStack
 *
 * Class representing a stack of modes that direct parsing.
 * Modes also keep srcMLState such as open parethesis/curly braces/tags.
 */
class ModeStack : public TokenParser {

public:

    #include <Mode.hpp>

   /**
     * ModeStack
     * @param ptp the token parser
     *
     * Constructor.  Create mode stack from TokenParser and current language.
     */
    ModeStack()
    {}

    /**
     * ~ModeStack
     *
     * Destructor
     */
    ~ModeStack() {}

     /** token parser */
    TokenParser* parser;

    /** list of states/modes */
    typedef std::vector<srcMLState> StateList;

    /** stack of states/modes */
    StateList st;

    /** open elements of all the modes, where each mode has the elements from its start */
    std::vector<int> elements;

    // the modes and transparent modes of the top state, or none without a state, so a mode test is one load from the
    // parser instead of from the top of the stack, after a check for an empty stack
    srcMLState::MODE_TYPE topMode;
    srcMLState::MODE_TYPE topTransparentMode;

    // the modes of the top state, after the stack changes other than as the mode operations here change it
    void syncTopModes() {
        if (st.empty()) {
            topMode = 0;
            topTransparentMode = 0;
            return;
        }

        topMode = st.back().getMode();
        topTransparentMode = st.back().getTransparentMode();
    }

protected:

    /**
     * currentState
     *
     * Get the current state (const no modification).
     *
     * @returns the current state.
     */
    const srcMLState& currentState() const {
        if (st.empty())
            throw Segmentation_Fault();

        return st.back();
    }

    /**
     * currentState
     *
     * Delegate to get the current state (allow modification).
     *
     * @returns the current state.
     */
    srcMLState& currentState() {
        if (st.empty())
            throw Segmentation_Fault();

        return st.back();
    }

    /**
     * startNewMode
     * @param m new modes to start
     *
     * Delegate to create a new mode and place it on top of stack.
     */
    void startNewMode(const srcMLState::MODE_TYPE& m) {

        const unsigned int start = (unsigned int) elements.size();
        st.push_back(srcMLState(m, !empty() ? getTransparentMode() : 0, !empty() ? getMode() : 0));
        st.back().elementsStart = start;
        syncTopModes();
    }

    /**
     * endMode
     *
     * Delegate to remove the current mode (pop from stack).
     */
    void endMode() {

        if (st.size() == 1)
            throw Segmentation_Fault();

        popMode();
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * endMode
     * @param m mode to end
     *
     * Delegate to remove the current mode m (pop from stack).
     * Does not actually check or use m.
     */
    void endMode(const srcMLState::MODE_TYPE& /* m */) {

        if (st.size() <= 1)
            throw Segmentation_Fault();

        popMode();
    }

#pragma GCC diagnostic pop

    /**
     * endLastMode
     *
     * Delegate to ends a mode (not necessarily last) popping from stack.
     */
    void endLastMode() {

        popMode();
    }

    /**
     * getMode
     *
     * Delegate to get the current mode from stack (top of stack).
     *
     * @returns the current mode.
     */
    srcMLState::MODE_TYPE getMode() const {

        return topMode;
    }

    /**
     * getTransparentMode
     *
     * Delegate to get the transparent mode from stack (top of stack).
     *
     * @returns the transparent mode.
     */
    srcMLState::MODE_TYPE getTransparentMode() const {

        return topTransparentMode;
    }

    /**
     * getFirstMode
     * @param m modes to query for
     *
     * Get the first mode matching m
     *
     * @returns the matching mode
     */
    srcMLState::MODE_TYPE getFirstMode(const srcMLState::MODE_TYPE& m) const {

        for(StateList::const_reverse_iterator citr = st.rbegin(); citr != st.rend(); ++citr) {

            if((citr->getMode() & m) != 0) return citr->getMode();

        }

        return 0;

    }

    /**
     * setMode
     * @param m modes to add to current mode
     *
     * Add the modes m to the current modes.
     */
    void setMode(const srcMLState::MODE_TYPE& m) {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().setMode(m);
        topMode |= m;
        topTransparentMode |= m;
    }

    /**
     * clearMode
     * @param m modes to remove modes from current mode
     *
     * Delegate to removes the modes m from the current modes.
     */
     void clearMode(const srcMLState::MODE_TYPE& m) {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().clearMode(m);
        topMode &= ~m;
        topTransparentMode &= ~m;
    }

    /**
     * push
     * @param id of open element to add to current mode open element stack
     *
     * Delegate to add the open element id to the top of the open element of the current mode.
     */
    void push(int id) {
        if (st.empty())
            throw Segmentation_Fault();

        elements.push_back(id);
    }

    /**
     * pop
     *
     * Delegate to remove the top open element form the current mode.
     */
     void pop() {
        if (st.empty() || elements.size() <= st.back().elementsStart)
            throw Segmentation_Fault();

        elements.pop_back();
    }

    /**
     * elementCount
     *
     * Get the number of open elements of the current mode.
     *
     * @returns the number of open elements.
     */
    int elementCount() const {
        return (int) (elements.size() - currentState().elementsStart);
    }

    /**
     * topElement
     *
     * Get the last open element of the current mode.
     *
     * @returns the open element.
     */
    int topElement() const {
        return elements.back();
    }

    /**
     * canPush
     *
     * Whether an element can be pushed without growing the open elements.
     */
    bool canPush() const {
        return !st.empty() && elements.data() + elements.size() < elements.data() + elements.capacity();
    }

    /**
     * size
     *
     * Delegate to get the current modes number of open elements
     *
     * @returns the number of open element in current mode.
     */
    int size() const {

        return (int)st.size();
    }

    /**
     * empty
     *
     * Delegate Predicte method to test if state stack is empty
     *
     * @returns if states in stack.
     */
    bool empty() const {

        return st.empty();
    }

   /**
     * inMode
     * @param m mode to test if currently in
     *
     * Delegate predicte method to test if currently in mode m.
     *
     * @returns if in mode m.
     */
    bool inMode(const srcMLState::MODE_TYPE& m) const {

        return (topMode & m) == m;
    }

    /**
     * inPrevMode
     * @param m mode to test if previously in
     *
     * Delegate predicte method to test if previously in mode m.
     *
     * @returns if in previous mode m.
     */
    bool inPrevMode(const srcMLState::MODE_TYPE& m) const {

        return st.size() > 1 ? st.back().inPrevMode(m) : false;
    }

     /**
     * inTransparentMode
     * @param m mode to test if transaprently in
     *
     * Delegate predicte method to test if transparently in mode m.
     *
     * @returns if in transparent mode m.
     */
    bool inTransparentMode(const srcMLState::MODE_TYPE& m) const {

        return (topTransparentMode & m) == m;
    }

    /**
     * getParen
     *
     * Delegate to get the open parenthesis in current mode.
     *
     * @returns number of open parethesis in current mode
     */
    int getParen() const {
        return !st.empty() ? st.back().getParen() : 0;
    }

    /**
     * incParen
     *
     * Delegate to increment the number of open parenthesis.
     */
    void incParen() {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().incParen();
    }

    /**
     * decParen
     *
     * Delegate to decrement the number of open parenthesis.
     */
    void decParen() {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().decParen();
    }

    /**
     * getCurly
     *
     * Delegate to get the open curly braces in current mode.
     *
     * @returns number of open curly braces in current mode
     */
    int getCurly() const {
        return !st.empty() ? st.back().getCurly() : 0;
    }

    /**
     * incCurly
     *
     * Delegate to increment the number of open curly braces.
     */
    void incCurly() {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().incCurly();
    }

    /**
     * decCurly
     *
     * Delegate to decrement the number of open curly braces.
     */
    void decCurly() {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().decCurly();
    }

    /**
     * getTypeCount count
     *
     * Delegate to get the types in current mode.
     *
     * @returns number of types in current mode
     */
    int getTypeCount() const {
        return !st.empty() ? st.back().getTypeCount() : 0;
    }

    /**
     * setTypeCount count
     * @param n the number of types to set to
     *
     * Delegate to set the types in current mode.
     */
    void setTypeCount(int n) {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().setTypeCount(n);
    }

    /**
     * incTypeCount
     *
     * Delegate to increment the number of types.
     */
    void incTypeCount() {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().incTypeCount();
    }

    /**
     * decTypeCount
     *
     * Delegate to decrement the number of types.
     */
    void decTypeCount() {
        if (st.empty())
            throw Segmentation_Fault();

        st.back().decTypeCount();
    }

    /**
     * endAllModes
     *
     * End all modes/states on stack.
     */
    void endAllModes() {

        // end all modes
        while (!st.empty()) {
            endMode(getMode());
        }
    }

    /**
     * popMode
     *
     * Delegate to remove the current mode/state from stack.
     */
    void popMode() {
        if (st.empty())
            throw Segmentation_Fault();

        // close all open elements
        while (!st.empty() && elements.size() > st.back().elementsStart) {
            endElement(elements.back());
        }

        st.pop_back();
        syncTopModes();
    }

    /**
     * endTopMode
     *
     * Delegate to remove/pop the mode on the top of the stack.
     * Equivalent to endMode.
     */
    void endTopMode() {

        endMode();
    }

    /**
     * replaceMode
     * @param oldm modes to clear
     * @param newm modes to set
     *
     * Clear the current modes oldm, and sets then sets the modes new.
     */
    void replaceMode(const srcMLState::MODE_TYPE& oldm, const srcMLState::MODE_TYPE& newm) {

        clearMode(oldm);
        setMode(newm);
    }

    /**
    * endDownToMode
    * @param mode mode to end down to
    *
    * End elements down to but not including the mode mode.
    */
    void endDownToMode(const srcMLState::MODE_TYPE& mode) {

        if (!inTransparentMode(mode))
            return;

        while (size() > 1 && !inMode(mode))
            endMode();
    }

    /**
     * endDownToModeSet
     * @param mode mode to end down to
     *
     * End elements down to but not including the mode mode.
     */
     void endDownToModeSet(const srcMLState::MODE_TYPE& mode) {

        while (size() > 1 && (getMode() & mode) == 0)
            endMode();
    }

    /**
     * endDownOverMode
     * @param mode mode to end down to
     *
     * End elements down to and including the mode mode.
     */
     void endDownOverMode(const srcMLState::MODE_TYPE& mode) {

        if (!inTransparentMode(mode))
            return;

        while (size() > 1 && !inMode(mode))
            endMode();
        endMode();
    }

    /**
     * endWhileMode
     * @param mode mode to down over
     *
     * End elements while in mode mode.
     */
     void endWhileMode(const srcMLState::MODE_TYPE& mode) {

        while (size() > 1 && inMode(mode))
            endMode();
    }

};

#endif
