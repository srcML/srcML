/*
  Options.h

  Copyright (C) 2005, 2006  SDML (www.sdml.info)

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

  Options for translation output.
*/

#ifndef OUTPUT_OPTION_H
#define OUTPUT_OPTION_H

#ifdef __GNUC__
  typedef unsigned long long OPTION_TYPE;
#else
  typedef unsigned __int64 OPTION_TYPE;
#endif

inline bool isoption(OPTION_TYPE options, OPTION_TYPE flag) {
  return (flag & options) > 0;
}

// issue debugging elements and declare debugging element namespace
// @src2srcml
const OPTION_TYPE OPTION_DEBUG       = 1ull << 0;

// unit is nested a root unit
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NESTED      = 1ull << 1;

// markup literal values
// @src2srcml
const OPTION_TYPE OPTION_LITERAL     = 1ull << 2;

// output is compressed (gzip)
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_COMPRESSED  = 1ull << 3;

// output is OPTION_TYPEeractive (good for editing applications) @src2srcml
// @src2srcml
const OPTION_TYPE OPTION_INTERACTIVE = 1ull << 4;

// input filenames is a file list
// @src2srcml
const OPTION_TYPE OPTION_FILELIST    = 1ull << 5;

// declared filename
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_FILENAME    = 1ull << 6;

// declared directory
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_DIRECTORY   = 1ull << 7;

// declared version
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_VERSION     = 1ull << 8;

// verbose translation
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_VERBOSE     = 1ull << 9;

// position
// @src2srcml
const OPTION_TYPE OPTION_POSITION    = 1ull << 10;

// path
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_PATH        = 1ull << 11;

// language
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_LANGUAGE    = 1ull << 12;

// xml encoding
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_XML_ENCODING= 1ull << 13;

// language
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_TEXT_ENCODING= 1ull << 14;

// text extraction
// @srcml2src
const OPTION_TYPE OPTION_TEXT         = 1ull << 15;

// language
// @srcml2src
const OPTION_TYPE OPTION_UNIT         = 1ull << 16;

// expand nested units OPTION_TYPEo separate files
// @srcml2src
const OPTION_TYPE OPTION_EXPAND       = 1ull << 17;

// output is xml
// @srcml2src
const OPTION_TYPE OPTION_XML          = 1ull << 18;

// info
// @srcml2src
const OPTION_TYPE OPTION_INFO         = 1ull << 19;

// long info
// @srcml2src
const OPTION_TYPE OPTION_LONG_INFO    = 1ull << 20;

// skip encoding
// @src2srcml
// @srcml2src
//  const OPTION_TYPE OPTION_SKIP_ENCODING = 1ull << 21;

// cpp markup #else sections
// @src2srcml
const OPTION_TYPE OPTION_CPP_MARKUP_ELSE = 1ull << 22;
const OPTION_TYPE OPTION_XPATH_TOTAL = OPTION_CPP_MARKUP_ELSE;

// cpp markup #if 0 sections
// @src2srcml
const OPTION_TYPE OPTION_CPP_MARKUP_IF0 = 1ull << 23;

// terminate after this file
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_TERMINATE = 1ull << 24;

// expression mode
// @src2srcml
const OPTION_TYPE OPTION_EXPRESSION = 1ull << 25;

// namespace mode
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NAMESPACE = 1ull << 26;

// xmlns mode
// @src2srcml
const OPTION_TYPE OPTION_XMLNS = 1ull << 27;

// cpp mode
// @src2srcml
const OPTION_TYPE OPTION_CPP = 1ull << 28;

// xml declaration
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_XMLDECL = 1ull << 29;

// namespace declaration
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NAMESPACEDECL = 0x1ull << 30;

// self version
//  const OPTION_TYPE OPTION_SELF_VERSION = 0x02000000;

// markup operators
// @src2srcml
const OPTION_TYPE OPTION_OPERATOR     = OPTION_UNIT;

// markup type modifiers
// @src2srcml
const OPTION_TYPE OPTION_MODIFIER     = OPTION_EXPAND;

// xpath evaluation
// @srcml2src
const OPTION_TYPE OPTION_XPATH        = OPTION_LITERAL;

// xslt evaluation
// @srcml2src
const OPTION_TYPE OPTION_XSLT         = OPTION_CPP;

// xslt all evaluation
// @srcml2src
const OPTION_TYPE OPTION_XSLT_ALL      = OPTION_XMLNS;

// relaxng evaluation
// @srcml2src
const OPTION_TYPE OPTION_RELAXNG      = OPTION_EXPRESSION;

// use old filename behavior, where it was just the base filename
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_OLD_FILENAME  = 0x100000000ull;

// specify input format
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_INPUT_FORMAT  = 0x200000000ull;

// specify ouput format
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_OUTPUT_FORMAT  = 0x400000000ull;

// list option
// @srcml2src
const OPTION_TYPE OPTION_LIST  = 0x800000000ull;

// skip default language option
// @src2srcml
const OPTION_TYPE OPTION_SKIP_DEFAULT  = 0x1000000000ull;

// skip default language option
// @src2srcml
const OPTION_TYPE OPTION_RECURSIVE  = 0x2000000000ull;

// skip output of notification messages
const OPTION_TYPE OPTION_QUIET  = 0x4000000000ull;

// settings
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_SETTINGS  = 0x8000000000ull;

#endif
