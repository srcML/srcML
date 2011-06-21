#include "srcexfun.hpp"
#include "srcmlns.hpp"

#include <cstring>

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

static struct { const char* name; const char* expr; } MACROS[] = {
  { "statement", "/src:unit//src:if | /src:unit//src:while" },
  { "if", "/src:unit//src:if" },
  { "while", "/src:unit//src:while" },
  { "nestedwhile", ".//src:while//src:while" },
  { "returntype", "/src:unit//src:function/src:type" },
};

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
    for (i = 0; i < sizeof(MACROS) / sizeof(MACROS[0]); ++i)
      if (strcmp(MACROS[i].name, (const char*) ctxt->context->function) == 0)
        break;

    //    fprintf(stderr, "HERE: %s\n", MACROS[i].expr);

    xmlXPathObjectPtr ret = xmlXPathEval(BAD_CAST MACROS[i].expr, ctxt->context);

    if (ret) {
      valuePush(ctxt, ret);
    } else {
      fprintf(stderr, "WHOOPS\n");
    }
}

void xpathsrcMLRegister(xmlXPathContextPtr context) {

  xmlXPathRegisterFuncNS(context, (const xmlChar *)"unit",
			 BAD_CAST "http://www.sdml.info/srcML/src",
			 srcContextFunction);

  xmlXPathRegisterFuncNS(context, (const xmlChar *)"archive",
 			 BAD_CAST "http://www.sdml.info/srcML/src",
			 srcRootFunction);

  for (unsigned int i = 0; i < sizeof(MACROS) / sizeof(MACROS[0]); ++i) {

    xmlXPathRegisterFuncNS(context, (const xmlChar *)MACROS[i].name,
 			 BAD_CAST "http://www.sdml.info/srcML/src",
			 srcMacrosFunction);
  }
}

void xsltsrcMLRegister () {

  xsltRegisterExtModuleFunction(BAD_CAST "unit",
			  BAD_CAST "http://www.sdml.info/srcML/src",
			  srcContextFunction);

  xsltRegisterExtModuleFunction(BAD_CAST "archive",
			  BAD_CAST "http://www.sdml.info/srcML/src",
			  srcRootFunction);
}
