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
    : language(lang != LANGUAGE_ASPECTJ ? lang : lang | LANGUAGE_JAVA)
    {}

  // gets the current language
  inline bool inLanguage(int l) const {
    return (l & language) > 0;
  }

  // gets the current language
  inline int getLanguage() const {
    return language;
  }

  // gets the current language
  const char* getLanguageString() const {
    char const* lstring = "";

    switch (language) {
    case LANGUAGE_C:
      lstring = "C";
      break;

    case LANGUAGE_CXX:
      lstring = "C++";
      break;

    case LANGUAGE_JAVA:
      lstring = "Java";
      break;

    case LANGUAGE_CXX_0X:
      lstring = "C++0x";
      break;

    case LANGUAGE_ASPECTJ | LANGUAGE_JAVA:
      lstring = "AspectJ";
      break;
    };

    return lstring;
  }

  ~Language() {}

 private:

  const int language;
};

#endif
