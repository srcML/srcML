/**
 * @file test_Language.cpp
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
  test_Language.cpp

  Unit tests for Language.

*/

#include <Language.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <iostream>

int main() {

    /*
      inLanguage()
    */
    std::cout << "Test 1" << '\n';
    {
        Language l(Language::LANGUAGE_CXX);
        assert(!l.inLanguage(Language::LANGUAGE_C));
    }

    std::cout << "Test 2" << '\n';
    {
        Language l(Language::LANGUAGE_CXX);
        assert(l.inLanguage(Language::LANGUAGE_CXX));
    }


    /*
      getLanguage()
    */
    std::cout << "Test 3" << '\n';
    {
        Language l(Language::LANGUAGE_CXX);
        assert(l.getLanguage() == Language::LANGUAGE_CXX);
    }

    /*
      getLanguage(const char *)
    */
    std::cout << "Test 4" << '\n';
    {
        assert(Language::getLanguage("C++") == Language::LANGUAGE_CXX);
    }

    /*
      getLanguageString()
    */
    std::cout << "Test 5" << '\n';
    {
        Language l(Language::LANGUAGE_CXX);
        assert(Language::getLanguage("C++") == Language::LANGUAGE_CXX);
    }

    return 0;
}
