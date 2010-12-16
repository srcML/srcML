/*
  ListUnits.cpp

  Copyright (C) 2010  SDML (www.sdml.info)

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

#ifndef INCLUDED_LISTUNITSSRC_H
#define INCLUDED_LISTUNITSSRC_H

#include "ProcessUnit.hpp"

class ListUnits : public ProcessUnit {
 public :
  ListUnits() {}

 public :

  virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                         int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar** attributes) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    // find the directory
    int dir_index = find_attribute_index(nb_attributes, attributes, UNIT_ATTRIBUTE_DIRECTORY);
    int dir_size = dir_index != -1 ? (const char*) attributes[dir_index + 4] - (const char*) attributes[dir_index + 3] : 0;

    // find the filename
    int filename_index = find_attribute_index(nb_attributes, attributes, UNIT_ATTRIBUTE_FILENAME);
    int filename_size = filename_index != -1 ? (const char*) attributes[filename_index + 4] - (const char*) attributes[filename_index + 3] : 0;

    // whole purpose
    fprintf(stdout, "%ld\t%.*s%.*s%.*s\n", pstate->count,
            dir_size, (char*) attributes[dir_index + 3],
            dir_size ? 1 : 0, "/",
            filename_size, (char*) attributes[filename_index + 3]);

  }
};

#endif
