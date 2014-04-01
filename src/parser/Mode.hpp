/**
 * @file Mode.hpp
 *
 * @copyright Copyright (C) 2004-2014 SDML (www.srcML.org)
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

#ifndef MODE_HPP
#define MODE_HPP

#include "TokenParser.hpp"
#include "Language.hpp"
#include "srcMLStateStack.hpp"

/**
 * Mode
 *
 * Class representing a stack of modes that direct parsing.
 * Modes also keep srcMLstate such as open parethesis/curly braces/tags
 * in the underlying structures in which it delegates.
 */
class Mode : public TokenParser, public Language {

public:

    /* Set of mode flags */

    /** any srcMLstatement (broad definition includes declarations, etc.) */
    const static srcMLState::MODE_TYPE MODE_STATEMENT;

    /** list used for comma and right parentheses */
    const static srcMLState::MODE_TYPE MODE_LIST;

    /** a particular mode is expected in the next start */
    const static srcMLState::MODE_TYPE MODE_EXPECT;

    /** srcMLstatement may be nested inside of the current */
    const static srcMLState::MODE_TYPE MODE_DETECT_COLON;

    /** mode for within a templare */
    const static srcMLState::MODE_TYPE MODE_TEMPLATE;

    /** mode for within a template parameter list */
    const static srcMLState::MODE_TYPE MODE_TEMPLATE_PARAMETER_LIST;

    /** an argument to a call */
    const static srcMLState::MODE_TYPE MODE_ARGUMENT;

    /** mode for a namespace */
    const static srcMLState::MODE_TYPE MODE_NAMESPACE;

    /** a parameter for a declaration/definition */
    const static srcMLState::MODE_TYPE MODE_PARAMETER;

    /** expressions */
    const static srcMLState::MODE_TYPE MODE_EXPRESSION;

    /** expecting a call (member initialization list) */
    const static srcMLState::MODE_TYPE MODE_CALL;

    /** 
     * setup for expecting a condition and detection of the end
     * of a condition at a left parentheses of the correct count 
     */
    const static srcMLState::MODE_TYPE MODE_CONDITION;

    /** marks top of some sequence of operations mostly to stop ending modes */
    const static srcMLState::MODE_TYPE MODE_TOP;

    /** blocks that are not necessarily srcMLstatements */
    const static srcMLState::MODE_TYPE MODE_BLOCK;

    /** mode for inititialization typically @code=<init>...</init>@endcode */
    const static srcMLState::MODE_TYPE MODE_INIT;

    /** 
     * block tags from being issued.  Should be moved to
     * output handling 
     */
    const static srcMLState::MODE_TYPE MODE_FUNCTION_TAIL;

    /** 
     * whether to parse the end of line character
     * used with preprocessor directives 
     */
    const static srcMLState::MODE_TYPE MODE_PARSE_EOL;

    /** local mode only used within a grammar rule */
    const static srcMLState::MODE_TYPE MODE_LOCAL;

    /** Mode for a variable name */
    const static srcMLState::MODE_TYPE MODE_VARIABLE_NAME;

    /** 
     * the if srcMLstatement includes some special processing
     * including starting a THEN element after the condition
     * and stopping the ending of srcMLstatements at the IF when
     * an ELSE is matched 
     */
    const static srcMLState::MODE_TYPE MODE_IF;

    /**
     * for special sections inside of mode such as in
     *  classes and switch srcMLstatement blocks
     */
    const static srcMLState::MODE_TYPE MODE_TOP_SECTION;

    /** in a for heading group i.e. for init/condition/increment */
    const static srcMLState::MODE_TYPE MODE_FOR_GROUP;

    /** for initialization (in header) */
    const static srcMLState::MODE_TYPE MODE_FOR_INITIALIZATION;

    /** for condition (in header) */
    const static srcMLState::MODE_TYPE MODE_FOR_CONDITION;

    /** for increment (in header) */
    const static srcMLState::MODE_TYPE MODE_FOR_INCREMENT;

    /** preprocessor mode */
    const static srcMLState::MODE_TYPE MODE_PREPROC;

    /** mode for nesting srcMLstatements */
    const static srcMLState::MODE_TYPE MODE_NEST;

    /** mode fore expression block */
    const static srcMLState::MODE_TYPE MODE_EXPRESSION_BLOCK;

    /** mode marking to end at right parenthesis @todo remove */
    const static srcMLState::MODE_TYPE MODE_INTERNAL_END_PAREN;

    /** access regions in classes used for matching of */
    const static srcMLState::MODE_TYPE MODE_ACCESS_REGION;

    /** mode for a do while srcMLstatement */
    const static srcMLState::MODE_TYPE MODE_DO_STATEMENT;
    
    /** mode to ignore ; */
    const static srcMLState::MODE_TYPE MODE_IGNORE_TERMINATE;

    /** mode for extern */
    const static srcMLState::MODE_TYPE MODE_EXTERN;
    
    /** mode to end at right curly */
    const static srcMLState::MODE_TYPE MODE_INTERNAL_END_CURLY;

    /** mode for a class */
    const static srcMLState::MODE_TYPE MODE_CLASS;
    
    /** mode to end at block */
    const static srcMLState::MODE_TYPE MODE_END_AT_BLOCK;

    /** mode to only end at right parentesis */
    const static srcMLState::MODE_TYPE MODE_END_ONLY_AT_RPAREN;

    /** mode to end at a block and not expect ; after */
    const static srcMLState::MODE_TYPE MODE_END_AT_BLOCK_NO_TERMINATE;

    /** mode for a function name */
    const static srcMLState::MODE_TYPE MODE_FUNCTION_NAME;

    /** mode for a if then */
    const static srcMLState::MODE_TYPE MODE_THEN;

    /** mode for an else */    
    const static srcMLState::MODE_TYPE MODE_ELSE;

    /** mode for a typdef */
    const static srcMLState::MODE_TYPE MODE_TYPEDEF;

    /** mode for a declaration of some type */
    const static srcMLState::MODE_TYPE MODE_DECL;

    /** mode to consume the type names */
    const static srcMLState::MODE_TYPE MODE_EAT_TYPE;

    /** mode for funciton parameter */
    const static srcMLState::MODE_TYPE MODE_FUNCTION_PARAMETER;

    /** mode for an internal decl */
    const static srcMLState::MODE_TYPE MODE_INNER_DECL;

    /** mode to mark in an init */
    const static srcMLState::MODE_TYPE MODE_IN_INIT;

    /** mode for a try */
    const static srcMLState::MODE_TYPE MODE_TRY;

    /** mode to end a list at a block */
    const static srcMLState::MODE_TYPE MODE_END_LIST_AT_BLOCK;

    /** mode to end at ; */
    const static srcMLState::MODE_TYPE MODE_ONLY_END_TERMINATE;

    /** mode for enum */
    const static srcMLState::MODE_TYPE MODE_ENUM;

    /** mode for anonymous item e.g. anonymous class */
    const static srcMLState::MODE_TYPE MODE_ANONYMOUS;

    /** mode to end at a comma */
    const static srcMLState::MODE_TYPE MODE_END_AT_COMMA;

    /** mode for in a using */
    const static srcMLState::MODE_TYPE MODE_USING;

    /** mode for function trailing return */
    const static srcMLState::MODE_TYPE MODE_TRAILING_RETURN;

    /** mode to issue an empty element at pop */
    const static srcMLState::MODE_TYPE MODE_ISSUE_EMPTY_AT_POP;

    /** mode to end at preprocessor endif */
    const static srcMLState::MODE_TYPE MODE_END_AT_ENDIF;

    /** mode for an argument list */
    const static srcMLState::MODE_TYPE MODE_ARGUMENT_LIST;

public:

    /**
     * Mode
     * @param ptp the token parser
     * @param lang the current language
     *
     * Constructor.  Create mode stack from TokenParser and current language.
     */
    Mode(TokenParser* ptp, int lang)
        : Language(lang), statev(ptp)
    {}

    /**
     * ~Mode
     * 
     * Destructor.
     */
    ~Mode() {}

    /** internal storage of srcML srcMLstates (modes) and other information */
    srcMLStateStack statev;

protected:

    /**
     * size
     *
     * Delegate to return the size of srcMLstate stack.
     *
     * @returns the size of the srcMLstate stack.
     */
    int size() const {
        return (int)statev.size();
    }

     /**
     * currentState
     *
     * Delegate to return the current srcMLstate.
     *
     * @returns the curent srcMLstate.
     */
    srcMLState& currentState() {

        return statev.currentState();
    }

     /**
     * getParen
     *
     * Delegate to get the number of open parenthesis in current srcMLstate.
     *
     * @returns the number of open parthesis.
     */
    int getParen() const {

        return statev.getParen();
    }

    /**
     * incParen
     *
     * Delegate to increment the number of open parenthesis in currrent srcMLstate.
     */
    void incParen() {

        statev.incParen();
    }

    /**
     * decParen
     *
     * Delegate to decrement the number of open parenthesis in currrent srcMLstate.
     */
    void decParen() {

        statev.decParen();
    }

    /**
     * getCurly
     *
     * Delegate to get the number of open curly braces in current srcMLstate.
     *
     * @returns the number of open curly braces.
     */
    int getCurly() const {

        return statev.getCurly();
    }

    /**
     * incCurly
     *
     * Delegate to increment the number of open curly braces in currrent srcMLstate.
     */
    void incCurly() {

        statev.incCurly();
    }

    /**
     * decCurly
     *
     * Delegate to decrement the number of open curly braces in currrent srcMLstate.
     */
    void decCurly() {

        statev.decCurly();
    }

    /**
     * getTypecount
     *
     * Delegate to get the number of types.
     *
     * @returns the number of types.
     */
    int getTypeCount() const {

        return statev.getTypeCount();
    }

    /**
     * setTypecount
     *
     * Delegate to set the number of types.
     */
    void setTypeCount(int n) {

        statev.setTypeCount(n);
    }

    /**
     * incTypecount
     *
     * Delegate to increment the number of types.
     */
    void incTypeCount() {

        statev.incTypeCount();
    }

    /**
     * decTypecount
     *
     * Delegate to decrement the number of types.
     */
    void decTypeCount() {

        statev.decTypeCount();
    }

    /**
     * startNewMode
     * @param m a new mode
     *
     * Delegate to create/push a new mode m onto the stack.
     */
    void startNewMode(const srcMLState::MODE_TYPE& m) {

        statev.startNewMode(m);
    }

    /**
     * endMode
     *
     * Delegate to remove/pop the mode on the top of the stack.
     */
    void endMode() {

        statev.endCurrentMode();
    }

    /**
     * endMode
     * @param m mode to take off
     *
     * Delegate to remove/pop the mode m off the top of stack.
     * No checking is done to see if m is on the top of the stack.
     * Same as endMode().
     */
    void endMode(const srcMLState::MODE_TYPE& m) {

        statev.endCurrentMode(m);
    }

    /**
     * endLastMode
     *
     * Delegate to remove/pop the last mode on the stack.
     * No actual checking is done to see if last mode.
     * Actually, less strict version of endMode.
     */
    void endLastMode() {

        statev.endLastMode();
    }

    /**
     * endTopMode
     *
     * Delegate to remove/pop the mode on the top of the stack.
     * Equivalent to endMode.
     */
    void endTopMode() {

        statev.endCurrentMode();
    }

    /**
     * setMode
     * @param m modes to set
     *
     * Delegate to add the modes m to the top of the stack.
     * Does not overide modes but adds them to current mode.
     */
    void setMode(const srcMLState::MODE_TYPE& m) {

        statev.setMode(m);
    }
    /**
     * getMode
     *
     * Delegate to get the current mode on top of the stack.
     */
    srcMLState::MODE_TYPE getMode() {

        return statev.getMode();
    }

    /**
     * clearMode
     * @param m modes to clear
     *
     * Delegate to clear/unset the modes m in the current mode.
     */
    void clearMode(const srcMLState::MODE_TYPE& m) {

        statev.clearMode(m);
    }

    /**
     * replaceMode
     * @param oldm modes to clear
     * @param newm modes to set
     *
     * Clear the current modes oldm, and sets then sets the modes new.
     */
    void replaceMode(const srcMLState::MODE_TYPE& oldm, const srcMLState::MODE_TYPE& newm) {

        statev.clearMode(oldm);
        statev.setMode(newm);
    }

    /**
     * inPrevMode
     * @param m modes to check if in
     *
     * Delegate to predicate to check if the previous mode before current has m (not exact mode, but at least m).
     *
     * @returns if in the previous mode m.
     */
    bool inPrevMode(const srcMLState::MODE_TYPE& m) const {

        return statev.inPrevMode(m);
    }

    /**
     * inMode
     * @param m modes to check if in
     *
     * Delegate to predicate to check if in the current mode m (not exact mode, but at least m).
     *
     * @returns if in the previous mode m.
     */
    bool inMode(const srcMLState::MODE_TYPE& m) const {

        return statev.inMode(m);
    }

    /**
     * inTransparentMode
     * @param m modes to check if in
     *
     * Delegate to predicate to check if any mode on entire stack has m (not extact mode, but at least m).
     *
     * @returns if in the previous mode m.
     */
    bool inTransparentMode(const srcMLState::MODE_TYPE& m) const {

        return statev.inTransparentMode(m);
    }

    /**
     * dupDownOverMode
     * @param m mode to stop on
     *
     * Duplicate modes on stack down to and including m.
     */
    void dupDownOverMode(const srcMLState::MODE_TYPE& m) {

        std::list<srcMLState> alist;
        while(!(statev.st.top().getMode() & m)) {

            alist.push_front(statev.st.top());
            statev.st.pop();

        }

        alist.push_front(statev.st.top());
        statev.st.pop();


        alist.front().setMode(MODE_TOP | MODE_END_AT_ENDIF);
        for(std::list<srcMLState>::iterator i = alist.begin(); i != alist.end(); ++i) {
            i->setMode(MODE_END_AT_ENDIF);
            statev.st.push(*i);
        }

        alist.front().openelements = std::stack<int>();
        for(std::list<srcMLState>::iterator i = alist.begin(); i != alist.end(); ++i) {
            i->setMode(MODE_ISSUE_EMPTY_AT_POP);
            statev.st.push(*i);

        }


    }

    // End elements down to a mode
    void endDownToMode(const srcMLState::MODE_TYPE& ele);

    // End elements down to one of a set of modes
    void endDownToModeSet(const srcMLState::MODE_TYPE& ele);

    // End elements down to a mode, then consume it
    void endDownOverMode(const srcMLState::MODE_TYPE& ele);
};

#endif
