/*
  test_Language.cpp

  Unit tests for Language.

 */

#include <Language.hpp>
#include <stdio.h>
#include <string.h>
#include <cassert>

int main(int argc, char * argv[]) {

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
    assert(reg_ext.at(0).n == Language::LANGUAGE_CXX);
    assert(reg_ext.at(0).s == "cpp");
  }

  return 0;
}
