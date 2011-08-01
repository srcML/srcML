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

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

    // allow for all exslt functions
    //    exsltRegisterAll();

    context = xmlXPathNewContext(ctxt->myDoc);
    xpathsrcMLRegister(context);

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
  }

  virtual void apply(void *ctx) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    // evaluate the xpath
    xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
    if (result_nodes == 0) {
      fprintf(stderr, "%s: Error in executing xpath\n", "srcml2src");
      return;
    }

    // process the resulting nodes
    xmlNodePtr a_node = xmlDocGetRootElement(ctxt->myDoc);
    bool outputunit = false;
    xmlNodePtr onode = 0;
    int result_size = 0;
    nodetype = result_nodes->type;
    switch (nodetype) {

      // node set result
    case XPATH_NODESET:

      // may not have any values
      if (!result_nodes->nodesetval)
        break;

      // may not have any results
      result_size = xmlXPathNodeSetGetLength(result_nodes->nodesetval);
      if (result_size == 0)
        break;

      // Do not need an archive is the input is not an archive, there is
      // one result from the XPath, and the result is a nodeset

      if (!pstate->isarchive && result_size == 1) // && xmlStrEqual(BAD_CAST "unit", xmlXPathNodeSetItem(result_nodes->nodesetval, 1)->name);
        options |= OPTION_XSLT_ALL;

      if (needroot && !isoption(options, OPTION_XSLT_ALL)) {

        // xml declaration
        if (!isoption(options, OPTION_XMLDECL))
          xmlOutputBufferWriteXMLDecl(ctxt, buf);

        // output a root element, just like the one read in
        // note that this has to be ended somewhere
        xmlOutputBufferWriteElementNs(buf, pstate->root.localname, pstate->root.prefix, pstate->root.URI,
                                      pstate->root.nb_namespaces, pstate->root.namespaces,
                                      pstate->root.nb_attributes, pstate->root.nb_defaulted, pstate->root.attributes);
      }
      needroot = false;

      // first time found a node result, so close root unit start tag
      if (!found && !isoption(options, OPTION_XSLT_ALL)) {
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">\n\n"));
        found = true;
      }

      // output all the found nodes
      for (int i = 0; i < xmlXPathNodeSetGetLength(result_nodes->nodesetval); ++i) {

        onode = xmlXPathNodeSetItem(result_nodes->nodesetval, i);

        // output a unit element around the fragment, unless
        // is is already a unit
        outputunit = strcmp("unit", (const char*) onode->name) != 0;

        // if we need a unit, output the start tag.  Line number starts at 1, not 0
        if (outputunit) {

          // output a wrapping element, just like the one read in
          // note that this has to be ended somewhere
          xmlOutputBufferWriteElementNs(buf, pstate->root.localname, pstate->root.prefix, pstate->root.URI,
                                        (data.size() - rootsize) / 2, &data[rootsize],
                                        0, 0, 0);

          // output all the current attributes
          for (xmlAttrPtr pAttr = a_node->properties; pAttr; pAttr = pAttr->next) {
            //        xmlNodeDumpOutput(buf, ctxt->myDoc, (xmlNodePtr) pAttr, 0, 0, 0);

            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" "));
            if (pAttr->ns && pAttr->ns->prefix) {
              xmlOutputBufferWriteString(buf, (const char*) pAttr->ns->prefix);
              xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(":"));
            }
            xmlOutputBufferWriteString(buf, (const char*) pAttr->name);
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("=\""));

	    for (xmlNodePtr child = pAttr->children; child; child = child->next)
	      xmlOutputBufferWriteString(buf, (const char*) child->content);

            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
          }

          // append line number and close unit start tag
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" item=\""));
          char s[50];
          snprintf(s, 50, "%d", i + 1);
          xmlOutputBufferWriteString(buf, s);
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\">"));
        }

        // save the result, but temporarily hide the namespaces
        xmlNsPtr savens = onode && !isoption(options, OPTION_XSLT_ALL) ? onode->nsDef : 0;
        if (savens) {
          onode->nsDef = 0;

          if (!outputunit) {
            // create a new list of namespaces
            xmlNsPtr cur = savens;

            // skip over the namespaces on the root
            for (int i = 0; i < UnitDOM::rootsize / 2; ++i)
              cur = cur->next;

            onode->nsDef = cur;
          }
        }
        xmlNodeDumpOutput(buf, ctxt->myDoc, onode, 0, 0, 0);
        if (savens)
          onode->nsDef = savens;

        // if we need a unit, output the end tag
        if (outputunit)
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("</unit>\n\n"));
        else if (!isoption(options, OPTION_XSLT_ALL))
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));
        else
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n"));
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
      xmlOutputBufferWriteString(buf, (const char*) result_nodes->stringval);
      xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n"));
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
        xmlOutputBufferWriteString(buf, found ? "</unit>\n" : "\n");

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

  virtual void xmlOutputBufferWriteXMLDecl(xmlParserCtxtPtr ctxt, xmlOutputBufferPtr buf) {

    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<?xml version=\""));
    xmlOutputBufferWriteString(buf, (const char*) ctxt->version);
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\" encoding=\""));
    xmlOutputBufferWriteString(buf, (const char*) (ctxt->encoding ? ctxt->encoding : ctxt->input->encoding));
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\" standalone=\""));
    xmlOutputBufferWriteString(buf, ctxt->standalone ? "yes" : "no");
    xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\"?>\n"));
  }

  virtual void xmlOutputBufferWriteElementNs(xmlOutputBufferPtr, const xmlChar* localname, const xmlChar* prefix,
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
};

#endif
