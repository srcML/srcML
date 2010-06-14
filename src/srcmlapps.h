/*
  srcmlapps.h

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

#ifndef INCLUDED_SRCMLAPPS_H
#define INCLUDED_SRCMLAPPS_H

#include <cstring>

#include "version.h"

const char* const VERSION = version();

const char* const COPYRIGHT = "Copyright (C) 2010 SDML";

const char* const EMAIL_ADDRESS = "srcml@sdml.info";

const char* const STDIN = "-";

const char* const OPTION_SEPARATOR = "--";

const char* const HELP_FLAG = "--help";
const char* const HELP_FLAG_SHORT = "-h";

const char* const VERSION_FLAG = "--version";
const char* const VERSION_FLAG_SHORT = "-V";

const char* const LANGUAGE_FLAG = "--language";
const char* const LANGUAGE_FLAG_SHORT = "-l";
const char* const LANGUAGE_FLAG_FULL = "--language=LANG";

const char* const FILENAME_FLAG = "--filename";
const char* const FILENAME_FLAG_SHORT = "-f";
const char* const FILENAME_FLAG_FULL = "--filename=FILE";

const char* const DIRECTORY_FLAG = "--directory";
const char* const DIRECTORY_FLAG_SHORT = "-d";
const char* const DIRECTORY_FLAG_FULL = "--directory=DIR";

const char* const ENCODING_FLAG = "--encoding";
const char* const ENCODING_FLAG_SHORT = "-x";
const char* const ENCODING_FLAG_FULL = "--encoding=ENC";

const char* const TEXTENCODING_FLAG = "--src-encoding";
const char* const TEXTENCODING_FLAG_SHORT = "-t";
const char* const TEXTENCODING_FLAG_FULL = "--src-encoding=ENC";

const char* const NESTED_FLAG = "--nested";
const char* const NESTED_FLAG_SHORT = "-n";

const char* const SRCVERSION_FLAG = "--src-version";
const char* const SRCVERSION_FLAG_SHORT = "-s";
const char* const SRCVERSION_FLAG_FULL = "--src-version=VER";

const char* const VERBOSE_FLAG = "--verbose";
const char* const VERBOSE_FLAG_SHORT = "-v";

const char* const OUTPUT_FLAG = "--output";
const char* const OUTPUT_FLAG_SHORT = "-o";

namespace LanguageName {

const char* const LANGUAGE_C = "C";
const char* const LANGUAGE_CXX = "C++";
const char* const LANGUAGE_CXX_0X = "C++0x";
const char* const LANGUAGE_JAVA = "Java";
const char* const LANGUAGE_ASPECTJ = "AspectJ";

};

const char* const SKIP_ENCODING_FLAG = "--skip-encoding";
const char* const SKIP_ENCODING_FLAG_SHORT = "-k";

const char* const COMPRESSED_FLAG = "--compress";
const char* const COMPRESSED_FLAG_SHORT = "-z";

const char* const NO_XML_DECLARATION_FLAG = "--no-xml-declaration";

const char* const NO_NAMESPACE_DECLARATION_FLAG = "--no-namespace-decl";

/* Return Status */

enum RETURN_STATUS {
  STATUS_SUCCESS = 0,
  STATUS_ERROR = 1,
  STATUS_INPUTFILE_PROBLEM = 2,
  STATUS_UNKNOWN_OPTION = 3,
  STATUS_UNKNOWN_ENCODING = 4,
  STATUS_LIBXML2_FEATURE = 5,
  STATUS_INVALID_LANGUAGE = 6,
  STATUS_LANGUAGE_MISSING = 7,
  STATUS_FILENAME_MISSING = 8,
  STATUS_DIRECTORY_MISSING = 9,
  STATUS_VERSION_MISSING = 10,
  STATUS_SRCENCODING_MISSING = 11,
  STATUS_XMLENCODING_MISSING = 12,
  STATUS_UNIT_MISSING = 13,
  STATUS_UNIT_INVALID = 14,
  STATUS_INVALID_OPTION_COMBINATION = 15,
  STATUS_TERMINATED = 16,
};


// width of long form of flag
const int COL = 20;

// current environment encoding
const char* getencoding();

const char* const DEFAULT_TEXT_ENCODING = "ISO-8859-1";

// extract option from full argument
inline char* extract_option(char* full_argument) { return strchr(full_argument, '='); }

// compare flags to arguments
bool compare_flags(const char* arg, const char* flag , const char* short_flag, int& pos);

bool compare_flags(const char* arg, const char* flag , const char* short_flag);

// filename extension
inline char* filename_ext(char* path) {

  char* pos = rindex(path, '.');
  if (!pos)
    pos = path + strlen(path);

  return pos;
}

inline char* filename_split(char* path, char*& dir, char*& filename) {

  char* point = rindex(path, '/'); 
  if (point) {
    *point = '\0';
    filename = point + 1; 
    dir = path;
  } else {
    filename = path;
    dir = path + strlen(path);
  }

  return path;
}

#endif
