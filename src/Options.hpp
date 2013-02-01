/*
  Options.hpp

  Copyright (C) 2005-2012  SDML (www.sdml.info)

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

#ifndef INCLUDED_OPTION_HPP
#define INCLUDED_OPTION_HPP

#ifdef __GNUC__
  typedef unsigned long long OPTION_TYPE;
#else
  typedef unsigned __int64 OPTION_TYPE;
#endif

#ifdef __GNUC__
#define ull(a) a##ULL
#else
#define ull(a) a##i64
#endif

inline bool isoption(OPTION_TYPE options, OPTION_TYPE flag) {
  return (flag & options) > 0;
}

// issue debugging elements and declare debugging element namespace
// @src2srcml
const OPTION_TYPE OPTION_DEBUG       = ull(1) << 0;

// unit is nested a root unit
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NESTED      = ull(1) << 1;

// markup literal values
// @src2srcml
const OPTION_TYPE OPTION_LITERAL     = ull(1) << 2;

// output is compressed (gzip)
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_COMPRESSED  = ull(1) << 3;

// output is OPTION_TYPEeractive (good for editing applications) @src2srcml
// @src2srcml
const OPTION_TYPE OPTION_INTERACTIVE = ull(1) << 4;

// input filenames is a file list
// @src2srcml
const OPTION_TYPE OPTION_FILELIST    = ull(1) << 5;

// declared filename
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_FILENAME    = ull(1) << 6;

// declared directory
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_DIRECTORY   = ull(1) << 7;

// declared version
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_VERSION     = ull(1) << 8;

// verbose translation
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_VERBOSE     = ull(1) << 9;

// position
// @src2srcml
const OPTION_TYPE OPTION_POSITION    = ull(1) << 10;

// path
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_PATH        = ull(1) << 11;

// language
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_LANGUAGE    = ull(1) << 12;

// xml encoding
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_XML_ENCODING= ull(1) << 13;

// language
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_SRC_ENCODING= ull(1) << 14;

// text extraction
// @srcml2src
const OPTION_TYPE OPTION_TEXT         = ull(1) << 15;

// language
// @srcml2src
const OPTION_TYPE OPTION_UNIT         = ull(1) << 16;

// expand nested units OPTION_TYPEo separate files
// @srcml2src
const OPTION_TYPE OPTION_EXPAND       = ull(1) << 17;

// output is xml
// @srcml2src
const OPTION_TYPE OPTION_XML          = ull(1) << 18;

// info
// @srcml2src
const OPTION_TYPE OPTION_INFO         = ull(1) << 19;

// long info
// @srcml2src
const OPTION_TYPE OPTION_LONG_INFO    = ull(1) << 20;

// skip encoding
// @src2srcml
// @srcml2src
//  const OPTION_TYPE OPTION_SKIP_ENCODING = ull(1) << 21;

// cpp markup #else sections
// @src2srcml
const OPTION_TYPE OPTION_CPP_MARKUP_ELSE = ull(1) << 22;
const OPTION_TYPE OPTION_XPATH_TOTAL = OPTION_CPP_MARKUP_ELSE;

// cpp markup #if 0 sections
// @src2srcml
const OPTION_TYPE OPTION_CPP_MARKUP_IF0 = ull(1) << 23;

// terminate after this file
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_TERMINATE = ull(1) << 24;

// expression mode
// @src2srcml
const OPTION_TYPE OPTION_EXPRESSION = ull(1) << 25;

// namespace mode
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NAMESPACE = ull(1) << 26;

// omit namespace mode
// @srcml2src
const OPTION_TYPE OPTION_OMIT = ull(1) << 39;

// xmlns mode
// @src2srcml
const OPTION_TYPE OPTION_XMLNS = ull(1) << 27;

// cpp mode
// @src2srcml
const OPTION_TYPE OPTION_CPP = ull(1) << 28;

// xml declaration
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_XMLDECL = ull(1) << 29;

// namespace declaration
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NAMESPACEDECL = ull(1) << 30;

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
const OPTION_TYPE OPTION_OLD_FILENAME  = ull(1) << 31;

// specify input format
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_INPUT_FORMAT  = ull(1) << 32;

// specify ouput format
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_OUTPUT_FORMAT  = ull(1) << 33;

// list option
// @srcml2src
const OPTION_TYPE OPTION_LIST  = ull(1) << 34;

// skip default language option
// @src2srcml
const OPTION_TYPE OPTION_RECURSIVE  = ull(1) << 35;

// skip output of notification messages
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_QUIET  = ull(1) << 36;

// settings
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_SETTINGS  = ull(1) << 37;

// @srcml2src
const OPTION_TYPE OPTION_DIFF  = ull(1) << 38;

// @srcml2src
const OPTION_TYPE OPTION_PRESERVE  = ull(1) << 39;

// @src2srcml
const OPTION_TYPE OPTION_REVISION  = ull(1) << 40;

// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NULL  = ull(1) << 41;

// @src2srcml
const OPTION_TYPE OPTION_SVN  = ull(1) << 42;

#endif
