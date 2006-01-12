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

#include "version.h"

const char* VERSION = version();

const char* COPYRIGHT = "Copyright (C) 2006 SDML";

const char* EMAIL_ADDRESS = "srcmlbugs@sdml.info";

const char* STDIN = "-";

const char* OPTION_SEPARATOR = "--";

const char* HELP_FLAG = "--help";
const char* HELP_FLAG_SHORT = "-h";

const char* VERSION_FLAG = "--version";
const char* VERSION_FLAG_SHORT = "-V";

const char* LANGUAGE_FLAG = "--language";
const char* LANGUAGE_FLAG_SHORT = "-l";
const char* LANGUAGE_FLAG_FULL = "--language=LANG";

const char* FILENAME_FLAG = "--filename";
const char* FILENAME_FLAG_SHORT = "-f";
const char* FILENAME_FLAG_FULL = "--filename=FILE";

const char* DIRECTORY_FLAG = "--directory";
const char* DIRECTORY_FLAG_SHORT = "-d";
const char* DIRECTORY_FLAG_FULL = "--directory=DIR";

const char* ENCODING_FLAG = "--xml-encoding";
const char* ENCODING_FLAG_SHORT = "-x";
const char* ENCODING_FLAG_FULL = "--xml-encoding=ENC";

const char* TEXTENCODING_FLAG = "--src-encoding";
const char* TEXTENCODING_FLAG_SHORT = "-t";
const char* TEXTENCODING_FLAG_FULL = "--src-encoding=ENC";

const char* NESTED_FLAG = "--nested";
const char* NESTED_FLAG_SHORT = "-n";

const char* SRCVERSION_FLAG = "--src-version";
const char* SRCVERSION_FLAG_SHORT = "-s";
const char* SRCVERSION_FLAG_FULL = "--src-version=VER";

const char* VERBOSE_FLAG = "--verbose";
const char* VERBOSE_FLAG_SHORT = "-v";

const char* LANGUAGE_C = "C";
const char* LANGUAGE_CXX = "C++";
const char* LANGUAGE_JAVA = "Java";

// width of long form of flag
const int COL = 20;

// current environment encoding
const char* getencoding();

const char* DEFAULT_TEXT_ENCODING = getencoding();  // "ISO-8859-1";

// extract option from full argument
inline char* extract_option(char* full_argument) { return strchr(full_argument, '='); }

// compare flags to arguments
bool compare_flags(const char* arg, const char* flag , const char* short_flag, int& pos);

bool compare_flags(const char* arg, const char* flag , const char* short_flag);

#endif
