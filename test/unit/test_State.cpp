/**
 * @file test_State.cpp
 * @copyright
 *
 * Copyright (C) 2013  SDML (www.srcML.org)
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

/*
  test_State.cpp

  Unit tests for Options.

*/

#include <State.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>

int main() {

    /*
      pop
    */

    {
        State s(1);
        try{
            s.pop();
            assert(false);
        } catch(...) {}
    }

    /*
      inMode
    */

    {
        State s(1, 2, 4);
        assert(s.inMode(1));
    }

    {
        State s(1, 2, 4);
        assert(!s.inMode(2));
    }

    /*
      inPrevMode
    */

    {
        State s(1, 2, 4);
        assert(s.inPrevMode(4));
    }

    {
        State s(1, 2, 4);
        assert(!s.inPrevMode(1));
    }

    /*
      inTransparentMode
    */

    {
        State s(1, 2, 4);
        assert(s.inTransparentMode(3));
    }

    {
        State s(1, 2, 4);
        assert(!s.inTransparentMode(4));
    }

    /*
      getMode
    */

    {
        State s(1);
        assert(s.getMode() == 1);
    }

    /*
      getTransparentMode
    */

    {
        State s(1, 2);
        assert(s.getTransparentMode() == 3);
    }

    /*
      setMode
    */

    {
        State s(1, 2);
        s.setMode(4);
        assert(s.getMode() == 5);
        assert(s.getTransparentMode() == 7);
    }

    /*
      setMode
    */

    {
        State s(1 | 4, 2 | 8);
        s.clearMode(4);
        s.clearMode(8);
        assert(s.getMode() == 1);
        assert(s.getTransparentMode() == 3);
    }

    return 0;
}
