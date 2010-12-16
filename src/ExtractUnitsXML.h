/*
  ExtractUnitsXML.cpp

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

#ifndef INCLUDED_EXTRACTUNITSXML_H
#define INCLUDED_EXTRACTUNITSXML_H

#include "ProcessUnit.hpp"
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>

class ExtractUnitsXML : public ProcessUnit {
 public :
 ExtractUnitsXML(const char* to_dir, const char* filename, const char* output_encoding)
   : to_directory(to_dir), ofilename(filename) {

    handler = xmlFindCharEncodingHandler(output_encoding);
  }

 private :
    const char* to_directory;
    const char* ofilename;
    xmlCharEncodingHandlerPtr handler;
    PROPERTIES_TYPE nsv;
    PROPERTIES_TYPE attrv;

 public :

    // handle root unit of compound document
    void startRootUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix,
                       const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
                       int nb_defaulted, const xmlChar** attributes) {

      // fprintf(stderr, "%s\n", __FUNCTION__);
      // collect namespaces
      collect_namespaces(nb_namespaces, namespaces, nsv);

      // collect attributes
      collect_attributes(nb_attributes, attributes, attrv);
    }

    virtual void startUnit(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                         const xmlChar** attributes) {

      // fprintf(stderr, "%s\n", __FUNCTION__);
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    // open the output text writer stream
    // "-" filename is standard output
    writer = xmlNewTextWriterFilename(ofilename,
					      isoption(*(pstate->poptions), OPTION_COMPRESSED) ? 1 : 0);

    // start this document the same as the current document
    if (!isoption(*(pstate->poptions), OPTION_XMLDECL))
      xmlTextWriterStartDocument(writer,
			       (const char*) ctxt->version,
			       (const char*) (ctxt->encoding ? ctxt->encoding : ctxt->input->encoding),
			       ctxt->standalone ? "yes" : "no");

    // start element with proper prefix
    const char* name = qname((const char*) prefix, (const char*) localname);
    xmlTextWriterStartElement(writer, BAD_CAST name);

    // merge this units namespaces
    collect_namespaces(nb_namespaces, namespaces, nsv);

    // output the merged namespaces
    if (!isoption(*(pstate->poptions), OPTION_NAMESPACEDECL))
      for (int i = 0; i < MAXPROPERTIES; ++i) {
	if (nsv[i].first == "")
	  break;

	xmlTextWriterWriteAttribute(writer, BAD_CAST nsv[i].second.c_str(), BAD_CAST nsv[i].first.c_str());
      }

    // merge this units attributes
    collect_attributes(nb_attributes, attributes, attrv);

    // output the merged attributes
    for (int i = 0; i < MAXPROPERTIES; ++i) {
      if (attrv[i].first == "")
	break;

      xmlTextWriterWriteAttribute(writer, BAD_CAST attrv[i].first.c_str(), BAD_CAST attrv[i].second.c_str());
    }
  }

  virtual void startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix, const xmlChar* URI,
		    int nb_namespaces, const xmlChar** namespaces, int nb_attributes, int nb_defaulted,
                              const xmlChar** attributes) {

    // fprintf(stderr, "%s %s\n", __FUNCTION__, localname);
    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    // start element with proper prefix
    const char* name = qname((const char*) prefix, (const char*) localname);
    xmlTextWriterStartElement(writer, BAD_CAST name);

    // copy namespaces
    if (!isoption(*(pstate->poptions), OPTION_NAMESPACE))
      for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

	const char* name = xmlnsprefix((const char*) namespaces[index]);

	xmlTextWriterWriteAttribute(writer, BAD_CAST name, namespaces[index + 1]);
      }

    // copy attributes
    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {

      const char* name = qname((const char*) attributes[index + 1], (const char*) attributes[index]);

      // write the attribute raw so we don't have to convert
      // the begin/end pointers of the attribute value to a string
      xmlTextWriterStartAttribute(writer, BAD_CAST name);
      xmlTextWriterWriteRawLen(writer, attributes[index + 3],
			       attributes[index + 4] - attributes[index + 3]);
      xmlTextWriterEndAttribute(writer);
    }
  }


  virtual void endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {
    // fprintf(stderr, "%s %s\n", __FUNCTION__, localname);
    xmlTextWriterEndElement(writer);
  }

  virtual void characters(void* ctx, const xmlChar* ch, int len) {

      // fprintf(stderr, "%s\n", __FUNCTION__);
    const char* c = (const char*) ch;
    int pos = 0;
    const char* chend = (const char*) ch + len;
    while (c < chend) {

      switch (*c) {
      case '<' :
	xmlTextWriterWriteRawLen(writer, BAD_CAST c - pos, pos);
	pos = 0;
	xmlTextWriterWriteRawLen(writer, BAD_CAST "&lt;", 4);
	break;

      case '>' :
	xmlTextWriterWriteRawLen(writer, BAD_CAST c - pos, pos);
	pos = 0;
	xmlTextWriterWriteRawLen(writer, BAD_CAST "&gt;", 4);
	break;

      case '&' :
	xmlTextWriterWriteRawLen(writer, BAD_CAST c - pos, pos);
	pos = 0;
	xmlTextWriterWriteRawLen(writer, BAD_CAST "&amp;", 5);
	break;

      default :
	++pos;
	break;
      };
      ++c;
    }

    xmlTextWriterWriteRawLen(writer, BAD_CAST c - pos, pos);
  }

  virtual void endUnit(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {
    // fprintf(stderr, "%s %s\n", __FUNCTION__, localname);
    endElementNs(ctx, localname, prefix, URI);

    xmlTextWriterEndDocument(writer);

    xmlFreeTextWriter(writer);
  }

  // comments
  void comments(void* ctx, const xmlChar* ch) {

    xmlTextWriterWriteComment(writer, ch);
  }

 private :
  const char* filename;      // output filename
  xmlTextWriterPtr writer;   // output text writer
};

#endif
