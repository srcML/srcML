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

void applyxslt(xmlOutputBufferPtr buf, xmlTextReaderPtr reader, xmlDocPtr doc, xsltStylesheetPtr xslt, const char* params[], int options, bool& found) {

       // copy the current tree to a new doc
       xmlDocSetRootElement(doc, xmlCopyNode(xmlTextReaderCurrentNode(reader), 1));

       // apply the style sheet to the extracted doc
       xmlDocPtr res = xsltApplyStylesheet(xslt, doc, params);

       // process result of stylesheet transformation
       xmlNodePtr resroot = xmlDocGetRootElement(res);

       if (res) {

	 // if in per-unit mode and this is the first result found
	 if (!found && !isoption(options, OPTION_XSLT_ALL)) {
	   xmlOutputBufferWrite(buf, 3, ">\n\n");
	   found = true;
	 }

	 // output the result of the stylesheet
	 xmlNsPtr savens = resroot ? resroot->nsDef : 0;
	 if (savens && !isoption(options, OPTION_XSLT_ALL))
	   resroot->nsDef = 0;
	 xsltSaveResultTo(buf, res, xslt);
	 if (savens && !isoption(options, OPTION_XSLT_ALL))
	   resroot->nsDef = savens;

	 // put some space between this unit and the next one
	 if (!isoption(options, OPTION_XSLT_ALL))
	   xmlOutputBufferWrite(buf, 1, "\n");
       }

       // finished with the result of the transformation
       xmlFreeDoc(res);

       xmlNodePtr oldnode = xmlDocGetRootElement(doc);
       xmlUnlinkNode(oldnode);
       xmlFreeNode(oldnode);
}

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
  char* unit_filename = (char*) xmlTextReaderGetAttribute(reader, BAD_CAST "filename");
  if (unit_filename) {
    params[paramcount++] = "src:filename";
    strcpy(filename + 1, unit_filename);
    strcat(filename, "'");
    params[paramcount++] = filename;

    xmlFree(unit_filename);
  }

  // directory
  char directory[MAX_ATTR_SIZE] = "'";
  char* unit_directory = (char*) xmlTextReaderGetAttribute(reader, BAD_CAST "dir");
  if (unit_directory) {
    params[paramcount++] = "src:directory";
    strcpy(directory + 1, unit_directory);
    strcat(directory, "'");
    params[paramcount++] = directory;

    xmlFree(unit_directory);
  }

  // language
  char language[MAX_ATTR_SIZE] = "'";
  char* unit_language = (char*) xmlTextReaderGetAttribute(reader, BAD_CAST "language");
  if (unit_language) {
    params[paramcount++] = "src:language";
    strcpy(language + 1, unit_language);
    strcat(language, "'");
    params[paramcount++] = language;

    xmlFree(unit_language);
  }


  // version
  char version[MAX_ATTR_SIZE] = "'";
  char* unit_version = (char*) xmlTextReaderGetAttribute(reader, BAD_CAST "version");
  if (unit_version) {
    params[paramcount++] = "src:version";
    strcpy(version + 1, unit_version);
    strcat(version, "'");
    params[paramcount++] = version;

    xmlFree(unit_version);
  }

  params[paramcount] = NULL;

  // copy the start tag of the root element unit for per-unit processing
  if (!isoption(options, OPTION_XSLT_ALL))
    xmlUnitDumpOutputBuffer(buf, xmlTextReaderCurrentNode(reader));

  // keep track of current position
  int position = 0;

  // keep track if we actually get any results
  bool found = false;

  // document to apply transform to
  xmlDocPtr doc = xmlNewDoc(NULL);

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

       // save the next pointer
       xmlNodePtr next = xmlTextReaderCurrentNode(reader)->next;
       xmlTextReaderCurrentNode(reader)->next = 0;

       // apply the xslt
       applyxslt(buf, reader, doc, xslt, params, options, found);

       xmlTextReaderCurrentNode(reader)->next = next;

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

    xmlOutputBufferWrite(buf, 1, "\n");
  }

  // all done with the buffer
  xmlOutputBufferClose(buf);

  xmlFreeDoc(doc);

  return 0;
}
