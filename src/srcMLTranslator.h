/*
  srcMLTranslator.h

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
*/

/*
  Class for straightforward source code to srcML translation
*/

#ifndef SRCMLTRANSLATOR_H
#define SRCMLTRANSLATOR_H

#include <iostream>
#include "Language.h"
#include "Options.h"
#include "srcMLOutput.h"

class FileError {};

class srcMLTranslator : public Language {
 public:

  // constructor
  srcMLTranslator(int language,
		  const char* src_encoding,
		  const char* xml_encoding,
		  const char* srcml_filename,
		  int options);

  // translate from input stream to output stream
  virtual void translate(const char* srcml_filename, const char* unit_directory = "",
			 const char* unit_filename = "", const char* unit_version ="");

  // destructor
  virtual ~srcMLTranslator();

 private:
  int language;
  int options;
  srcMLOutput out;
}; 

#endif
