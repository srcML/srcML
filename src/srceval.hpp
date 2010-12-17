#ifndef INCLUDED_SRCEVAL_H
#define INCLUDED_SRCEVAL_H

#include <libxml/xpath.h>
#include <libxml/xmlreader.h>

void xmlUnitDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNodePtr node);

int srceval(const char* context_element,
	    const char* xpath[], 
	    const char* xslt_filename,
	    const char* relaxng_filename,
	    xmlTextReaderPtr reader, const char* ofilename);

void setPosition(int n);

void xsltsrcMLRegister (void);

void
srcContextFunction (xmlXPathParserContextPtr ctxt, int nargs);

#endif
