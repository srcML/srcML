#include "srcMLReader.hpp"

#include <libxml/xmlwriter.h>

void outputXML(xmlTextReaderPtr reader, xmlTextWriterPtr writer);

srcMLReader::srcMLReader(const char * filename)
  : read_root(false){

  reader = xmlNewTextReaderFilename(filename);

}

srcMLReader::~srcMLReader() {

  xmlFreeTextReader(reader);
  reader = 0;

}

std::string * srcMLReader::read() {
  
  xmlBufferPtr buffer = xmlBufferCreate();
  xmlTextWriterPtr writer = xmlNewTextWriterMemory(buffer, 0);
  //xmlTextWriterStartDocument(writer, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);

  while(xmlTextReaderRead(reader)) {

    outputXML(reader, writer);

    if(strcmp((const char *)xmlTextReaderLocalName(reader), "unit") == 0) {

      if(xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
        ;

      if(xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT)
        break;

    }

    xmlTextWriterEndDocument(writer);
    xmlFreeTextWriter(writer);

  }

}

// output current XML node in reader
void outputXML(xmlTextReaderPtr reader, xmlTextWriterPtr writer) {

  bool isemptyelement = false;

  switch (xmlTextReaderNodeType(reader)) {
  case XML_READER_TYPE_ELEMENT:

    // record if this is an empty element since it will be erased by the attribute copying
    isemptyelement = xmlTextReaderIsEmptyElement(reader) > 0;

    // start the element
    xmlTextWriterStartElement(writer, xmlTextReaderLocalName(reader));

    // copy all the attributes
    while (xmlTextReaderMoveToNextAttribute(reader)) {
      xmlTextWriterWriteAttribute(writer, xmlTextReaderConstName(reader), xmlTextReaderConstValue(reader));
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
    xmlTextWriterWriteComment(writer, xmlTextReaderConstValue(reader));
    break;

  case XML_READER_TYPE_TEXT:
  case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:

    // output the UTF-8 buffer escaping the characters.  Note that the output encoding
    // is handled by libxml
    for (unsigned char* p = (unsigned char*) xmlTextReaderConstValue(reader); *p != 0; ++p) {
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
