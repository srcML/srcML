#include "srceval.h"

#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>

#include <libexslt/exslt.h>

void xmlAttrDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNodePtr node, xmlAttrPtr pAttr) {

  xmlOutputBufferWrite(buf, 1, " ");
  xmlOutputBufferWriteString(buf, (const char*) pAttr->name);
  xmlOutputBufferWrite(buf, 2, "=\"");
  xmlOutputBufferWriteString(buf, (const char*) xmlGetProp(node, pAttr->name));
  xmlOutputBufferWrite(buf, 1, "\"");
}

void xmlNsDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNsPtr pAttr) {

  xmlOutputBufferWrite(buf, 6, " xmlns");
  if (pAttr->prefix)
    xmlOutputBufferWrite(buf, 1, ":");
  xmlOutputBufferWriteString(buf, (const char*) pAttr->prefix);
  xmlOutputBufferWrite(buf, 2, "=\"");
  xmlOutputBufferWriteString(buf, (const char*) pAttr->href);
  xmlOutputBufferWrite(buf, 1, "\"");
}

void xmlUnitDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNodePtr node) {

  // register the namespaces on the root element
  xmlOutputBufferWrite(buf, 5, "<unit");
  for (xmlNsPtr pAttr =  node->nsDef; pAttr; pAttr = pAttr->next)
    xmlNsDumpOutputBuffer(buf, pAttr);

  // copy all attributes
  for (xmlAttrPtr pAttr = node->properties; pAttr; pAttr = pAttr->next)
    xmlAttrDumpOutputBuffer(buf, node, pAttr);
  xmlOutputBufferWrite(buf, 3, ">\n\n");
}
