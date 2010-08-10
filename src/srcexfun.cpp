#include "srcexfun.h"
#include "srcmlns.h"

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

static int Position;
static PROPERTIES_TYPE* pattributes;

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

void xpathsrcMLRegister(xmlXPathContextPtr context) {

  xmlXPathRegisterFuncNS(context, (const xmlChar *)"unit",
			 BAD_CAST "http://www.sdml.info/srcML/src",
			 srcContextFunction);

  xmlXPathRegisterFuncNS(context, (const xmlChar *)"archive",
			 BAD_CAST "http://www.sdml.info/srcML/src",
			 srcRootFunction);
}

void xsltsrcMLRegister () {

  xsltRegisterExtModuleFunction(BAD_CAST "unit",
			  BAD_CAST "http://www.sdml.info/srcML/src",
			  srcContextFunction);

  xsltRegisterExtModuleFunction(BAD_CAST "archive",
			  BAD_CAST "http://www.sdml.info/srcML/src",
			  srcRootFunction);
}
