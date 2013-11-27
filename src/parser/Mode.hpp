/*
  Mode.hpp

  Copyright (C) 2004-2013  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef MODE_HPP
#define MODE_HPP

#include "TokenParser.hpp"
#include "Language.hpp"
#include "srcMLStateStack.hpp"

class Mode : public TokenParser, public Language {

public:

    /* Set of mode flags */

    // any statement (broad definition includes declarations, etc.)
    const static State::MODE_TYPE MODE_STATEMENT;

    // list used for comma and right parentheses
    const static State::MODE_TYPE MODE_LIST;

    // a particular mode is expected in the next start
    const static State::MODE_TYPE MODE_EXPECT;

    // statement may be nested inside of the current
    const static State::MODE_TYPE MODE_DETECT_COLON;

    //
    const static State::MODE_TYPE MODE_TEMPLATE;
    const static State::MODE_TYPE MODE_TEMPLATE_PARAMETER_LIST;

    // an argument to a call
    const static State::MODE_TYPE MODE_ARGUMENT;
    const static State::MODE_TYPE MODE_NAMESPACE;

    // a parameter for a declaration/definition
    const static State::MODE_TYPE MODE_PARAMETER;

    // expressions
    const static State::MODE_TYPE MODE_EXPRESSION;

    // expecting a call (member initialization list)
    const static State::MODE_TYPE MODE_CALL;

    // setup for expecting a condition and detection of the end
    // of a condition at a left parentheses of the correct count
    const static State::MODE_TYPE MODE_CONDITION;

    const static State::MODE_TYPE MODE_TOP;

    // blocks that are not necessarily statements
    const static State::MODE_TYPE MODE_BLOCK;
    const static State::MODE_TYPE MODE_INIT;

    // block tags from being issued.  Should be moved to
    // output handling
    const static State::MODE_TYPE MODE_FUNCTION_TAIL;

    // whether to parse the end of line character
    // used with preprocessor directives
    const static State::MODE_TYPE MODE_PARSE_EOL;

    // local mode only used within a grammar rule
    const static State::MODE_TYPE MODE_LOCAL;

    const static State::MODE_TYPE MODE_VARIABLE_NAME;

    // the if statement includes some special processing
    // including starting a THEN element after the condition
    // and stopping the ending of statements at the IF when
    // an ELSE is matched
    const static State::MODE_TYPE MODE_IF;

    // for special sections inside of mode such as in
    // classes and switch statement blocks
    const static State::MODE_TYPE MODE_TOP_SECTION;

    // for flags
    const static State::MODE_TYPE MODE_FOR_GROUP;          // in a for heading group
    const static State::MODE_TYPE MODE_FOR_INITIALIZATION; // for initialization (in header)
    const static State::MODE_TYPE MODE_FOR_CONDITION;      // for condition (in header)
    const static State::MODE_TYPE MODE_FOR_INCREMENT;      // for increment (in header)

    const static State::MODE_TYPE MODE_PREPROC;
    const static State::MODE_TYPE MODE_NEST;
    const static State::MODE_TYPE MODE_EXPRESSION_BLOCK;
    const static State::MODE_TYPE MODE_INTERNAL_END_PAREN; // remove

    // access regions in classes used for matching of
    const static State::MODE_TYPE MODE_ACCESS_REGION;
    const static State::MODE_TYPE MODE_DO_STATEMENT;
    const static State::MODE_TYPE MODE_IGNORE_TERMINATE;

    const static State::MODE_TYPE MODE_EXTERN;
    const static State::MODE_TYPE MODE_INTERNAL_END_CURLY;

    const static State::MODE_TYPE MODE_CLASS;
    const static State::MODE_TYPE MODE_END_AT_BLOCK;

    const static State::MODE_TYPE MODE_END_ONLY_AT_RPAREN;

    const static State::MODE_TYPE MODE_END_AT_BLOCK_NO_TERMINATE;

    const static State::MODE_TYPE MODE_FUNCTION_NAME;

    const static State::MODE_TYPE MODE_THEN;
    const static State::MODE_TYPE MODE_ELSE;

    const static State::MODE_TYPE MODE_TYPEDEF;

    const static State::MODE_TYPE MODE_DECL;

    const static State::MODE_TYPE MODE_EAT_TYPE;

    const static State::MODE_TYPE MODE_FUNCTION_PARAMETER;

    const static State::MODE_TYPE MODE_INNER_DECL;

    const static State::MODE_TYPE MODE_IN_INIT;

    const static State::MODE_TYPE MODE_TRY;

    const static State::MODE_TYPE MODE_END_LIST_AT_BLOCK;

    const static State::MODE_TYPE MODE_ONLY_END_TERMINATE;

    const static State::MODE_TYPE MODE_ENUM;

    const static State::MODE_TYPE MODE_ANONYMOUS;

    const static State::MODE_TYPE MODE_END_AT_COMMA;

    const static State::MODE_TYPE MODE_TRAILING_RETURN;

public:

    Mode(TokenParser* ptp, int lang)
        : Language(lang), statev(ptp)
        {}

    ~Mode() {}

    srcMLStateStack statev;

protected:

    // flush any skipped tokens to the output token stream
    // overridden in StreamParser
    //  void flushSkip() {}

    int size() const {
        return statev.size();
    }

    srcMLState& currentState() {

        return statev.currentState();
    }

    int getParen() const {

        return statev.getParen();
    }

    void incParen() {

        statev.incParen();
    }

    void decParen() {

        statev.decParen();
    }

    int getCurly() const {

        return statev.getCurly();
    }

    void incCurly() {

        statev.incCurly();
    }

    void decCurly() {

        statev.decCurly();
    }

    int getTypeCount() const {

        return statev.getTypeCount();
    }

    void setTypeCount(int n) {

        statev.setTypeCount(n);
    }

    void incTypeCount() {

        statev.incTypeCount();
    }

    void decTypeCount() {

        statev.decTypeCount();
    }

    void startNewMode(const State::MODE_TYPE& m) {

        statev.startNewMode(m);
    }

    void endMode() {

        statev.endCurrentMode();
    }

    void endMode(const State::MODE_TYPE& m) {

        statev.endCurrentMode(m);
    }

    void endLastMode() {

        statev.endLastMode();
    }

    void endTopMode() {

        statev.endCurrentMode();
    }

    void setMode(const State::MODE_TYPE& m) {

        statev.setMode(m);
    }

    void clearMode(const State::MODE_TYPE& m) {

        statev.clearMode(m);
    }

    void replaceMode(const State::MODE_TYPE& oldm, const State::MODE_TYPE& newm) {

        statev.clearMode(oldm);
        statev.setMode(newm);
    }

    bool inPrevMode(const State::MODE_TYPE& m) const {

        return statev.inPrevMode(m);
    }

    bool inMode(const State::MODE_TYPE& m) const {

        return statev.inMode(m);
    }

    bool inTransparentMode(const State::MODE_TYPE& m) const {

        return statev.inTransparentMode(m);
    }

    // End elements down to a mode
    void endDownToMode(const State::MODE_TYPE& ele);

    // End elements down to one of a set of modes
    void endDownToModeSet(const State::MODE_TYPE& ele);

    // End elements down to a mode, then consume it
    void endDownOverMode(const State::MODE_TYPE& ele);
};

#endif
