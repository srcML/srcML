/*
  ExtractUnitsDiffXML.cpp

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

#ifndef INCLUDED_EXTRACTUNITSDIFFXML_HPP
#define INCLUDED_EXTRACTUNITSDIFFXML_HPP

#include "ExtractUnitsXML.hpp"
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include <stack>

class ExtractUnitsDiffXML : public ExtractUnitsXML {
public :
  ExtractUnitsDiffXML(const char* to_dir, const char* filename, const char* output_encoding, const char* version)
    : ExtractUnitsXML(to_dir, filename, output_encoding), version(version) {

    status = strcmp(version, "1") == 0 ? DIFF_OLD : DIFF_NEW;
    st.push(DIFF_COMMON);
  }

private :
  enum DIFF { DIFF_COMMON, DIFF_OLD, DIFF_NEW };
  const char* version;
  std::stack<DIFF> st;
  int status;

public :

  // handle root unit of compound document
  void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
                     const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
                     int nb_defaulted, const xmlChar** attributes) {

    // clear out the diff namespace from the array before passing it on
    bool found = false;
    int deccount = 0;
    for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

      const char* uri = (const char*) namespaces[index + 1];

      if (!found && strcmp(uri, "http://www.sdml.info/srcDiff") == 0) {
        fprintf(stderr, "DEBUG:  %s %s %d\n", __FILE__,  __FUNCTION__, __LINE__);

        found = true;
        deccount = 1;
      } else if (found) {
        namespaces[index - 2] = namespaces[index];
        namespaces[index + 1 - 2] = namespaces[index + 1];
      }
    }
    nb_namespaces -= deccount;
    
    ExtractUnitsXML::startRootUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces,
                                   nb_attributes, nb_defaulted, attributes);
  }

  // handle root unit of compound document
  void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
                     const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
                     int nb_defaulted, const xmlChar** attributes) {

    // clear out the diff namespace from the array before passing it on
    bool found = false;
    int deccount = 0;
    for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

      const char* uri = (const char*) namespaces[index + 1];

      if (strcmp(uri, "http://www.sdml.info/srcDiff") == 0) {
        found = true;
        deccount = 1;
      } else if (found) {
        namespaces[index - 2] = namespaces[index];
        namespaces[index + 1 - 2] = namespaces[index + 1];
      }
    }
    nb_namespaces -= deccount;
    
    ExtractUnitsXML::startUnit(ctx, localname, prefix, URI, nb_namespaces, namespaces,
                                   nb_attributes, nb_defaulted, attributes);
  }

  void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                      int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                      const xmlChar** attributes) {

    if (strcmp((const char*) URI, "http://www.sdml.info/srcDiff") == 0) {

      if (strcmp((const char*) localname, "delete") == 0) {

        st.push(DIFF_OLD);
        return;
      } else if (strcmp((const char*) localname, "insert") == 0) {

        st.push(DIFF_NEW);
        return;
      } else if (strcmp((const char*) localname, "common") == 0) {

        st.push(DIFF_COMMON);
        return;
      }
    }

    if (st.top() != DIFF_COMMON && st.top() != status)
      return;

    ExtractUnitsXML::startElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces,
                                    nb_attributes, nb_defaulted, attributes);
  }


  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    if ((strcmp((const char*) URI, "http://www.sdml.info/srcDiff") == 0 && (
        strcmp((const char*) localname, "insert") == 0
        || strcmp((const char*) localname, "delete") == 0
        || strcmp((const char*) localname, "common") == 0))) {

          st.pop();
          return;
    }

    if (st.top() != DIFF_COMMON && st.top() != status)
      return;

    ExtractUnitsXML::endElementNs(ctx, localname, prefix, URI);
  }

  void characters(void* ctx, const xmlChar* ch, int len) {

    if (st.top() != DIFF_COMMON && st.top() != status)
      return;

    ExtractUnitsXML::characters(ctx, ch, len);
  }

  // comments
  void comments(void* ctx, const xmlChar* ch) {

    if (st.top() != DIFF_COMMON && st.top() != status)
      return;

    ExtractUnitsXML::comments(ctx, ch);
  }
};

#endif
