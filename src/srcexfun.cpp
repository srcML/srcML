/*
  srcexfun.cpp

  Copyright (C) 2009-2010  SDML (www.sdml.info)                                                                                                                                                

  This file is part of the srcML translator.                                                                                                                                                   

  The srcML translator is free software; you can redistribute it and/or modify                                                                                                                 
  it under the terms of the GNU General Public License as published by                                                                                                                           the Free Software Foundation; either version 2 of the License, or                                                                                                                              (at your option) any later version.                                                                                                                                                                                                                                                                                                                                                           The srcML translator is distributed in the hope that it will be useful,                                                                                                                        but WITHOUT ANY WARRANTY; without even the implied warranty of                                                                                                                                 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                                                                                                                  GNU General Public License for more details.                                                                                                                                                                                                                                                                                                                                                  You should have received a copy of the GNU General Public License                                                                                                                              along with the srcML translator; if not, write to the Free Software                                                                                                                            Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "srcexfun.hpp"
#include "srcmlns.hpp"

#include <cstring>
#include <string>

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlreader.h>

static xmlChar* unit_directory = 0;
static xmlChar* unit_filename = 0;

#include <libxml/tree.h>

#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>

#include "URIStream.hpp"

static int Position;
static PROPERTIES_TYPE* pattributes;

static std::vector<struct xpath_ext_function> MACROS;

void setPosition(int n) {
  Position = n;
}

void setRootAttributes(PROPERTIES_TYPE& attributes) {
  pattributes = &attributes;
}

// 
static void srcContextFunction (xmlXPathParserContextPtr ctxt, int nargs) {

    if (nargs != 0) {
	xmlXPathSetArityError(ctxt);
	return;
    }

  valuePush(ctxt, xmlXPathNewFloat(Position));
}

static void srcRootFunction (xmlXPathParserContextPtr ctxt, int nargs) {

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    xmlChar* name = xmlXPathPopString(ctxt);

    const char* value = find(*pattributes, (const char*) name);

    if (value)
      valuePush(ctxt, xmlXPathNewString(BAD_CAST value));
    else
      valuePush(ctxt, NULL);
}

static void srcMacrosFunction (xmlXPathParserContextPtr ctxt, int nargs) {

  // find out which expression is being used based on the name
  unsigned int i;
  for (i = 0; i < MACROS.size(); ++i)
    if (strcmp(MACROS[i].name.c_str(), (const char*) ctxt->context->function) == 0)
        break;

  //fprintf(stderr, "HERE: %s\n", MACROS[i].expr);

  xmlXPathObjectPtr ret = xmlXPathEval(BAD_CAST MACROS[i].expr.c_str(), ctxt->context);

    if (ret) {
      valuePush(ctxt, ret);
    } else {
      fprintf(stderr, "WHOOPS\n");
    }
}

void xpathsrcMLRegister(xmlXPathContextPtr context) {

  xmlXPathRegisterFuncNS(context, (const xmlChar *)"unit",
                         BAD_CAST SRCML_SRC_NS_URI,
			 srcContextFunction);

  xmlXPathRegisterFuncNS(context, (const xmlChar *)"archive",
                         BAD_CAST SRCML_SRC_NS_URI,
			 srcRootFunction);

  for (unsigned int i = 0; i < MACROS.size(); ++i) {

    xmlXPathRegisterFuncNS(context, (const xmlChar *)MACROS[i].name.c_str(),
                         BAD_CAST SRCML_SRC_NS_URI,
			 srcMacrosFunction);
  }
}

void xsltsrcMLRegister () {

  xsltRegisterExtModuleFunction(BAD_CAST "unit",
                         BAD_CAST SRCML_SRC_NS_URI,
                         srcContextFunction);

  xsltRegisterExtModuleFunction(BAD_CAST "archive",
                         BAD_CAST SRCML_SRC_NS_URI,
   		         srcRootFunction);
}

void xpathRegisterDefaultExtensionFunctions() {

  xpathRegisterExtensionFunction("statement", "/src:unit//src:if | /src:unit//src:while");
  xpathRegisterExtensionFunction("if", "/src:unit//src:if");
  xpathRegisterExtensionFunction("while", "/src:unit//src:while");
  xpathRegisterExtensionFunction("nestedwhile", ".//src:while//src:while");
  xpathRegisterExtensionFunction("returntype", "/src:unit//src:function/src:type");
}

void xpathRegisterExtensionFunction(const std::string & name, const std::string & xpath) {

  struct xpath_ext_function xpath_function = {name, xpath};

  MACROS.push_back(xpath_function);
}

const std::vector<xpath_ext_function> getXPathExtensionFunctions() {

  return MACROS;
}
