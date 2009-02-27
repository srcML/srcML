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

inline bool isoption(int options, int flag) {
  return (flag & options) > 0;
}

enum Options {

  // issue debugging elements and declare debugging element namespace
  // @src2srcml
  OPTION_DEBUG       = 0x00000001,

  // unit is nested a root unit
  // @src2srcml
  // @srcml2src
  OPTION_NESTED      = 0x00000002,

  // markup literal values
  // @src2srcml
  OPTION_LITERAL     = 0x00000004,

  // output is compressed (gzip)
  // @src2srcml
  // @srcml2src
  OPTION_COMPRESSED  = 0x00000008,

  // output is interactive (good for editing applications) @src2srcml
  // @src2srcml
  OPTION_INTERACTIVE = 0x00000010,

  // input filenames is a file list
  // @src2srcml
  OPTION_FILELIST    = 0x00000020,

  // declared filename
  // @src2srcml
  // @srcml2src
  OPTION_FILENAME    = 0x00000040,

  // declared directory
  // @src2srcml
  // @srcml2src
  OPTION_DIRECTORY   = 0x00000080,

  // declared version
  // @src2srcml
  // @srcml2src
  OPTION_VERSION     = 0x00000100,

  // verbose translation
  // @src2srcml
  // @srcml2src
  OPTION_VERBOSE     = 0x00000200,

  // help
  // @src2srcml
  // @srcml2src
  OPTION_HELP        = 0x00000400,

  // program version
  // @src2srcml
  // @srcml2src
  OPTION_PVERSION    = 0x00000800,

  // language
  // @src2srcml
  // @srcml2src
  OPTION_LANGUAGE    = 0x00001000,

  // xml encoding
  // @src2srcml
  // @srcml2src
  OPTION_XML_ENCODING= 0x00002000,

  // language
  // @src2srcml
  // @srcml2src
  OPTION_TEXT_ENCODING= 0x00004000,

  // text extraction
  // @src2srcml
  // @srcml2src
  OPTION_TEXT         = 0x00008000,

  // language
  // @srcml2src
  OPTION_UNIT         = 0x00010000,

  // expand nested units into separate files
  // @srcml2src
  OPTION_EXPAND       = 0x00020000,

  // output is xml
  // @src2srcml
  // @srcml2src
  OPTION_XML          = 0x00040000,

  // info
  // @srcml2src
  OPTION_INFO         = 0x00080000,

  // long info
  // @srcml2src
  OPTION_LONG_INFO    = 0x00100000,

  // skip encoding
  // @src2srcml
  // @srcml2src
  OPTION_SKIP_ENCODING = 0x00200000,

  // cpp markup #else sections
  // @src2srcml
  OPTION_CPP_MARKUP_ELSE = 0x00400000,

  // cpp markup #if 0 sections
  // @src2srcml
  OPTION_CPP_MARKUP_IF0 = 0x00800000,

  // terminate after this file
  // @src2srcml
  // @srcml2src
  OPTION_TERMINATE = 0x01000000,

  // expression mode
  // @src2srcml
  OPTION_EXPRESSION = 0x02000000,

  // namespace mode
  // @src2srcml
  // @srcml2src
  OPTION_NAMESPACE = 0x04000000,

  // xmlns mode
  // @src2srcml
  OPTION_XMLNS = 0x08000000,

  // cpp mode
  // @src2srcml
  OPTION_CPP = 0x10000000,

  // xml declaration
  // @src2srcml
  // @srcml2src
  OPTION_XMLDECL = 0x20000000,

  // namespace declaration
  // @src2srcml
  // @srcml2src
  OPTION_NAMESPACEDECL = 0x40000000,

  // self version
  //  OPTION_SELF_VERSION = 0x02000000,

  // markup operators
  // @src2srcml
  OPTION_OPERATOR     = OPTION_UNIT,

  // markup type modifiers
  // @src2srcml
  OPTION_MODIFIER     = OPTION_EXPAND,

  // xpath evaluation
  // @srcml2src
  OPTION_XPATH        = OPTION_LITERAL,

  // xslt evaluation
  // @srcml2src
  OPTION_XSLT         = OPTION_CPP,

  // relaxng evaluation
  // @srcml2src
  OPTION_RELAXNG      = OPTION_EXPRESSION,

};

#endif
