/*
  ExtractUnitsDiffSrc.cpp

  Copyright (C) 2008-2011  SDML (www.sdml.info)

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

#ifndef INCLUDED_EXTRACTUNITSDIFFSRC_HPP
#define INCLUDED_EXTRACTUNITSDIFFSRC_HPP

#include "ExtractUnitsSrc.hpp"
#include "srcmlapps.hpp"
#include <stack>

class ExtractUnitsDiffSrc : public ExtractUnitsSrc {
public :
  ExtractUnitsDiffSrc(const char* to_dir, const char* output_filename, const char* output_encoding, const char* version)
    : ExtractUnitsSrc(to_dir, output_filename, output_encoding) {

    status = strcmp(version, "1") == 0 ? DIFF_OLD : DIFF_NEW;
    st.push(DIFF_COMMON);
  }

private :
  enum DIFF { DIFF_COMMON, DIFF_OLD, DIFF_NEW };
  std::stack<DIFF> st;
  int status;

public :

  // escape control character elements
  void startElementNs(void* ctx, const xmlChar* localname,
                      const xmlChar* prefix, const xmlChar* URI,
                      int nb_namespaces, const xmlChar** namespaces,
                      int nb_attributes, int nb_defaulted,
                      const xmlChar** attributes) {

    if (strcmp((const char*) URI, "http://www.sdml.info/srcDiff") == 0) {

      if (strcmp((const char*) localname, "insert") == 0) {

        st.push(DIFF_OLD);
        return;
      } else if (strcmp((const char*) localname, "delete") == 0) {

        st.push(DIFF_NEW);
        return;
      } else if (strcmp((const char*) localname, "common") == 0) {

        st.push(DIFF_COMMON);
        return;
      }
    }

    if (st.top() != DIFF_COMMON && st.top() != status)
      return;

    ExtractUnitsSrc::startElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces,
                                    nb_attributes, nb_defaulted, attributes);
  }

  void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

    if ((strcmp((const char*) URI, "http://www.sdml.info/srcDiff") == 0 && (
        strcmp((const char*) localname, "insert") == 0
        || strcmp((const char*) localname, "delete") == 0
        || strcmp((const char*) localname, "common") == 0)) {

          st.pop();
          return;
    }

    if (st.top() != DIFF_COMMON && st.top() != status)
      return;

    ExtractUnitsSrc::endElementNs(ctx, localname, prefix, URI);
  }

  void characters(void* ctx, const xmlChar* ch, int len) {

    if (st.top() != DIFF_COMMON && st.top() != status)
      return;

    ExtractUnitsSrc::characters(ctx, ch, len);
  }
};

#endif
