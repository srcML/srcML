/**
 * @file test_srcMLStateStack.cpp
 *
 * @copyright Copyright (C) 2013-2014 SDML (www.srcML.org)
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
  test_Mode.cpp

  Unit tests for Mode.

*/

#include <srcMLStateStack.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <TokenParser.hpp>

class TP : public TokenParser {

public:
    TP() {}

    virtual void startElement(int) {}
    virtual void startNoSkipElement(int) {}
    virtual void endElement(int) {}
    virtual void emptyElement(int) {}
    virtual void flushSkip() {}
    virtual int SkipBufferSize() { return 0; }
    virtual antlr::RefToken* CurrentToken() { return 0; }
    virtual void addElement(int) { }
    virtual ~TP() {}
};

int main(/*int argc, char * argv[]*/) {

    /*
      size

      anything above zero causes exception on Destructor.
    */

    {
        TP tp;
        srcMLStateStack s(&tp);
        assert(s.size() == 0);
    }

    return 0;
}
