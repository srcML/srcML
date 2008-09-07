/*
  srcmlapps.cpp

  Copyright (C) 2004, 2005  SDML (www.sdml.info)

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

  Common utilities for srcML applications.
*/

#include <cstring>
#include <clocale>

#ifdef __GNUC__
#include <langinfo.h>
#endif

// current environment encoding
const char* getencoding() {

#ifdef __GNUC__
  setlocale(LC_ALL, "");

  return nl_langinfo(CODESET);
#else
  return "ISO-8859-1";
#endif
}

// compare flags to arguments
bool compare_flags(const char* arg, const char* flag , const char* short_flag, int& pos) {

  // single characters not allowed
  if (strlen(arg) <= 1)
    return false;

  // in middle of short character matching
  if (pos > 0 && arg[1] != '-' && arg[1 + pos] == short_flag[1]) {
    ++pos;
    return true;
  }

  if (// direct match to flag
      strcmp(arg, flag) == 0 ||

      // direct match to short flag
      strcmp(arg, short_flag) == 0 ||

      // match to flag but with immediate value
      (strncmp(arg, flag, strlen(flag)) == 0 && arg[strlen(flag)] == '='))

    return true;

  // match to first significant character of short flag
  if (arg[1] != '-' && arg[1 + pos] == short_flag[1]) {
    ++pos;
    return true;
  }

  return false;
}

bool compare_flags(const char* arg, const char* flag , const char* short_flag) {
  int pos = 0;
  return compare_flags(arg, flag, short_flag, pos);
}

