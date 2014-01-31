/*
  SAX2ExtractUnitsSrc.hpp

  Copyright (C) 2008-2012  SDML (www.srcML.org)

  This file is part of the srcML Toolkit.

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Class for straightforward translation
*/

#ifndef INCLUDED_SAX2EXTRACTUNITSSRC
#define INCLUDED_SAX2EXTRACTUNITSSRC

#include <libxml/parser.h>
#include <srcMLUtility.hpp>
#include <ProcessUnit.hpp>
#include <srcml_wrapper.hpp>

#include <algorithm>
#include <vector>

struct Element {

  Element() : localname(0), prefix(0), URI(0),
              nb_namespaces(0), namespaces(0), nb_attributes(0),
              nb_defaulted(0), attributes(0)
  {}

  Element(xmlParserCtxtPtr ctxt, const xmlChar* localname,
	  const xmlChar* prefix, const xmlChar* URI)
    : localname(0), prefix(0), URI(0),
      nb_namespaces(0), namespaces(0), nb_attributes(0),
      nb_defaulted(0), attributes(0)
  {

    // save all the info in case this is not a srcML archive
    this->localname = localname ? (xmlChar*) srcml_strdup_sax((const char*) localname, ctxt) : 0;
    this->prefix = prefix ? (xmlChar*) srcml_strdup_sax((const char*) prefix, ctxt) : 0;
    this->URI = URI ? (xmlChar*) srcml_strdup_sax((const char*) URI, ctxt) : 0;

  }

  Element(xmlParserCtxtPtr ctxt, const xmlChar* localname,
	  const xmlChar* prefix, const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces,
	  int nb_attributes, int nb_defaulted, const xmlChar** attributes)
    : localname(0), prefix(0), URI(0),
      nb_namespaces(0), namespaces(0), nb_attributes(0),
      nb_defaulted(0), attributes(0)
  {

    // save all the info in case this is not a srcML archive
    this->localname = localname ? (xmlChar*) srcml_strdup_sax((const char*) localname, ctxt) : 0;
    this->prefix = prefix ? (xmlChar*) srcml_strdup_sax((const char*) prefix, ctxt) : 0;
    this->URI = URI ? (xmlChar*) srcml_strdup_sax((const char*) URI, ctxt) : 0;

    this->nb_namespaces = nb_namespaces;
    int ns_length = nb_namespaces * 2;
    this->namespaces = (const xmlChar**) srcml_malloc_sax(ns_length * sizeof(namespaces[0]), ctxt);
    for (int i = 0; i < ns_length; ++i)
      if(prefix && namespaces[i] && strcmp((const char *)prefix, (const char *)namespaces[i]) == 0)
	this->namespaces[i] = this->prefix;
      else if(URI && namespaces[i] && strcmp((const char *)URI, (const char *)namespaces[i]) == 0)
	this->namespaces[i] = this->URI;
      else
	this->namespaces[i] = namespaces[i] ? (xmlChar*) srcml_strdup_sax((const char*) namespaces[i], ctxt) : 0;

    this->nb_attributes = nb_attributes;
    this->nb_defaulted = nb_defaulted;

    int nb_length = nb_attributes * 5;
    this->attributes = (const xmlChar**) srcml_malloc_sax(nb_length * sizeof(attributes[0]), ctxt);
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
      this->attributes[index] = attributes[index] ? (xmlChar*) srcml_strdup_sax((const char*) attributes[index], ctxt) : 0;
      this->attributes[index + 1] = attributes[index + 1] ? (xmlChar*) srcml_strdup_sax((const char*) attributes[index + 1], ctxt) : 0;
      this->attributes[index + 2] = attributes[index + 2] ? (xmlChar*) srcml_strdup_sax((const char*) attributes[index + 2], ctxt) : 0;
      long vallength = attributes[index + 4] - attributes[index + 3];
      this->attributes[index + 3] = (const xmlChar*) srcml_malloc_sax(vallength + 1, ctxt);
      memset((void *)this->attributes[index + 3], 0, vallength + 1);
      strncpy((char *) this->attributes[index + 3], (const char*) attributes[index + 3], vallength);
      this->attributes[index + 4] = this->attributes[index + 3] + vallength;
    }

  }

  Element(const Element & element) {

    // save all the info in case this is not a srcML archive
    this->localname = element.localname ? (xmlChar*) strdup((const char*) element.localname) : 0;
    this->prefix = element.prefix ? (xmlChar*) strdup((const char*) element.prefix) : 0;
    this->URI = element.URI ? (xmlChar*) strdup((const char*) element.URI) : 0;

    this->nb_namespaces = element.nb_namespaces;
    int ns_length = element.nb_namespaces * 2;
    this->namespaces = (const xmlChar**) malloc(ns_length * sizeof(element.namespaces[0]));
    for (int i = 0; i < ns_length; ++i)
      if(element.prefix && element.namespaces[i] && strcmp((const char *)element.prefix, (const char *)element.namespaces[i]) == 0)
	this->namespaces[i] = this->prefix;
      else if(element.URI && element.namespaces[i] && strcmp((const char *)element.URI, (const char *)element.namespaces[i]) == 0)
	this->namespaces[i] = this->URI;
      else
	this->namespaces[i] = element.namespaces[i] ? (xmlChar*) strdup((const char*) element.namespaces[i]) : 0;

    this->nb_attributes = element.nb_attributes;
    this->nb_defaulted = element.nb_defaulted;

    int nb_length = element.nb_attributes * 5;
    this->attributes = (const xmlChar**) malloc(nb_length * sizeof(element.attributes[0]));
    for (int i = 0, index = 0; i < element.nb_attributes; ++i, index += 5) {
      this->attributes[index] = element.attributes[index] ? (xmlChar*) strdup((const char*) element.attributes[index]) : 0;
      this->attributes[index + 1] = element.attributes[index + 1] ? (xmlChar*) strdup((const char*) element.attributes[index + 1]) : 0;
      this->attributes[index + 2] = element.attributes[index + 2] ? (xmlChar*) strdup((const char*) element.attributes[index + 2]) : 0;
      long vallength = element.attributes[index + 4] - element.attributes[index + 3];
      this->attributes[index + 3] = (const xmlChar*) malloc(vallength + 1);
      memset((void *)this->attributes[index + 3], 0, vallength + 1);
      strncpy((char *) this->attributes[index + 3], (const char*) element.attributes[index + 3], vallength);
      this->attributes[index + 4] = this->attributes[index + 3] + vallength;
    }
   
  }

  Element & operator=(Element element) {

    this->swap(element);
    return *this;

  }

  void swap(Element & element) {

    std::swap(this->localname, element.localname);
    std::swap(this->prefix, element.prefix);
    std::swap(this->URI, element.URI);
    std::swap(this->nb_namespaces, element.nb_namespaces);
    std::swap(this->namespaces, element.namespaces);
    std::swap(this->nb_attributes, element.nb_attributes);
    std::swap(this->nb_defaulted, element.nb_defaulted);
    std::swap(this->attributes, element.attributes);    

  }

  ~Element() {

    if(namespaces) {

      for(int i = 0; i < nb_namespaces * 2; ++i)
        if(namespaces[i] && namespaces[i] != prefix && namespaces[i] != URI)
          free((void *)namespaces[i]);

      free((void *)namespaces);
    }

    if(localname) free((void *)localname);
    if(prefix) free((void *)prefix);
    if(URI) free((void *)URI);

    if(attributes) {

      for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {
        if(attributes[index])
          free((void *)attributes[index]);
        if(attributes[index + 1])
          free((void *)attributes[index + 1]);
        if(attributes[index + 2])
          free((void *)attributes[index + 2]);
        free((void *)attributes[index + 3]);
      }

      free((void *)attributes);

    }

  }

    const xmlChar* localname;
    const xmlChar* prefix;
    const xmlChar* URI;
    int nb_namespaces;
    const xmlChar** namespaces;
    int nb_attributes;
    int nb_defaulted;
    const xmlChar** attributes;
};

extern const char* diff_version;

class SAX2ExtractUnitsSrc {

public:

    ProcessUnit* pprocess;
    OPTION_TYPE* poptions;
    int unit;
    long count;
    Element root;
    std::vector<Element> macro_list;
    std::string firstcharacters;
    bool isarchive;
    bool rootonly;
    bool stop;
    enum DIFF { DIFF_COMMON, DIFF_OLD, DIFF_NEW };
    std::vector<DIFF> st;
    int status;

public:

    SAX2ExtractUnitsSrc(ProcessUnit* pprocess, OPTION_TYPE* poptions, int unit, const char* diff_version)
        : pprocess(pprocess), poptions(poptions), unit(unit), count(0), isarchive(false), rootonly(false), stop(false)
        {
            if (isoption(*poptions, OPTION_DIFF))
                status = strcmp(diff_version, "1") == 0 ? DIFF_OLD : DIFF_NEW;
            st.push_back(DIFF_COMMON);
        }

    static xmlSAXHandler factory();

    static void startDocument(void *ctx);

    static void endDocument(void *ctx);

    // output all characters to output buffer
    static void charactersPre(void* user_data, const xmlChar* ch, int len);

    // handle root unit of compound document
    static void startElementNsRoot(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                   int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                   const xmlChar** attributes);

    // unit element
    static void startElementNsFirst(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                    const xmlChar** attributes);

    // unit element
    static void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                               int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                               const xmlChar** attributes);

    static void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);

    static void endElementNsSkip(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);

    /*
      Call process methods
    */
    static void startElementNsUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
                                   int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                                   const xmlChar** attributes);

    // output all characters to output buffer
    static void charactersUnit(void* user_data, const xmlChar* ch, int len);
    static void cdatablockUnit(void* user_data, const xmlChar* ch, int len);
    static void commentUnit(void* user_data, const xmlChar* ch);

    static void endElementNsUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI);

    // stop all processing
    static void stopUnit(void* ctx);

};

#endif
