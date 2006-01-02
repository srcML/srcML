/*
  State.h

  Copyright (C) 2002, 2003, 2004, 2005  SDML (www.sdml.info)

  This file is part of the srcML translator.

  The srcML translator is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML translator is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML translator; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef STATE_H
#define STATE_H

#include <list>
#include "Stack.h"

#include "srcMLException.h"

class State {
 public:

#ifdef __GNUC__
  typedef unsigned long long MODE_TYPE;
#else
  typedef unsigned __int64 MODE_TYPE;
#endif

  State(const MODE_TYPE& mode = 0, const MODE_TYPE& transmode = 0)
    : flags(mode), flags_prev(transmode | mode)
    {}

  // callstack methods

  void push(int id) {
    callstack.push(id);
  }

  void pop() {
    if (callstack.empty())
      throw Segmentation_Fault();

    callstack.pop();
  }

  // mode methods

  bool inMode(const MODE_TYPE& m) const {

    return (flags & m) == m;
  }

  bool inTransparentMode(const MODE_TYPE& m) const {

    return (flags_prev & m) == m;
  }

  const MODE_TYPE& getMode() const {
    return flags;
  }

  const MODE_TYPE& getTransparentMode() const {
    return flags_prev;
  }

  void setMode(const MODE_TYPE& m1) {

    flags |= m1;
    flags_prev |= m1;
  }

  void clearMode(const MODE_TYPE& m) {
  
    flags &= ~m;
    flags_prev &= ~m;
  }

  ~State() {
  }

 public:
  MODE_TYPE flags;
  MODE_TYPE flags_prev;

  SimpleStack<int, 100> callstack;
};

#endif
