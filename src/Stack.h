/*
  Stack.h

  Copyright (C) 2004, 2005  SDML (www.sdml.info)

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

  A simple stack, mainly done for performance reasons.
*/

#include "srcMLException.h"

#ifndef STACK_H
#define STACK_H

template <class Type, int N>
class SimpleStack {
 public:
  SimpleStack()
    : topindex(-1)
    {}

  SimpleStack(const SimpleStack& st)
    : topindex(st.topindex) {

    for (int i = 0; i < st.size(); ++i)
      ar[i] = st.ar[i];
  }

  Type& top() {
    if (topindex < 0)
      throw Segmentation_Fault();

    return ar[topindex];
  }

  const Type& top() const {
    if (topindex < 0)
      throw Segmentation_Fault();

    return ar[topindex];
  }

  const Type& prev() const {
    if (topindex < 1)
      throw Segmentation_Fault();

    return ar[topindex - 1];
  }

  void pop() {
    --topindex;
  }

  void push(const Type& d) {
    ar[++topindex] = d;
  }

  int size() const {
    return topindex + 1;
  }

  bool empty() const {
    return size() == 0;
  }

  SimpleStack& operator=(const SimpleStack& st) {

    for (int i = 0; i < st.size(); ++i)
      ar[i] = st.ar[i];

    topindex = st.topindex;

    return *this;
  }

  Type ar[N];
  int topindex;
 private:
};

#endif
