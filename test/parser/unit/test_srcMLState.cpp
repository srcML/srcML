/**
 * @file test_srcMLState.cpp
 *
 * @copyright Copyright (C) 2013-2014 srcML, LLC. (www.srcML.org)
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
  test_srcMLState.cpp

  Unit tests for srcMLState.

*/

#include <srcMLState.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <dassert.hpp>

int main() {


    /*
      pop
    */

    {
        srcMLState s(1);
        try{
            s.pop();
            assert(false);
        } catch(...) {}
    }

    /*
      inMode
    */

    {
        srcMLState s(1, 2, 4);
        assert(s.inMode(1));
    }

    {
        srcMLState s(1, 2, 4);
        assert(!s.inMode(2));
    }

    /*
      inPrevMode
    */

    {
        srcMLState s(1, 2, 4);
        assert(s.inPrevMode(4));
    }

    {
        srcMLState s(1, 2, 4);
        assert(!s.inPrevMode(1));
    }

    /*
      inTransparentMode
    */

    {
        srcMLState s(1, 2, 4);
        assert(s.inTransparentMode(3));
    }

    {
        srcMLState s(1, 2, 4);
        assert(!s.inTransparentMode(4));
    }

    /*
      getMode
    */

    {
        srcMLState s(1);
        dassert(s.getMode(), 1);
    }

    /*
      getTransparentMode
    */

    {
        srcMLState s(1, 2);
        dassert(s.getTransparentMode(), 3);
    }

    /*
      setMode
    */

    {
        srcMLState s(1, 2);
        s.setMode(4);
        dassert(s.getMode(), 5);
        dassert(s.getTransparentMode(), 7);
    }

    /*
      setMode
    */

    {
        srcMLState s(1 | 4, 2 | 8);
        s.clearMode(4);
        s.clearMode(8);
        dassert(s.getMode(), 1);
        dassert(s.getTransparentMode(), 3);
    }

    /*
      getParen
    */

    {
        srcMLState s;
        dassert(s.getParen(), 0);
    }

    /*
      incParen
    */

    {
        srcMLState s;
        s.incParen();
        dassert(s.getParen(), 1);
    }

    /*
      decParen
    */

    {
        srcMLState s;
        s.incParen();
        s.incParen();
        s.decParen();
        dassert(s.getParen(), 1);
    }

    /*
      getTypeCount
    */

    {
        srcMLState s;
        dassert(s.getTypeCount(), 0);
    }

    /*
      incTypeCount
    */

    {
        srcMLState s;
        s.incTypeCount();
        dassert(s.getTypeCount(), 1);
    }

    /*
      decTypeCount
    */

    {
        srcMLState s;
        s.incTypeCount();
        s.incTypeCount();
        s.decTypeCount();
        dassert(s.getTypeCount(), 1);
    }


    /*
      setTypeCount
    */

    {
        srcMLState s;
        s.setTypeCount(4);
        dassert(s.getTypeCount(), 4);
    }

    return 0;
}
