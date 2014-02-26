/**
 * @file test_srcMLState.cpp
 *
 * @copyright Copyright (C) 2013-2014  SDML (www.srcML.org)
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

int main() {

    /*
      getParen
    */

    {
        srcMLState s;
        assert(s.getParen() == 0);
    }

    /*
      incParen
    */

    {
        srcMLState s;
        s.incParen();
        assert(s.getParen() == 1);
    }

    /*
      decParen
    */

    {
        srcMLState s;
        s.incParen();
        s.incParen();
        s.decParen();
        assert(s.getParen() == 1);
    }

    /*
      getTypeCount
    */

    {
        srcMLState s;
        assert(s.getTypeCount() == 0);
    }

    /*
      incTypeCount
    */

    {
        srcMLState s;
        s.incTypeCount();
        assert(s.getTypeCount() == 1);
    }

    /*
      decTypeCount
    */

    {
        srcMLState s;
        s.incTypeCount();
        s.incTypeCount();
        s.decTypeCount();
        assert(s.getTypeCount() == 1);
    }


    /*
      setTypeCount
    */

    {
        srcMLState s;
        s.setTypeCount(4);
        assert(s.getTypeCount() == 4);
    }

    return 0;
}
