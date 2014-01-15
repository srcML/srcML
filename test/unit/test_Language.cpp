/**
 * @file test_Language.cpp
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
  test_Language.cpp

  Unit tests for Language.

 */

#include <Language.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>

int main() {

  /*
    inLanguage()
   */ 

  {
    Language l(Language::LANGUAGE_CXX);
    assert(!l.inLanguage(Language::LANGUAGE_C));
  }

  {
    Language l(Language::LANGUAGE_CXX);
    assert(l.inLanguage(Language::LANGUAGE_CXX));
  }

  /*
    getLanguage()
   */ 

  {
    Language l(Language::LANGUAGE_CXX);
    assert(l.getLanguage() == Language::LANGUAGE_CXX);
  }

  /*
    getLanguage(const char *)
   */ 
  {
    assert(Language::getLanguage("C++") == Language::LANGUAGE_CXX);
  }

  /*
    getLanguageString()
   */ 
  {
    Language l(Language::LANGUAGE_CXX);
    assert(Language::getLanguage("C++") == Language::LANGUAGE_CXX);
  }

  /*
    registerUserExt()
   */ 

  {
    std::vector<pair> reg_ext;
    assert(Language::registerUserExt("cpp", "C++", reg_ext));
    assert(reg_ext.size() == 1);
    assert(reg_ext.at(0).n == Language::LANGUAGE_CXX);
    assert(reg_ext.at(0).s == "cpp");
  }

  {
    std::vector<pair> reg_ext;
    assert(!Language::registerUserExt("cpp", "Foo", reg_ext));
    assert(reg_ext.size() == 0);
  }

  /*
    getLanguageFromFilename
   */

  {
    std::vector<pair> reg_ext;
    Language::registerUserExt("cpp", "C++", reg_ext);
    assert(Language::getLanguageFromFilename("a.cpp", reg_ext) == Language::LANGUAGE_CXX);
  }

  {
    std::vector<pair> reg_ext;
    Language::registerUserExt("cpp", "C++", reg_ext);
    assert(Language::getLanguageFromFilename("a.cpp.gz", reg_ext) == Language::LANGUAGE_CXX);
  }

  {
    std::vector<pair> reg_ext;
    Language::registerUserExt("cpp", "C++", reg_ext);
    assert(Language::getLanguageFromFilename("a.cpp.bz2", reg_ext) == Language::LANGUAGE_CXX);
  }

  {
    std::vector<pair> reg_ext;
    Language::registerUserExt("cpp", "C++", reg_ext);
    assert(Language::getLanguageFromFilename("a.cpp.xz", reg_ext) == Language::LANGUAGE_CXX);
  }

  {
    std::vector<pair> reg_ext;
    Language::registerUserExt("cpp", "C++", reg_ext);
    assert(Language::getLanguageFromFilename("a.foo", reg_ext) == 0);
  }

  /*
    register_standard_file_extensions()
   */ 

  {
    std::vector<pair> reg_ext;
    Language::register_standard_file_extensions(reg_ext);
    assert(reg_ext.size() != 0);
  }

  return 0;
}
