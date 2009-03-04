/*
  xpathproc

  A simple XPath expression evaluator.

  Usage:  xpathproc xpath-expression xml-file
*/

#include "srcxslteval.h"
#include "srceval.h"
#include "Options.h"

#include <cstring>
#include <iostream>
#include <cstdio>

#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

int srcxslteval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename, const char* params[],
		int paramcount, int options) {

  // allow for all exslt functions
  exsltRegisterAll();

  // parse the stylesheet
  xsltStylesheetPtr xslt = xsltParseStylesheetFile(BAD_CAST xpath);

  // read the first node
  xmlTextReaderRead(reader);

  // setup output
  xmlOutputBufferPtr buf = xmlOutputBufferCreateFilename(ofilename, NULL, 0);

  const int MAX_ATTR_SIZE = 50;

  // insert the standard parameters into the param list
  char posstr[MAX_ATTR_SIZE];
  params[paramcount++] = "src:position";
  params[paramcount++] = posstr;

  // filename
  char filename[MAX_ATTR_SIZE] = "'";
  const char* unit_filename = (const char*) xmlTextReaderGetAttribute(reader, BAD_CAST "filename");
  if (unit_filename) {
    params[paramcount++] = "src:filename";
    strcpy(filename + 1, unit_filename);
    strcat(filename, "'");
    params[paramcount++] = filename;
  }

  // directory
  char directory[MAX_ATTR_SIZE] = "'";
  const char* unit_directory = (const char*) xmlTextReaderGetAttribute(reader, BAD_CAST "dir");
  if (unit_directory) {
    params[paramcount++] = "src:directory";
    strcpy(directory + 1, unit_directory);
    strcat(directory, "'");
    params[paramcount++] = directory;
  }

  // language
  char language[MAX_ATTR_SIZE] = "'";
  const char* unit_language = (const char*) xmlTextReaderGetAttribute(reader, BAD_CAST "language");
  if (unit_language) {
    params[paramcount++] = "src:language";
    strcpy(language + 1, unit_language);
    strcat(language, "'");
    params[paramcount++] = language;
  }

  // version
  char version[MAX_ATTR_SIZE] = "'";
  const char* unit_version = (const char*) xmlTextReaderGetAttribute(reader, BAD_CAST "version");
  if (unit_version) {
    params[paramcount++] = "src:version";
    strcpy(version + 1, unit_version);
    strcat(version, "'");
    params[paramcount++] = version;
  }

  params[paramcount] = NULL;

  if (!isoption(options, OPTION_XSLT_ALL)) {

    // copy the start tag of the root element unit
    xmlUnitDumpOutputBuffer(buf, xmlTextReaderCurrentNode(reader));
  }

  int position = 0;

  bool found = false;

  while (1) {

    // read a node
    if (!isoption(options, OPTION_XSLT_ALL)) {
      int ret = xmlTextReaderRead(reader);
      if (ret != 1)
	break;
    }

    // nested unit tag
    if (isoption(options, OPTION_XSLT_ALL)  || (xmlTextReaderDepth(reader) == 1 &&
	 xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
       	xmlTextReaderConstName(reader)[0] == 'u')) {

       // mark the internal parameter with the position
       ++position;
       sprintf(posstr, "'%d'", position);

       // expand this unit to make it the context
       xmlTextReaderExpand(reader);

       // apply the style sheet to the extracted doc
       xmlDocPtr res = xsltApplyStylesheet(xslt, xmlTextReaderCurrentDoc(reader), params);

       xmlNodePtr resroot = xmlDocGetRootElement(res);

       if (resroot) {

	 if (!found && !isoption(options, OPTION_XSLT_ALL)) {
	   xmlOutputBufferWrite(buf, 3, ">\n\n");
	   found = true;
	 }

	 // if nested unit, top unit is not needed in result
	 xmlNodePtr resout = resroot;
         if (!isoption(options, OPTION_XSLT_ALL) &&
	     (strcmp("unit", (const char*) resout->name) == 0) &&
	     resout->children != 0 &&
	     (strcmp("unit", (const char*) resout->children->name) == 0))
	   resout = resout->children;

	 // output the result of the stylesheet
	 //	 xmlNsPtr savens = resout->nsDef;
	 //	 resout->nsDef = 0;
	 xmlNodeDumpOutput(buf, res, resout, 0, 0, 0);
	 //	 resout->nsDef = savens;

	 // put some space between this unit and the next one
	 if (!isoption(options, OPTION_XSLT_ALL))
	   xmlOutputBufferWrite(buf, 2, "\n\n");
       }

       // finished with the result of the transformation
       xmlFreeDoc(res);

       // move over this expanded node
       xmlTextReaderNext(reader);
     }

    // only need to do this once if applied to all
    if (isoption(options, OPTION_XSLT_ALL)) {
	break;
    }
  }

  // root unit end tag
  if (!isoption(options, OPTION_XSLT_ALL)) {
    if (found)
      xmlOutputBufferWrite(buf, 7, "</unit>");
    else
      xmlOutputBufferWrite(buf, 2, "/>");
  }
  xmlOutputBufferWrite(buf, 1, "\n");

  // all done with the buffer
  xmlOutputBufferClose(buf);

  return 0;
}
