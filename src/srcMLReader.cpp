#include "srcMLReader.hpp"

void output_node_srcml(const xmlNode & node, xmlTextWriterPtr writer);
void output_node_source(const xmlNode & node, xmlOutputBufferPtr output_buffer);

xmlNodePtr getNode(xmlTextReaderPtr reader) {

  xmlNodePtr node = xmlCopyNode(xmlTextReaderCurrentNode(reader), 2);
  node->extra = xmlTextReaderIsEmptyElement(reader);
  node->type = (xmlElementType)xmlTextReaderNodeType(reader);

  return node;
}

void freeNode(xmlNodePtr node) {

  if(node && (xmlReaderTypes)node->type != XML_READER_TYPE_TEXT
     && (xmlReaderTypes)node->type != XML_READER_TYPE_SIGNIFICANT_WHITESPACE)
    xmlFreeNode(node);
}

srcMLReader::srcMLReader(const char * filename)
  : is_archive(false), done(false) {

  reader = xmlNewTextReaderFilename(filename);
  xmlTextReaderRead(reader);
  node = getNode(reader);
}

srcMLReader::~srcMLReader() {

  xmlFreeTextReader(reader);
  reader = 0;

}

void srcMLReader::readUnitAttributesInternal(std::string ** language, std::string ** filename,
                                             std::string ** directory, std::string ** version) {

  xmlAttrPtr attribute = node->properties;
  while (attribute) {

    if(strcmp((const char *)attribute->name, "language") == 0)
      (*language) = new std::string((const char *)attribute->children->content);
    else if(strcmp((const char *)attribute->name, "filename") == 0)
      (*filename) = new std::string((const char *)attribute->children->content);
    else if(strcmp((const char *)attribute->name, "directory") == 0)
      (*directory) = new std::string((const char *)attribute->children->content);
    else if(strcmp((const char *)attribute->name, "version") == 0)
      (*version) = new std::string((const char *)attribute->children->content);

    attribute = attribute->next;
  }

}

int srcMLReader::readUnitAttributes(std::string ** language, std::string ** filename,
                                    std::string ** directory, std::string ** version) {

  bool read_unit_start = false;

  if(!save_nodes.empty()) {

  for(int i = 0; i < save_nodes.size(); ++i)
    freeNode(save_nodes.at(i));
  save_nodes.clear();

  }

  if(done) return 0;

  // forward to start unit
  while(true) {
    if(node && (xmlReaderTypes)node->type == XML_READER_TYPE_ELEMENT && strcmp((const char *)node->name, "unit") == 0)
      break;

    if(xmlTextReaderRead(reader) != 1) { done = true; return 0; }
    freeNode(node);
    node = getNode(reader);
  }

  readUnitAttributesInternal(language, filename, directory, version);
  if(xmlTextReaderRead(reader) != 1) { freeNode(node); done = true; return 0; }

  save_nodes.push_back(node);
  node = getNode(reader);

  if(is_archive) return 1;

  while(true) {

    save_nodes.push_back(node);

    if(node->type == (xmlElementType)XML_READER_TYPE_ELEMENT && strcmp((const char *)node->name, "unit") == 0) {

      is_archive = true;
      for(int i = 0; i < save_nodes.size() - 1; ++i)
        freeNode(save_nodes.at(i));
      save_nodes.clear();
      save_nodes.push_back(node);
      if(*language) delete *language, (*language) = 0;
      if(*filename) delete *filename, (*filename) = 0;
      if(*directory) delete *directory, (*directory) = 0;
      if(*version) delete *version, (*version) = 0;
      readUnitAttributesInternal(language, filename, directory, version);
      break;
    }

    if(node->type == (xmlElementType)XML_READER_TYPE_ELEMENT
       && strcmp((const char *)node->name, "unit") != 0) {
      is_archive = true;
      break;
    }

    if(xmlTextReaderRead(reader) != 1) {done = true; return 0; }
    node = getNode(reader);

  }

  readUnitAttributesInternal(language, filename, directory, version);
  if(xmlTextReaderRead(reader) != 1) { done = true; return 0; }
  node = getNode(reader);

  return 1;

}

int srcMLReader::readsrcML(xmlTextWriterPtr writer) {

  if(done) return 0;

  //xmlTextWriterStartDocument(writer, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);
  bool read_unit_start = false;

  if(!save_nodes.empty()) {

    for(int i = 0; i < save_nodes.size(); ++i)
      output_node_srcml(*save_nodes.at(i), writer);

    for(int i = 0; i < save_nodes.size(); ++i)
      freeNode(save_nodes.at(i));

    save_nodes.clear();

  } else {

    // forward to start unit
    while(true) {
      if(node && (xmlReaderTypes)node->type == XML_READER_TYPE_ELEMENT && strcmp((const char *)node->name, "unit") == 0)
        break;

      if(xmlTextReaderRead(reader) != 1) {done = true; return 0; }
      freeNode(node);
      node = getNode(reader);
    }

  }

  while(true) {

    if(is_archive) output_node_srcml(*node, writer);
    else save_nodes.push_back(node);

    if(strcmp((const char *)node->name, "unit") == 0) {

      if(node->type == (xmlElementType)XML_READER_TYPE_ELEMENT) {

        if(read_unit_start) {

          is_archive = true;
          for(int i = 0; i < save_nodes.size() - 1; ++i)
            freeNode(save_nodes.at(i));
          save_nodes.clear();
          output_node_srcml(*node, writer);

        }

        read_unit_start = true;
      }

      if(node->type == (xmlElementType)XML_READER_TYPE_END_ELEMENT) {
        break;
      }

    }

    if(!save_nodes.empty() && node->type == (xmlElementType)XML_READER_TYPE_ELEMENT
       && strcmp((const char *)node->name, "unit") != 0) {

      is_archive = true;

      for(int i = 0; i < save_nodes.size(); ++i)
        output_node_srcml(*save_nodes.at(i), writer);

      for(int i = 0; i < save_nodes.size() - 1; ++i)
        freeNode(save_nodes.at(i));

      save_nodes.clear();

    }

    if(is_archive) freeNode(node);
    if(xmlTextReaderRead(reader) != 1) {done = true; return 0; }
    node = getNode(reader);

  }

  if(is_archive) freeNode(node);
  node = 0;

  xmlTextWriterEndDocument(writer);

  return 1;

}


int srcMLReader::read(xmlOutputBufferPtr output_buffer) {

  if(done) return 0;

  //xmlTextWriterStartDocument(output_buffer, XML_VERSION, xml_encoding, XML_DECLARATION_STANDALONE);
  bool read_unit_start = false;

  if(!save_nodes.empty()) {

    for(int i = 0; i < save_nodes.size(); ++i)
      output_node_source(*save_nodes.at(i), output_buffer);

    for(int i = 0; i < save_nodes.size(); ++i)
      freeNode(save_nodes.at(i));

    save_nodes.clear();

  } else {

    // forward to start unit
    while(true) {
      if(node && (xmlReaderTypes)node->type == XML_READER_TYPE_ELEMENT && strcmp((const char *)node->name, "unit") == 0)
        break;

      if(xmlTextReaderRead(reader) != 1) {done = true; return 0; }
      freeNode(node);
      node = getNode(reader);
    }

  }

  while(true) {

    if(is_archive) output_node_source(*node, output_buffer);
    else save_nodes.push_back(node);

    if(strcmp((const char *)node->name, "unit") == 0) {

      if(node->type == (xmlElementType)XML_READER_TYPE_ELEMENT) {

        if(read_unit_start) {

          is_archive = true;
          for(int i = 0; i < save_nodes.size() - 1; ++i)
            freeNode(save_nodes.at(i));
          save_nodes.clear();
          output_node_source(*node, output_buffer);

        }

        read_unit_start = true;
      }

      if(node->type == (xmlElementType)XML_READER_TYPE_END_ELEMENT) {
        break;
      }

    }

    if(!save_nodes.empty() && node->type == (xmlElementType)XML_READER_TYPE_ELEMENT
       && strcmp((const char *)node->name, "unit") != 0) {

      is_archive = true;

      for(int i = 0; i < save_nodes.size(); ++i)
        output_node_source(*save_nodes.at(i), output_buffer);

      for(int i = 0; i < save_nodes.size() - 1; ++i)
        freeNode(save_nodes.at(i));

      save_nodes.clear();

    }

    if(is_archive) freeNode(node);
    if(xmlTextReaderRead(reader) != 1) {done = true; return 0; }
    node = getNode(reader);

  }

  if(is_archive) freeNode(node);
  node = 0;

  return 1;

}

std::string * srcMLReader::readsrcML() {

  if(done) return 0;

  xmlBufferPtr buffer = xmlBufferCreate();
  xmlTextWriterPtr writer = xmlNewTextWriterMemory(buffer, 0);
  int status = readsrcML(writer);

  if(!status) return 0;

  std::string * unit = new std::string((const char *)buffer->content);
  xmlFreeTextWriter(writer);
  xmlBufferFree(buffer);

  return unit;

}

std::string * srcMLReader::read() {

  if(done) return 0;

  xmlBufferPtr buffer = xmlBufferCreate();
  xmlOutputBufferPtr output_buffer = xmlOutputBufferCreateBuffer(buffer, xmlFindCharEncodingHandler("UTF-8"));
  int status = read(output_buffer);

  if(!status) return 0;

  std::string * unit = new std::string((const char *)buffer->content);
  xmlOutputBufferClose(output_buffer);
  xmlBufferFree(buffer);
  return unit;

}

// output current XML node in reader
void output_node_srcml(const xmlNode & node, xmlTextWriterPtr writer) {

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


// output current XML node in reader
void output_node_source(const xmlNode & node, xmlOutputBufferPtr output_buffer) {

  bool isemptyelement = false;
  switch (node.type) {

  case XML_READER_TYPE_TEXT:
  case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:

    // output the UTF-8 buffer escaping the characters.  Note that the output encoding
    // is handled by libxml
    for (unsigned char* p = (unsigned char*) node.content; *p != 0; ++p) {
      if (*p == '&')
        xmlOutputBufferWrite(output_buffer, 5, "&amp;");
      else if (*p == '<')
        xmlOutputBufferWrite(output_buffer, 4, "&lt;");
      else if (*p == '>')
        xmlOutputBufferWrite(output_buffer, 4, "&gt;");
      else
        xmlOutputBufferWrite(output_buffer, 1, (const char *)p);
    }
    break;

  default:
    break;
  }
}

