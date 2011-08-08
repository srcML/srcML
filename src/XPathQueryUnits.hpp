/*
  XPathQueryUnits.cpp

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

#ifndef INCLUDED_XPATHQUERYUNITS_HPP
#define INCLUDED_XPATHQUERYUNITS_HPP

#include <sstream>

#include <libxml/parser.h>
#include <libxml/xpath.h>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libexslt/exslt.h>

#define SIZEPLUSLITERAL(s) sizeof(s) - 1, s
#define LITERALPLUSSIZE(s) s, sizeof(s) - 1

#include "srcexfun.hpp"

#include "UnitDOM.hpp"

class XPathQueryUnits : public UnitDOM {
public :

  XPathQueryUnits(const char* a_context_element, const char* a_ofilename, int options,
                  xmlXPathCompExprPtr compiled_xpath)
    : ofilename(a_ofilename), options(options),
      compiled_xpath(compiled_xpath), total(0), found(false), needroot(true) {
  }

  virtual ~XPathQueryUnits() {}

  virtual void startOutput(void* ctx) {

    //    fprintf(stderr, "%s\n", __FUNCTION__);

    // setup output
    buf = xmlOutputBufferCreateFilename(ofilename, NULL, 0);
    // TODO:  Detect error

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

    // allow for all exslt functions
    //    exsltRegisterAll();

    context = xmlXPathNewContext(ctxt->myDoc);
    // TODO:  Detect error

    xpathsrcMLRegister(context);
    // TODO:  Detect error

    // register standard prefixes for standard namespaces
    const char* prefixes[] = {
      SRCML_SRC_NS_URI, "src",
      SRCML_CPP_NS_URI, SRCML_CPP_NS_PREFIX_DEFAULT,
      SRCML_ERR_NS_URI, SRCML_ERR_NS_PREFIX_DEFAULT,
      SRCML_EXT_LITERAL_NS_URI, SRCML_EXT_LITERAL_NS_PREFIX_DEFAULT,
      SRCML_EXT_OPERATOR_NS_URI, SRCML_EXT_OPERATOR_NS_PREFIX_DEFAULT,
      SRCML_EXT_MODIFIER_NS_URI, SRCML_EXT_MODIFIER_NS_PREFIX_DEFAULT,
      SRCML_EXT_POSITION_NS_URI, SRCML_EXT_POSITION_NS_PREFIX_DEFAULT,
      0, 0
    };

    for (unsigned int i = 0; prefixes[i] != 0; i += 2){
      if (xmlXPathRegisterNs(context, BAD_CAST prefixes[i + 1], BAD_CAST prefixes[i]) == -1) {
        fprintf(stderr, "%s: Unable to register prefix '%s' for namespace %s\n", "srcml2src", prefixes[i + 1], prefixes[i]);
        exit(1);
      }
    }

    // register exslt functions for XPath usage
    if (exsltDateXpathCtxtRegister(context, BAD_CAST "date") == -1) {
      fprintf(stderr, "%s: Unable to register prefix for exslt '%s' function\n",
              "srcml2src", "date");
    }
    if (exsltMathXpathCtxtRegister(context, BAD_CAST "math") == -1) {
      fprintf(stderr, "%s: Unable to register prefix for exslt '%s' function\n",
              "srcml2src", "math");
    }
    if (exsltSetsXpathCtxtRegister(context, BAD_CAST "set") == -1) {
      fprintf(stderr, "%s: Unable to register prefix for exslt '%s' function\n",
              "srcml2src", "set");
    }
    if (exsltStrXpathCtxtRegister(context, BAD_CAST "str") == -1) {
      fprintf(stderr, "%s: Unable to register prefix for exslt '%s' function\n",
              "srcml2src", "str");
    }
  }

  virtual void apply(void *ctx) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    isarchive = pstate->isarchive;

    // evaluate the xpath
    xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
    if (result_nodes == 0) {
      //      fprintf(stderr, "%s: Error in executing xpath\n", "srcml2src");
      return;
    }

    // process the resulting nodes
    xmlNodePtr a_node = xmlDocGetRootElement(ctxt->myDoc);
    std::string wrap;
    bool outputunit = false;
    xmlNodePtr onode = 0;
    int result_size = 0;
    nodetype = result_nodes->type;

    switch (nodetype) {

      // node set result
    case XPATH_NODESET:

      if (needroot && !isoption(options, OPTION_XSLT_ALL)) {

        // xml declaration
        if (!isoption(options, OPTION_XMLDECL))
          xmlOutputBufferWriteXMLDecl(ctxt, buf);

        // output a root element, just like the one read in
        // note that this has to be ended somewhere
        xmlOutputBufferWriteElementNs(buf, pstate->root.localname, pstate->root.prefix, pstate->root.URI,
                                      pstate->root.nb_namespaces, pstate->root.namespaces,
                                      pstate->isarchive ? pstate->root.nb_attributes : 0, pstate->root.nb_defaulted, pstate->root.attributes);

        closetag = true;
      }
      needroot = false;

      // may not have any values or results
      result_size = xmlXPathNodeSetGetLength(result_nodes->nodesetval);
      if (result_size == 0)
        break;

      // opened the root start element before, now need to close it.
      // why not do this when it is started?  May not have any results, and
      // need an empty element
      if (closetag) {
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">\n\n"));
        closetag = false;
      }

      found = true;

      // output all the found nodes
      for (int i = 0; i < result_nodes->nodesetval->nodeNr; ++i) {

        // index into results
        onode = result_nodes->nodesetval->nodeTab[i];

        // output a unit element around the fragment, unless
        // is is already a unit
        outputunit = strcmp("unit", (const char*) onode->name) != 0;

        // if we need a unit, output the start tag.  Line number starts at 1, not 0
        if (outputunit) {

          // form text for wrapping unit.  Cached in a string since we may need it for
          // each result
          if (wrap == "") {

            // output a wrapping element, just like the one read in
            // note that this has to be ended somewhere
            xmlOutputBufferWriteElementNs(wrap, pstate->root.localname, pstate->root.prefix, pstate->root.URI,
                                          (data.size() - rootsize) / 2, &data[rootsize],
                                          0, 0, 0);

            // output all the current attributes from the individual unit
            for (xmlAttrPtr pAttr = a_node->properties; pAttr; pAttr = pAttr->next) {

              wrap.append(LITERALPLUSSIZE(" "));
              if (pAttr->ns && pAttr->ns->prefix) {
                wrap.append((const char*) pAttr->ns->prefix);
                wrap.append(LITERALPLUSSIZE(":"));
              }
              wrap.append((const char*) pAttr->name);
              wrap.append(LITERALPLUSSIZE("=\""));

              for (xmlNodePtr child = pAttr->children; child; child = child->next)
                wrap.append((const char*) child->content);

              wrap.append(LITERALPLUSSIZE("\""));
            }

            wrap.append(LITERALPLUSSIZE(" item=\""));
          }

          // output the start of the wrapping unit
          xmlOutputBufferWrite(buf, wrap.size(), wrap.c_str());

          // append line number and close unit start tag
          const int MAXSSIZE = 50;
          static char itoabuf[MAXSSIZE];
          snprintf(itoabuf, MAXSSIZE, "%d", i + 1);
          xmlOutputBufferWriteString(buf, itoabuf);
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\">"));
        }

        /*
          Three possibilities:

          - Input was an archive, and XPath result is a unit
          Resulting namespaces are those that were on the original unit

          - Input was not an archive, and XPath result is a unit
          Need to split the name

          - XPath result was a node, but not a unit
        */

        // input was an archive, xpath result is a unit
        if (pstate->isarchive && !outputunit) {

          // create a new list of namespaces
          // skip over the namespaces on the root
          xmlNsPtr savens = onode->nsDef;
          onode->nsDef = savens;
          for (int i = 0; i < UnitDOM::rootsize / 2; ++i)
            onode->nsDef = onode->nsDef->next;

          // dump the namespace-modified tree
          xmlNodeDumpOutput(buf, ctxt->myDoc, onode, 0, 0, 0);

          // restore original namespaces
          onode->nsDef = savens;

          // space between internal units
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));

        } else if (!pstate->isarchive && !outputunit) {

          // input was not an archive, xpath result is a unit

          // namespace list only need the cpp namespace, if it exists
          xmlNsPtr savens = onode->nsDef;
          for (onode->nsDef = savens; onode->nsDef; onode->nsDef = onode->nsDef->next)
            if (strcmp((const char*) onode->nsDef->href, SRCML_CPP_NS_URI) == 0)
              break;

          // if we found it, then
          xmlNsPtr keepcppnext = 0;
          if (onode->nsDef) {
            keepcppnext = onode->nsDef->next;
            onode->nsDef->next = 0;
          }

          // dump the namespace-modified tree
          xmlNodeDumpOutput(buf, ctxt->myDoc, onode, 0, 0, 0);

          // restore original namespaces
          if (onode->nsDef)
            onode->nsDef->next = keepcppnext;
          onode->nsDef = savens;

          // space between internal units
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));

        } else {

          // xpath results was not a unit, but was wrapped in a unit

          // dump the namespace-modified tree
          xmlNodeDumpOutput(buf, ctxt->myDoc, onode, 0, 0, 0);

          // wrapped in a unit, so output the end tag
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("</unit>\n\n"));
        }
      }

      break;

      // numeric result
    case XPATH_NUMBER:
      if (!isoption(options, OPTION_XPATH_TOTAL)) {
        std::ostringstream out;
        if ((int)result_nodes->floatval == result_nodes->floatval)
          out << (int)result_nodes->floatval;
        else
          out << result_nodes->floatval;

        xmlOutputBufferWriteString(buf, out.str().c_str());
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n"));
      }
      total += result_nodes->floatval;
      break;

      // boolean result
    case XPATH_BOOLEAN:
      if (!isoption(options, OPTION_XPATH_TOTAL))
        xmlOutputBufferWriteString(buf, result_nodes->boolval ? "true\n" : "false\n");

      result_bool |= result_nodes->boolval;
      break;

      // string
    case XPATH_STRING:
      {
	const char* p = (const char*) result_nodes->stringval;
	const char* pos = p;
	while (*p) {

	  if (p[0] == '&' && p[1] == 'l' && p[2] == 't' && p[3] == ';') {

	    xmlOutputBufferWrite(buf, p - pos, pos);
	    xmlOutputBufferWrite(buf, 1, "<");
	    p += 4;
	    pos = p;

	  } else if (p[0] == '&' && p[1] == 'g' && p[2] == 't' && p[3] == ';') {

	    xmlOutputBufferWrite(buf, p - pos, pos);
	    xmlOutputBufferWrite(buf, 1, ">");
	    p += 4;
	    pos = p;

	  } else if (p[0] == '&' && p[1] == '#' && isdigit(p[2]) && isdigit(p[3]) && p[4] == ';') {

	    xmlOutputBufferWrite(buf, p - pos, pos);
	    char s[3] = { p[2], p[3], '\0' };
	    int c = atoi(s);
	    xmlOutputBufferWrite(buf, 1, (const char*) &c);
	    p += 5;
	    pos = p;

	  } else {

	    ++p;
	  }
	}
	xmlOutputBufferWrite(buf, p - pos, pos);
      }
      break;

    default:
      fprintf(stderr, "Unhandled type %d\n", nodetype);
      break;
    };

    // finished with the result nodes
    xmlXPathFreeObject(result_nodes);
  }

  virtual void endOutput(void *ctx) {

    //    fprintf(stderr, "%s %d\n", __FUNCTION__, nodetype);

    // finalize results
    switch (nodetype) {
    case XPATH_NODESET:

      // root unit end tag
      if (!isoption(options, OPTION_XSLT_ALL))
        xmlOutputBufferWriteString(buf, found ? "</unit>\n" : "/>\n");

      break;

    case XPATH_NUMBER:
      if (isoption(options, OPTION_XPATH_TOTAL)) {
        std::ostringstream out;
        if ((int)total == total)
          out << (int)total;
        else
          out << total;

        xmlOutputBufferWriteString(buf, out.str().c_str());
        xmlOutputBufferWriteString(buf, "\n");
      }
      break;

      // boolean result
    case XPATH_BOOLEAN:
      if (isoption(options, OPTION_XPATH_TOTAL))
        xmlOutputBufferWriteString(buf, result_bool ? "true\n" : "false\n");
      break;

    default:
      break;
    }

    // all done with the buffer
    xmlOutputBufferClose(buf);
  }

  static void xmlOutputBufferWriteXMLDecl(xmlParserCtxtPtr ctxt, xmlOutputBufferPtr buf) {

    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<?xml version=\""));
    xmlOutputBufferWriteString(buf, (const char*) ctxt->version);
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\" encoding=\""));
    xmlOutputBufferWriteString(buf, (const char*) (ctxt->encoding ? ctxt->encoding : ctxt->input->encoding));
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\" standalone=\""));
    xmlOutputBufferWriteString(buf, ctxt->standalone ? "yes" : "no");
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\"?>\n"));
  }

  void xmlOutputBufferWriteElementNs(xmlOutputBufferPtr buf, const xmlChar* localname, const xmlChar* prefix,
                                     const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces,
                                     int nb_attributes, int nb_defaulted, const xmlChar** attributes) {

    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<"));
    if (prefix != NULL) {
      xmlOutputBufferWriteString(buf, (const char*) prefix);
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(":"));
    }
    xmlOutputBufferWriteString(buf, (const char*) localname);

    // output the namespaces
    for (int i = 0; i < nb_namespaces; ++i) {

      // don't put cpp namespace on the root for a non-archive
      if (!isarchive && strcmp((const char*) namespaces[2 * i + 1], SRCML_CPP_NS_URI) == 0)
        continue;

      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" xmlns"));
      if (namespaces[i * 2]) {
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(":"));
        xmlOutputBufferWriteString(buf, (const char*) namespaces[i * 2]);
      }
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("=\""));
      xmlOutputBufferWriteString(buf, (const char*) namespaces[i * 2 + 1]);
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
    }

    // output the attributes
    for (int i = 0; i < nb_attributes; ++i) {

      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" "));
      if (attributes[i * 5 + 1]) {
        xmlOutputBufferWriteString(buf, (const char*) attributes[i * 5 + 1]);
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(":"));
      }
      xmlOutputBufferWriteString(buf, (const char*) attributes[i * 5]);
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("=\""));

      xmlOutputBufferWrite(buf, attributes[i * 5 + 4] - attributes[i * 5 + 3] + 1,
                           (const char*) attributes[i * 5 + 3]);

      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
    }
  }

  void xmlOutputBufferWriteElementNs(std::string& s, const xmlChar* localname, const xmlChar* prefix,
                                     const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces,
                                     int nb_attributes, int nb_defaulted, const xmlChar** attributes) {

    s.append(LITERALPLUSSIZE("<"));
    if (prefix != NULL) {
      s.append((const char*) prefix);
      s.append(LITERALPLUSSIZE(":"));
    }
    s.append((const char*) localname);

    // output the namespaces
    for (int i = 0; i < nb_namespaces; ++i) {

      // only put cpp namespace on the non-root unit for a non-archive
      if (!isarchive && strcmp((const char*) namespaces[2 * i + 1], SRCML_CPP_NS_URI) != 0)
        continue;

      s.append(LITERALPLUSSIZE(" xmlns"));
      if (namespaces[i * 2]) {
        s.append(LITERALPLUSSIZE(":"));
        s.append((const char*) namespaces[i * 2]);
      }
      s.append(LITERALPLUSSIZE("=\""));
      s.append((const char*) namespaces[i * 2 + 1]);
      s.append(LITERALPLUSSIZE("\""));
    }

    // output the attributes
    for (int i = 0; i < nb_attributes; ++i) {

      s.append(LITERALPLUSSIZE(" "));
      if (attributes[i * 5 + 1]) {
        s.append((const char*) attributes[i * 5 + 1]);
        s.append(LITERALPLUSSIZE(":"));
      }
      s.append((const char*) attributes[i * 5]);
      s.append(LITERALPLUSSIZE("=\""));

      s.append((const char*) attributes[i * 5 + 3], attributes[i * 5 + 4] - attributes[i * 5 + 3] + 1);

      s.append(LITERALPLUSSIZE("\""));
    }
  }

private :
  const char* ofilename;
  int options;
  xmlXPathContextPtr context;
  xmlXPathCompExprPtr compiled_xpath;
  double total;
  bool result_bool;
  int nodetype;
  bool found;
  xmlOutputBufferPtr buf;
  bool needroot;
  bool closetag;
  bool isarchive;
};

#endif
