#ifndef INCLUDED_SRCEVAL_H
#define INCLUDED_SRCEVAL_H

#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

void xmlAttrDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNodePtr node, xmlAttrPtr pAttr);

void xmlNsDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNsPtr pAttr);

void xmlUnitDumpOutputBuffer(xmlOutputBufferPtr buf, xmlNodePtr node);

#endif
