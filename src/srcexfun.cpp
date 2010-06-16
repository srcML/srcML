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

int Position;

void setPosition(int n) {
  Position = n;
}

// 
static void srcContextFunction (xmlXPathParserContextPtr ctxt, int nargs) {
  /*
    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    xmlChar* arg = xmlXPathPopString(ctxt);
  */
    //    xmlXPathObjectPtr ret = xmlXPathEval(arg, ctxt->context);

    //    valuePush(ctxt, ret);

  valuePush(ctxt, xmlXPathNewFloat(Position));
}

static void srcRootFunction (xmlXPathParserContextPtr ctxt, int nargs) {

    if (nargs != 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    xmlChar* arg1 = xmlXPathPopString(ctxt);

    xmlXPathObjectPtr ret = xmlXPathEval(arg1, ctxt->context);

    valuePush(ctxt, ret);
}

void xpathsrcMLRegister(xmlXPathContextPtr context) {

  xmlXPathRegisterFuncNS(context, (const xmlChar *)"rootunit",
			 BAD_CAST "http://www.sdml.info/srcML/src",
			 srcContextFunction);
}

void xsltsrcMLRegister () {

    xsltRegisterExtModuleFunction(BAD_CAST "context",
				  BAD_CAST "http://www.sdml.info/srcML/src",
				  srcContextFunction);
}
