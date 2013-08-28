#include "srcMLReader.hpp"

#include <libxml/xmlwriter.h>

#include <vector>

void output_node(const xmlNode & node, xmlTextWriterPtr writer);

srcMLReader::srcMLReader(const char * filename)
  : read_root(false){

  reader = xmlNewTextReaderFilename(filename);
  xmlTextReaderRead(reader);
  node = xmlTextReaderCurrentNode(reader);
}

srcMLReader::~srcMLReader() {

  xmlFreeTextReader(reader);
  reader = 0;

}

std::string * srcMLReader::read() {

  xmlBufferPtr buffer = xmlBufferCreate();
  xmlTextWriterPtr writer = xmlNewTextWriterMemory(buffer, 0);
  //xmlTextWriterStartDocument(writer, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);
  bool read_unit_start = false;

  // forward to start unit
  while(true) {
    if(node && (xmlReaderTypes)node->type == XML_READER_TYPE_ELEMENT && strcmp((const char *)node->name, "unit") == 0)
      break;
    node = xmlTextReaderCurrentNode(reader);
    node->extra = xmlTextReaderIsEmptyElement(reader);
  }

  std::vector<xmlNodePtr> save_nodes;

  while(true) {

    if(read_root)
      output_node(*node, writer);
    else
      save_nodes.push_back(node);

    if(strcmp((const char *)node->name, "unit") == 0) {

      if((xmlReaderTypes)node->type == XML_READER_TYPE_ELEMENT) {

        if(read_unit_start)
          read_root = true;

        read_unit_start = true;
      }

      if((xmlReaderTypes)node->type == XML_READER_TYPE_END_ELEMENT)
        break;


    } else if(save_nodes(xmlReaderTypes)node->type == XML_READER_TYPE_ELEMENT) {

    }

    xmlFreeNode(node);
    node = 0;

    node = xmlTextReaderCurrentNode(reader);
    node->extra = xmlTextReaderIsEmptyElement(reader);

  }

  xmlFreeNode(node);
  node = 0;

  xmlTextWriterEndDocument(writer);
  xmlFreeTextWriter(writer);

  std::string * unit = new std::string((const char *)buffer->content);
  xmlBufferFree(buffer);
  return unit;

}

// output current XML node in reader
void output_node(const xmlNode & node, xmlTextWriterPtr writer) {

  bool isemptyelement = false;

  switch (node.type) {
  case XML_READER_TYPE_ELEMENT:

    // record if this is an empty element since it will be erased by the attribute copying
    isemptyelement = node.extra & 0x1;

    // start the element
    if (node.ns && node.ns->prefix) {
      std::string s = ((char*) node.ns->prefix);
      s += ":";
      s += (char*) node.name;

      xmlTextWriterStartElement(writer, BAD_CAST s.c_str());

    } else
      xmlTextWriterStartElement(writer, (xmlChar *)node.name);

    // copy all the attributes
    {
      xmlAttrPtr attribute = node.properties;
      while (attribute) {

        xmlTextWriterWriteAttribute(writer, (const xmlChar *)attribute->name, (const xmlChar *)attribute->children->content);
        attribute = attribute->next;
      }
    }

    // end now if this is an empty element
    if (isemptyelement) {

      xmlTextWriterEndElement(writer);
    }

    break;

  case XML_READER_TYPE_END_ELEMENT:
    xmlTextWriterEndElement(writer);
    break;

  case XML_READER_TYPE_COMMENT:
    xmlTextWriterWriteComment(writer, (const xmlChar *)node.content);
    break;

  case XML_READER_TYPE_TEXT:
  case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:

    // output the UTF-8 buffer escaping the characters.  Note that the output encoding
    // is handled by libxml
    for (unsigned char* p = (unsigned char*) node.content; *p != 0; ++p) {
      if (*p == '&')
        xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&amp;", 5);
      else if (*p == '<')
        xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&lt;", 4);
      else if (*p == '>')
        xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) "&gt;", 4);
      else
        xmlTextWriterWriteRawLen(writer, BAD_CAST (unsigned char*) p, 1);
    }
    break;

  default:
    break;
  }
}

