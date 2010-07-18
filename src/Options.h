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
const OPTION_TYPE OPTION_DEBUG       = 0x00000001ull;

// unit is nested a root unit
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NESTED      = 0x00000002ull;

// markup literal values
// @src2srcml
const OPTION_TYPE OPTION_LITERAL     = 0x00000004ull;

// output is compressed (gzip)
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_COMPRESSED  = 0x00000008ull;

// output is OPTION_TYPEeractive (good for editing applications) @src2srcml
// @src2srcml
const OPTION_TYPE OPTION_INTERACTIVE = 0x00000010ull;

// input filenames is a file list
// @src2srcml
const OPTION_TYPE OPTION_FILELIST    = 0x00000020ull;

// declared filename
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_FILENAME    = 0x00000040ull;

// declared directory
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_DIRECTORY   = 0x00000080ull;

// declared version
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_VERSION     = 0x00000100ull;

// verbose translation
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_VERBOSE     = 0x00000200ull;

// position
// @src2srcml
const OPTION_TYPE OPTION_POSITION    = 0x00000400ull;

// path
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_PATH        = 0x00000800ull;

// language
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_LANGUAGE    = 0x00001000ull;

// xml encoding
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_XML_ENCODING= 0x00002000ull;

// language
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_TEXT_ENCODING= 0x00004000ull;

// text extraction
// @srcml2src
const OPTION_TYPE OPTION_TEXT         = 0x00008000ull;

// language
// @srcml2src
const OPTION_TYPE OPTION_UNIT         = 0x00010000ull;

// expand nested units OPTION_TYPEo separate files
// @srcml2src
const OPTION_TYPE OPTION_EXPAND       = 0x00020000ull;

// output is xml
// @srcml2src
const OPTION_TYPE OPTION_XML          = 0x00040000ull;

// info
// @srcml2src
const OPTION_TYPE OPTION_INFO         = 0x00080000ull;

// long info
// @srcml2src
const OPTION_TYPE OPTION_LONG_INFO    = 0x00100000ull;

// skip encoding
// @src2srcml
// @srcml2src
//  const OPTION_TYPE OPTION_SKIP_ENCODING = 0x00200000;

// cpp markup #else sections
// @src2srcml
const OPTION_TYPE OPTION_CPP_MARKUP_ELSE = 0x00400000ull;
const OPTION_TYPE OPTION_XPATH_TOTAL = OPTION_CPP_MARKUP_ELSE;

// cpp markup #if 0 sections
// @src2srcml
const OPTION_TYPE OPTION_CPP_MARKUP_IF0 = 0x00800000ull;

// terminate after this file
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_TERMINATE = 0x01000000ull;

// expression mode
// @src2srcml
const OPTION_TYPE OPTION_EXPRESSION = 0x02000000ull;

// namespace mode
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NAMESPACE = 0x04000000ull;

// xmlns mode
// @src2srcml
const OPTION_TYPE OPTION_XMLNS = 0x08000000ull;

// cpp mode
// @src2srcml
const OPTION_TYPE OPTION_CPP = 0x10000000ull;

// xml declaration
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_XMLDECL = 0x20000000ull;

// namespace declaration
// @src2srcml
// @srcml2src
const OPTION_TYPE OPTION_NAMESPACEDECL = 0x40000000ull;

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

#endif
