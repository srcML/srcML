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
    LANGUAGE_JAVA = 4,
    LANGUAGE_C_FAMILY = LANGUAGE_C | LANGUAGE_CXX,
    LANGUAGE_OO = LANGUAGE_CXX | LANGUAGE_JAVA
  };

  Language(int lang)
    : language(lang)
    {}

  // gets the current language
  virtual bool inLanguage(int l) const {
    return (l & language) > 0;
  }

  // gets the current language
  virtual int getLanguage() const {
    return language;
  }

  // gets the current language
  virtual const char* getLanguageString() const {
    char* lstring = "";

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
    };

    return lstring;
  }

  virtual ~Language() {}

 private:

  int language;
};

#endif
