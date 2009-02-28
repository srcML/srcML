/*
  xpathproc

  A simple XPath expression evaluator.

  Usage:  xpathproc xpath-expression xml-file
*/

#include "srcxslteval.h"
#include "srceval.h"

#include <cstring>
#include <iostream>
#include <cstdio>

#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

int srcxslteval(const char* xpath, xmlTextReaderPtr reader, const char* ofilename, const char* params[],
		int paramcount) {

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

  // copy the start tag of the root element unit
  xmlUnitDumpOutputBuffer(buf, xmlTextReaderCurrentNode(reader));

  // doc for applying stylesheet to
  xmlDocPtr doc = xmlNewDoc(NULL);

  int position = 0;

  while (1) {

     // read a node
     int ret = xmlTextReaderRead(reader);
     if (ret != 1)
       break;

     // nested unit tag
     if (xmlTextReaderDepth(reader) == 1 &&
	 xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT &&
	 xmlTextReaderConstName(reader)[0] == 'u') {

       // mark the internal parameter with the position
       ++position;
       sprintf(posstr, "'%d'", position);

       // expand this unit to make it the context
       xmlNodePtr node = xmlTextReaderExpand(reader);

       // create a separate document with the expanded unit
       xmlDocSetRootElement(doc, xmlCopyNode(node, 1));

       // apply the style sheet to the extracted doc
       xmlDocPtr res = xsltApplyStylesheet(xslt, doc, params);

       // remove and store the namespace (if we can)
       xmlNodePtr resroot = xmlDocGetRootElement(res); 
       if (resroot) {

	 // get the namespaces out
	 xmlNsPtr savens = xmlDocGetRootElement(res)->nsDef;
	 xmlDocGetRootElement(res)->nsDef = 0;
       
	 // save the transformed tree
	 xsltSaveResultTo(buf, res, xslt);
	 xmlOutputBufferWriteString(buf, "\n");

	 // put the namespace back in
	 xmlDocGetRootElement(res)->nsDef = savens;
       }

       // finished with the result of the transformation
       xmlFreeDoc(res);

       // cleanup the extracted doc
       xmlNodePtr oldnode = xmlDocGetRootElement(doc);
       xmlUnlinkNode(oldnode);
       xmlFreeNode(oldnode);

       // move over this expanded node
       xmlTextReaderNext(reader);
     }
  }

  // root unit end tag
  xmlOutputBufferWriteString(buf, "</unit>\n");

  // all done with the buffer
  xmlOutputBufferClose(buf);

  // all done with the doc
  xmlFreeDoc(doc);

  return 0;
}
