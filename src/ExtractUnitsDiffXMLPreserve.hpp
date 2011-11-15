/*
  ExtractUnitsDiffXMLPreserve.cpp

  Copyright (C) 2011  SDML (www.sdml.info)

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

#ifndef INCLUDED_EXTRACTUNITSDIFFXMLPRESERVE_HPP
#define INCLUDED_EXTRACTUNITSDIFFXMLPRESERVE_HPP

#include "ExtractUnitsXML.hpp"
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <stack>

class ExtractUnitsDiffXMLPreserve : public ExtractUnitsXML {
public :
  ExtractUnitsDiffXMLPreserve(const char* to_dir, const char* filename, const char* output_encoding, const char* uri)
    : ExtractUnitsXML(to_dir, filename, output_encoding), uri(uri) {
  }

private :
  const char* uri;

public :

  // handle root unit of compound document
  void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
                     const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
                     int nb_defaulted, const xmlChar** attributes) {

    // only leave the diff namespace
    int index = 0;
    int i = 0;
    for (i = 0; i < nb_namespaces; ++i, index += 2) {

      const char* nsuri = (const char*) namespaces[index + 1];
      if (strcmp(nsuri, uri) == 0)
        break;
    }

    if (i < nb_namespaces) {
      namespaces[0] = namespaces[index];
      namespaces[1] = namespaces[index + 1];
      nb_namespaces = 1;
    } else 
      nb_namespaces = 0;
    
    ExtractUnitsXML::startRootUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces,
                                   nb_attributes, nb_defaulted, attributes);
  }

  // handle root unit of compound document
  void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
                     const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
                     int nb_defaulted, const xmlChar** attributes) {

    // only leave the diff namespace
    int index = 0;
    int i = 0;
    for (i = 0; i < nb_namespaces; ++i, index += 2) {

      const char* nsuri = (const char*) namespaces[index + 1];
      if (strcmp(nsuri, uri) == 0)
        break;
    }

    if (i < nb_namespaces) {
      namespaces[0] = namespaces[index];
      namespaces[1] = namespaces[index + 1];
      nb_namespaces = 1;
    } else 
      nb_namespaces = 0;
    
    ExtractUnitsXML::startUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces,
                                   nb_attributes, nb_defaulted, attributes);
  }

  void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                      int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                      const xmlChar** attributes) {

    if (strcmp((const char*) URI, uri) == 0)
      ExtractUnitsXML::startElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces,
                                    nb_attributes, nb_defaulted, attributes);
  }

  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    if (strcmp((const char*) URI, uri) == 0)
      ExtractUnitsXML::endElementNs(ctx, localname, prefix, URI);
  }
};

#endif
