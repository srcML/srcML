/*
  Language.h

  Copyright (C) 2002, 2003, 2004, 2005  SDML (www.sdml.info)

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

#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <map>
#include <string>
#include "srcmlapps.h"

class Language {

 public:

  enum Languages {
    LANGUAGE_C = 1,
    LANGUAGE_CXX = 2,
    LANGUAGE_CXX_0X = 4,
    LANGUAGE_JAVA = 8,
    LANGUAGE_ASPECTJ = 16,
    LANGUAGE_CXX_FAMILY = LANGUAGE_CXX | LANGUAGE_CXX_0X,
    LANGUAGE_C_FAMILY = LANGUAGE_C | LANGUAGE_CXX_FAMILY,
    LANGUAGE_JAVA_FAMILY = LANGUAGE_JAVA | LANGUAGE_ASPECTJ,
    LANGUAGE_OO = LANGUAGE_CXX_FAMILY | LANGUAGE_JAVA_FAMILY,
  };

  Language(int lang)
    : language(lang != LANGUAGE_ASPECTJ ? lang : lang | LANGUAGE_JAVA) {

  }

  static bool filledLang() {
    lang2int.insert(std::make_pair(LanguageName::LANGUAGE_C, LANGUAGE_C));
    lang2int.insert(std::make_pair(LanguageName::LANGUAGE_CXX, LANGUAGE_CXX));
    lang2int.insert(std::make_pair(LanguageName::LANGUAGE_JAVA, LANGUAGE_JAVA));
    lang2int.insert(std::make_pair(LanguageName::LANGUAGE_CXX_0X, LANGUAGE_CXX_0X));
    lang2int.insert(std::make_pair(LanguageName::LANGUAGE_ASPECTJ, LANGUAGE_ASPECTJ));

    return true;
  }

  // gets the current language
  inline bool inLanguage(int l) const {
    return (l & language) > 0;
  }

  // gets the current language
  inline int getLanguage() const {
    return language;
  }

  // gets the current language
  static int getLanguage(const char* const s) {

    std::map<std::string, int>::const_iterator pos = lang2int.find(s);
  
    if (pos != lang2int.end())
      return pos->second;
    else
      return 0;
  }

  // gets the current language
  const char* getLanguageString() const {

    for (std::map<std::string, int>::const_iterator pos = lang2int.begin(); pos != lang2int.end(); ++pos)
      if (pos->second == language)
	return pos->first.c_str();

    return "";
  }

  ~Language() {}

 private:

  const int language;

  static bool filled;
  static std::map<std::string, int> lang2int;
};

#endif
