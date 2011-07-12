/*
  Language.cpp

  Copyright (C) 2008-2011  SDML (www.sdml.info)

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

#include "Language.hpp"
#include <regex.h>
#include <algorithm>

pair Language::lang2int[] = {
    { LanguageName::LANGUAGE_C, LANGUAGE_C },
    { LanguageName::LANGUAGE_CXX, LANGUAGE_CXX },
    { LanguageName::LANGUAGE_JAVA, LANGUAGE_JAVA },
    { LanguageName::LANGUAGE_CXX_0X, LANGUAGE_CXX_0X },
    { LanguageName::LANGUAGE_ASPECTJ, LANGUAGE_ASPECTJ },
    { LanguageName::LANGUAGE_NONE, LANGUAGE_NONE },
    { 0, 0 }
};

static int usercount = 0;

pair Language::userext2int[47] = { { 0, 0 } };

const char * const regex = "(zg\\.|2zb\\.)*([^\\.]*)";

bool Language::registerUserExt(const char* ext, int language) {

  userext2int[usercount].s = ext;
  userext2int[usercount].n = language;
  ++usercount;

  return true;
}

bool Language::registerUserExt(const char* ext, const char* language) {

  int nlanguage = Language::getLanguage(language);
  if (!nlanguage)
    return false;

  return registerUserExt(ext, nlanguage);
}

const char* getLanguageExtension(const char * const inpath)
{
  // internal string for returning constant
  static std::string extension;

  // reversed copy of the path
  std::string path(inpath);
  std::reverse(path.begin(), path.end());

  // setup the regular expression
  static regex_t preg = { 0 };
  static int errorcode = regcomp(&preg, regex, REG_EXTENDED);

  // evalue the regex
  regmatch_t pmatch[3];
  errorcode = errorcode || regexec(&preg, path.c_str(), 3, pmatch, 0);

  // extract the extension from the path, reversing as we go
  extension.assign(&path[pmatch[2].rm_so], &path[pmatch[2].rm_eo]);
  std::reverse(extension.begin(), extension.end());

  // if we have a non-blank extension, return that
  return extension.empty() ? 0 : extension.c_str();
}

// gets the current language based on the extenstion
int Language::getLanguageFromFilename(const char* const path) {

  // extract the (pure) extension
  const char* extension = getLanguageExtension(path);

  if (!extension)
    return 0;

  // custom extensions
  for (int i = usercount - 1; i >= 0; --i) {
    if (strcmp(userext2int[i].s, extension) == 0)
      return userext2int[i].n == LANGUAGE_NONE ? 0 : userext2int[i].n;
  }

  return 0;
}

void Language::register_standard_file_extensions()
{
  Language::registerUserExt("c",    LANGUAGE_C );
  Language::registerUserExt("h",    LANGUAGE_C );

  Language::registerUserExt("cpp",  LANGUAGE_CXX );
  Language::registerUserExt("hpp",  LANGUAGE_CXX );
  Language::registerUserExt("cxx",  LANGUAGE_CXX );
  Language::registerUserExt("hxx",  LANGUAGE_CXX );
  Language::registerUserExt("cc",   LANGUAGE_CXX );
  Language::registerUserExt("hh",   LANGUAGE_CXX );
  Language::registerUserExt("c++",  LANGUAGE_CXX );
  Language::registerUserExt("h++",  LANGUAGE_CXX );
  Language::registerUserExt("C",    LANGUAGE_CXX );
  Language::registerUserExt("H",    LANGUAGE_CXX );
  Language::registerUserExt("tcc",  LANGUAGE_CXX );

  Language::registerUserExt("java", LANGUAGE_JAVA );

  Language::registerUserExt("aj",   LANGUAGE_ASPECTJ );

}
