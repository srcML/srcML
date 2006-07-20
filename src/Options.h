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
  OPTION_DEBUG       = 0x1,

  // unit is nested a root unit
  OPTION_NESTED      = 0x2,

  // markup literal values
  OPTION_LITERAL     = 0x4,

  // output is compressed (gzip)
  OPTION_COMPRESSED  = 0x8,

  // output is interactive (good for editing applications)
  OPTION_INTERACTIVE = 0x10,

  // input filenames is a file list
  OPTION_FILELIST    = 0x20,

  // declared filename
  OPTION_FILENAME    = 0x40,

  // declared directory
  OPTION_DIRECTORY   = 0x80,

  // declared version
  OPTION_VERSION     = 0x100,

  // verbose translation
  OPTION_VERBOSE     = 0x200,

  // help
  OPTION_HELP        = 0x400,

  // program version
  OPTION_PVERSION    = 0x800,

  // language
  OPTION_LANGUAGE    = 0x1000,

  // xml encoding
  OPTION_XML_ENCODING= 0x2000,

  // language
  OPTION_TEXT_ENCODING= 0x4000,

  // text extraction
  OPTION_TEXT         = 0x8000,

  // language
  OPTION_UNIT         = 0x10000,

  // expand nested units into separate files
  OPTION_EXPAND       = 0x20000,

  // output is xml
  OPTION_XML          = 0x40000,

  // info
  OPTION_INFO         = 0x80000,

  // long info
  OPTION_LONG_INFO    = 0x00100000,

  // skip encoding
  OPTION_SKIP_ENCODING = 0x00200000,

  // cpp markup #else sections
  OPTION_CPP_MARKUP_ELSE = 0x00400000,

  // cpp markup #if 0 sections
  OPTION_CPP_MARKUP_IF0 = 0x00800000,

  // terminate after this file
  OPTION_TERMINATE = 0x01000000,

  // expression mode
  OPTION_EXPRESSION = 0x02000000,

  // namespace mode
  OPTION_NAMESPACE = 0x04000000,

  // xmlns mode
  OPTION_XMLNS = 0x08000000,
};

#endif
