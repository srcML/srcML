/*
  Language.cpp

  Copyright (C) 2008  SDML (www.sdml.info)

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

#include "Language.h"

pair Language::lang2int[] = {
    { LanguageName::LANGUAGE_C, LANGUAGE_C },
    { LanguageName::LANGUAGE_CXX, LANGUAGE_CXX },
    { LanguageName::LANGUAGE_JAVA, LANGUAGE_JAVA },
    { LanguageName::LANGUAGE_CXX_0X, LANGUAGE_CXX_0X },
    { LanguageName::LANGUAGE_ASPECTJ, LANGUAGE_ASPECTJ },
    { 0, 0 }
};

pair Language::ext2int[] = {
    { "*.c", LANGUAGE_C },
    { "*.h", LANGUAGE_C },

    { "*.C", LANGUAGE_CXX },
    { "*.cpp", LANGUAGE_CXX },
    { "*.cc", LANGUAGE_CXX },
    { "*.cxx", LANGUAGE_CXX },
    { "*.c++", LANGUAGE_CXX },
    { "*.H", LANGUAGE_CXX },
    { "*.hpp", LANGUAGE_CXX },
    { "*.hh", LANGUAGE_CXX },
    { "*.hxx", LANGUAGE_CXX },
    { "*.h++", LANGUAGE_CXX },

    { "*.java", LANGUAGE_JAVA },

    { "*.aj", LANGUAGE_ASPECTJ },
    /*
    { LanguageName::LANGUAGE_CXX_0X, LANGUAGE_CXX_0X },
    */
    { 0, 0 }
};

static int usercount = 0;

pair Language::userext2int[32] = { { 0, 0 } };

bool Language::registerUserExt(const char* ext, const char* language) {

  int nlanguage = Language::getLanguage(language);
  if (!nlanguage)
    return false;

  userext2int[usercount].s = ext;
  userext2int[usercount].n = nlanguage;
  ++usercount;

  return true;
}

