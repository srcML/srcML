/*
  Stack.hpp

  Copyright (C) 2004-2010  SDML (www.sdml.info)

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

#include "srcMLException.hpp"

#ifndef STACK_HPP
#define STACK_HPP

#include <deque>

template <class Type>
class SimpleStack {
public:
    SimpleStack() {}

    SimpleStack(const SimpleStack& st)
        : data(st.data) {}

    Type& top() {
        return data.back();
    }

    const Type& top() const {
        return data.back();
    }

    const Type& prev() const {
        typename std::deque<Type>::const_reverse_iterator it = data.rbegin();
        ++it;
          
        return *it;
    }

    void pop() {
        data.pop_back();
    }

    void push(const Type& d) {
        data.push_back(d);
    }

    int size() const {
        return data.size();
    }

    bool empty() const {
        return data.empty();
    }

    SimpleStack& operator=(const SimpleStack& st) {

        data = st.data;

        return *this;
    }
private:
    std::deque<Type> data;
};

#endif
