/*
  srcMLUtility.cpp

  Copyright (C) 2004-2006  SDML (www.sdml.info)

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

#include "SAX2TextWriter.h"
#include "SAX2UnitDOM.h"
#include "SAX2Utilities.h"
#include "srcmlns.h"

#include <cstdio>
#include <cstring>
#include <libxml/xmlwriter.h>
#include <libxml/parser.h>
#include <libxml/parserInternals.h>
#include "Options.h"
#include "srcmlns.h"

#include "srcexfun.h"

#include <libxml/parserInternals.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

#include <libxml/SAX2.h>

static int counter = 0;
static int tcounter = 0;

SAX2UnitDOM::SAX2UnitDOM()/* :
			     SAX2TextWriter(ofilename, options, unit), nsv(nsv), attrv(attrv), placescount(0), placesunit(0)*/ {

}

xmlSAXHandler SAX2UnitDOM::factory() {

  xmlSAXHandler sax = { 0 };

  sax.initialized    = XML_SAX2_MAGIC;

  sax.startDocument  = &SAX2UnitDOM::startDocument; //xmlSAX2StartDocument;
  sax.endDocument    = &SAX2UnitDOM::endDocument;   //xmlSAX2EndDocument;
  sax.startElementNs = &SAX2UnitDOM::startElementNs;
  sax.endElementNs   = &SAX2UnitDOM::endElementNs;
  sax.characters     = &SAX2UnitDOM::characters;
  //  sax.characters     = xmlSAX2Characters;
  //  sax.ignorableWhitespace = &SAX2UnitDOM::ignorableWhitespace;
  sax.comment        = xmlSAX2Comment;

  return sax;
}

static xmlNodePtr unitnode;

xsltStylesheetPtr xslt;

// setup output
xmlOutputBufferPtr buf;


// start document
void SAX2UnitDOM::startDocument(void *ctx) {

    // allow for all exslt functions
    exsltRegisterAll();

    xsltsrcMLRegister();

    // parse the stylesheet
    xslt = xsltParseStylesheetFile(BAD_CAST "copy.xsl");

    // setup output
    buf = xmlOutputBufferCreateFilename("/dev/stdout", NULL, 0);

    xmlSAX2StartDocument(ctx);
}

// end document
void SAX2UnitDOM::endDocument(void *ctx) {

  xmlSAX2EndDocument(ctx);

  // all done with the buffer
  xmlOutputBufferClose(buf);
}

// handle root unit of compound document
void SAX2UnitDOM::startElementNs(void* ctx, const xmlChar* localname, const xmlChar* prefix,
		    const xmlChar* URI, int nb_namespaces, const xmlChar** namespaces, int nb_attributes,
		    int nb_defaulted, const xmlChar** attributes) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  int depth = ctxt->nodeNr;

  if (depth == 1)
    ctxt->input->line = 0;

  xmlSAX2StartElementNs(ctx, localname, prefix, URI, nb_namespaces, namespaces, nb_attributes,
			nb_defaulted, attributes);
  if (depth == 1) {
    unitnode = ctxt->node;
    ctxt->sax->startElementNs = xmlSAX2StartElementNs;
    ++counter;

    fprintf(stderr, "Position:  %d\n", counter);
    /*
    if (counter == 13068) {
      fprintf(stderr, "Turning whitespace off\n");
      ctxt->sax->ignorableWhitespace = 0; // &SAX2UnitDOM::ignorableWhitespace;
      ctxt->sax->characters = 0;

    }    else{
      ctxt->sax->ignorableWhitespace = &SAX2UnitDOM::ignorableWhitespace;
      ctxt->sax->characters = xmlSAX2Characters;
    }
    */
    fflush(stderr);

    tcounter = 0;
  }
}

// end unit element and current file/buffer (started by startElementNs
void SAX2UnitDOM::endElementNs(void *ctx, const xmlChar *localname, const xmlChar *prefix, const xmlChar *URI) {

  xmlParserCtxtPtr ctxt = (xmlParserCtxtPtr) ctx;

  xmlSAX2EndElementNs(ctx, localname, prefix, URI);

  if (ctxt->nodeNr == 1) {

    ctxt->sax->startElementNs = &SAX2UnitDOM::startElementNs;

    // keep track if we actually get any results
    bool found = false;

       // apply the style sheet to the extracted doc
       xmlDocPtr res = xsltApplyStylesheet(xslt, ctxt->myDoc, NULL);

       if (res) {

	 // if in per-unit mode and this is the first result found
	 //	 if (!found && !isoption(options, OPTION_XSLT_ALL)) {
	   xmlOutputBufferWrite(buf, 3, ">\n\n");
	   found = true;
	   //	 }

	 // output the result of the stylesheet
	 xmlNodePtr resroot = xmlDocGetRootElement(res);
	 xmlNsPtr savens = resroot ? resroot->nsDef : 0;
	 if (savens /* && !isoption(options, OPTION_XSLT_ALL)*/)
	   resroot->nsDef = 0;
	 //	 xsltSaveResultTo(buf, res, xslt);
	 xmlNodeDumpOutput(buf, res, resroot, 0, 0, 0);
	 if (savens /* && !isoption(options, OPTION_XSLT_ALL)*/)
	    resroot->nsDef = savens;

	 // put some space between this unit and the next one
	 //	 if (!isoption(options, OPTION_XSLT_ALL))
	   xmlOutputBufferWrite(buf, 1, "\n");
       }

       // finished with the result of the transformation
       xmlFreeDoc(res);

    //    fprintf(stderr, "Line: %d\n", unitnode->line);

    //    xmlDocPtr doc = xmlNewDoc(NULL);

    //    xmlDocSetRootElement(doc, unitnode);

    xmlUnlinkNode(unitnode);
    
    xmlFreeNode(unitnode);

    unitnode = 0;
  }

  /*
    SAX2UnitDOM* pstate = (SAX2UnitDOM*) ctx;

    if (pstate->ctxt->nameNr != 2)
      return;

    // check that this is a nested file
    if (pstate->count == 0 && !(strcmp((const char*) localname, "unit") == 0 &&
	  strcmp((const char*) URI, SRCML_SRC_NS_URI) == 0)) {
      xmlStopParser(pstate->ctxt);
      return;
    }

    ++(pstate->count);

    // output file status message if in verbose mode
    if (isoption(pstate->options, OPTION_VERBOSE)) {
      for (int i = 0; i < pstate->placescount; ++i)
	fprintf(stderr, "\b");
      fprintf(stderr, "%ld", pstate->count);

      if (pstate->count == pstate->placesunit) {
	pstate->placesunit *= 10;
	++pstate->placescount;
      }
    }

    if (pstate->count < pstate->unit - 1)
      return;

    // now ready for the next unit, to treat as root
    pstate->ctxt->sax->startElementNs = &startElementNsUnit;
    pstate->ctxt->sax->endElementNs = 0;
  */
}

void SAX2UnitDOM::ignorableWhitespace(void* ctx, const xmlChar* ch, int len) {
  /*
  if (counter >= 13067 && counter <= 14000)
    fprintf(stderr, "%d: Text: %d\n", tcounter, len);

  fflush(stderr);
  ++tcounter;
  if (counter != 13068)
  */
      xmlSAX2Characters(ctx, ch, len);
}

void SAX2UnitDOM::characters(void* ctx, const xmlChar* ch, int len) {
  /*
  if (counter >= 13067 && counter <= 14000)
    fprintf(stderr, "%d: Text: %d\n", tcounter, len);

  fflush(stderr);
  ++tcounter;
  if (counter != 13068)
  */
      xmlSAX2Characters(ctx, ch, len);
}
