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

  XPathQueryUnits(const char* a_context_element, const char* a_fxpath[], const char* a_ofilename, int options,
                  xmlXPathCompExprPtr compiled_xpath)
    : context_element(a_context_element), ofilename(a_ofilename), options(options), fxpath(a_fxpath), total(0),
      prev_unit_filename(0), itemcount(0), found(false), compiled_xpath(compiled_xpath), needroot(true) {
  }

  virtual ~XPathQueryUnits() {

    if (prev_unit_filename)
      free(prev_unit_filename);
  }

  virtual void startOutput(void* ctx) {

    //    fprintf(stderr, "%s\n", __FUNCTION__);

    // setup output
    buf = xmlOutputBufferCreateFilename(ofilename, NULL, 0);

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

    // allow for all exslt functions
    //    exsltRegisterAll();

    //    if (!pstate->fxpath[0][0])
    //      return;

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

    // TODO:  Do we always do this?
    xmlOutputBufferWriteString(buf, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
    xmlOutputBufferWriteString(buf, "<unit");
  }

  virtual void apply(void *ctx) {

    xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;
    SAX2ExtractUnitsSrc* pstate = (SAX2ExtractUnitsSrc*) ctxt->_private;

    // TODO:  Do we always do this?
    xmlXPathObjectPtr result_nodes = xmlXPathCompiledEval(compiled_xpath, context);
    if (result_nodes == 0) {
      fprintf(stderr, "%s: Error in executing xpath\n", "srcml2src");
      return;
    }

    int result_size = 0;

    bool outputunit = false;

    xmlNodePtr onode = 0;

    xmlNodePtr a_node = xmlDocGetRootElement(ctxt->myDoc);

    // for some reason, xmlGetNsProp has an issue with the namespace
    char* unit_filename = (char*) xmlGetProp(a_node, BAD_CAST UNIT_ATTRIBUTE_FILENAME);
    char* unit_directory = (char*) xmlGetProp(a_node, BAD_CAST UNIT_ATTRIBUTE_DIRECTORY);
    char* unit_version = (char*) xmlGetProp(a_node, BAD_CAST UNIT_ATTRIBUTE_VERSION);
    char* unit_language = (char*) xmlGetProp(a_node, BAD_CAST UNIT_ATTRIBUTE_LANGUAGE);

    if (!prev_unit_filename || (unit_filename && strcmp(prev_unit_filename, unit_filename) != 0))
      itemcount = 0;

    // process the resulting nodes
    int nodetype = result_nodes->type;
    switch (nodetype) {

      // node set result
    case XPATH_NODESET:
      /*
        if (needroot) {
        xmlOutputBufferWrite(buf, pstate->rootbuf->use, "<unit );
        xmlBufferFree(pstate->rootbuf);
        needroot = false;
        }
      */
      // may not have any values
      if (!result_nodes->nodesetval)
        break;

      // may not have any results
      result_size = xmlXPathNodeSetGetLength(result_nodes->nodesetval);
      if (result_size == 0)
        break;

      // first time found a node result, so close root unit start tag
      if (!found) {
        xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(">\n\n"));
        found = true;
      }

      // output all the found nodes
      for (int i = 0; i < xmlXPathNodeSetGetLength(result_nodes->nodesetval); ++i) {

        onode = xmlXPathNodeSetItem(result_nodes->nodesetval, i);

        ++itemcount;

        // output a unit element around the fragment, unless
        // is is already a unit
        outputunit = strcmp("unit", (const char*) onode->name) != 0;

        // if we need a unit, output the start tag.  Line number starts at 1, not 0
        if (outputunit) {

          // unit start tag
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("<unit"));

          // output the namespaces
          for (xmlNsPtr pAttr =  a_node->nsDef; pAttr != 0; pAttr = pAttr->next) {

            // see if on the root
            int place = -1;
            for (int i = 0; i < pstate->root.nb_namespaces * 2; i += 2)
              if (strcmp((const char*) pAttr->href, (const char*) pstate->root.namespaces[i + 1]) == 0
                  && ( pAttr->prefix && pstate->root.namespaces[i]
                       ? strcmp((const char*) pAttr->prefix, (const char*) pstate->root.namespaces[i]) == 0
                       : !pAttr->prefix && !pstate->root.namespaces[i])) {
                place = i;
                break;
              }

            if (strcmp((const char*) pAttr->href, "http://www.sdml.info/srcML/src") != 0)
              //            if (!pstate->isnested && strcmp((const char*) pAttr->href, "http://www.sdml.info/srcML/src") != 0)
              place = -1;

            if (place == -1) {
              xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" xmlns"));
              if (pAttr->prefix) {
                xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(":"));
                xmlOutputBufferWriteString(buf, (const char*) pAttr->prefix);
              }
              xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("=\""));
              xmlOutputBufferWriteString(buf, (const char*) pAttr->href);
              xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
            }
          }

          // language attribute
          if (unit_language) {
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" language=\""));
            xmlOutputBufferWriteString(buf, (const char*) unit_language);
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
          }

          // directory attribute
          if (unit_directory) {
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" dir=\""));
            xmlOutputBufferWriteString(buf, (const char*) unit_directory);
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
          }

          // filename attribute
          if (unit_filename) {
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" filename=\""));
            xmlOutputBufferWriteString(buf, (const char*) unit_filename);
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
          }

          // version attribute
          if (unit_version) {
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" version=\""));
            xmlOutputBufferWriteString(buf, (const char*) unit_version);
            xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\""));
          }

          // line number and clost unit start tag
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL(" item=\""));
          char s[50];
          snprintf(s, 50, "%d", itemcount);
          xmlOutputBufferWriteString(buf, s);
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\">"));
        }

        // save the result, but temporarily hide the namespaces
        xmlNsPtr savens = onode && !isoption(options, OPTION_XSLT_ALL) ? onode->nsDef : 0;
        if (savens) {
          onode->nsDef = 0;

          // create a new list of namespaces
          xmlNsPtr ret = NULL;
          xmlNsPtr p = NULL;
          xmlNsPtr cur = savens;
          while (cur != NULL) {

            // see if on the root
            int place = -1;
            for (int i = 0; i < pstate->root.nb_namespaces * 2; i += 2)
              if (strcmp((const char*) cur->href, (const char*) pstate->root.namespaces[i + 1]) == 0
                  && ( cur->prefix && pstate->root.namespaces[i]
                       ? strcmp((const char*) cur->prefix, (const char*) pstate->root.namespaces[i]) == 0
                       : !cur->prefix && !pstate->root.namespaces[i])) {
                place = i;
                break;
              }

            // if its not on the root
            if (place == -1 || (strcmp((const char*) cur->href, "http://www.sdml.info/srcML/cpp") == 0)) {
              //            if (place == -1 || (!pstate->isnested && strcmp((const char*) cur->href, "http://www.sdml.info/srcML/cpp") == 0)) {
              xmlNsPtr q = xmlCopyNamespace(cur);
              if (p == NULL) {
                ret = p = q;
              } else {
                p->next = q;
                p = q;
              }
            }
            cur = cur->next;
          }
          onode->nsDef = ret;
        }
        xmlNodeDumpOutput(buf, ctxt->myDoc, onode, 0, 0, 0);
        if (savens) {
          xmlFreeNsList(onode->nsDef);
          onode->nsDef = savens;
        }

        // if we need a unit, output the end tag
        if (outputunit)
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("</unit>\n\n"));
        else
          xmlOutputBufferWrite(buf, SIZEPLUSLITERAL("\n\n"));
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
        xmlOutputBufferWriteString(buf, "\n");
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
      xmlOutputBufferWriteString(buf, "\n");
      break;

    default:
      fprintf(stderr, "Unhandled type %d\n", nodetype);
      break;
    };
    // finished with the result nodes
    xmlXPathFreeObject(result_nodes);

    // save the previous filename to see if there is a transition for
    // item numbering

    if (prev_unit_filename)
      free(prev_unit_filename);
    prev_unit_filename = unit_filename ? strdup(unit_filename) : 0;

    xmlFree(unit_filename);
    xmlFree(unit_directory);
    xmlFree(unit_version);
  }

  virtual void endOutput(void *ctx) {

    //   fprintf(stderr, "%s\n", __FUNCTION__);

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

private :
  const char* context_element;
  const char* ofilename;
  int options;
  const char** fxpath;
  xmlXPathContextPtr context;
  xmlXPathCompExprPtr compiled_xpath;
  double total;
  bool result_bool;
  int nodetype;
  char* prev_unit_filename;
  int itemcount;
  bool found;
  xmlOutputBufferPtr buf;
  bool needroot;
};

#endif
