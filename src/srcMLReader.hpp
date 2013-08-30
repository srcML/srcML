/*
  srcMLReader.hpp

  Copyright (C) 2013  SDML (www.sdml.info)

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

#include "Options.hpp"

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include <string>
#include <vector>

#ifndef INCLUDED_SRCML_READER_HPP
#define INCLUDED_SRCML_READER_HPP

class srcMLReader {

  xmlTextReaderPtr reader;
  xmlNodePtr node;
  bool is_archive;
  std::vector<xmlNodePtr> save_nodes;
  bool done;

public:

  srcMLReader(const char * filename);
  srcMLReader(xmlParserInputBufferPtr input);
  ~srcMLReader();

  int readRootUnitAttributes(std::string ** language, std::string ** filename,
                             std::string ** directory, std::string ** version,
                             std::vector<std::string> & attributes,
                             std::vector<std::string> & prefixes,
                             std::vector<std::string> & namespaces,
                             OPTION_TYPE & options,
                             int & tabstop);
  void readUnitAttributesInternal(std::string ** language, std::string ** filename,
                                  std::string ** directory, std::string ** version);
  int readUnitAttributes(std::string ** language, std::string ** filename,
                         std::string ** directory, std::string ** version);
  int readsrcML(xmlTextWriterPtr writer);
  int read(xmlOutputBufferPtr output_buffer);
  std::string * readsrcML();
  std::string * read();

};

#endif
