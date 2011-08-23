/*
  srcMLTranslator.hpp

  Copyright (C) 2003-2011  SDML (www.sdml.info)

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

#ifndef SRCMLTRANSLATOR_HPP
#define SRCMLTRANSLATOR_HPP

class srcMLTranslatorCore;
class FileError {};

class srcMLTranslator {
 public:

  // constructor
  srcMLTranslator(int language, const char* srcml_filename, OPTION_TYPE& op);

  // constructor
  srcMLTranslator(int language,
		  const char* src_encoding,
		  const char* xml_encoding,
		  const char* srcml_filename,
		  OPTION_TYPE& options,
		  const char* directory,
		  const char* filename,
		  const char* version,
		  const char* uri[],
		  int tabsize
		  );

  void* setInput(const char* path);

  void close();

  // translate from input stream to output stream
  void translate(const char* path,
		 const char* unit_directory = 0,
		 const char* unit_filename = 0,
		 const char* unit_version = 0,
		 int language = 0);

  // destructor
  ~srcMLTranslator();

 private:
  srcMLTranslatorCore * translator;
};

#endif
