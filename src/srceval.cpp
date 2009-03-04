#include "srceval.h"

void xmlUnitDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNodePtr node) {

  xmlOutputBufferWrite(buf, 5, "<unit");

  // register the namespaces on the root element
  for (xmlNsPtr pAttr =  node->nsDef; pAttr; pAttr = pAttr->next)
    xmlNodeDumpOutput(buf, node->doc, (xmlNodePtr) pAttr, 0, 0, 0);

  // copy all attributes
  for (xmlAttrPtr pAttr = node->properties; pAttr; pAttr = pAttr->next)
    xmlNodeDumpOutput(buf, node->doc, (xmlNodePtr) pAttr, 0, 0, 0);

  //  xmlOutputBufferWrite(buf, 3, ">\n\n");
}
