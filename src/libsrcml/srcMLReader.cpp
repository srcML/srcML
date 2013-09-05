/*
  srcMLReader.hpp

  Copyright (C) 2013  SDML (www.sdml.info)

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

#include "srcMLReader.hpp"
#include "../srcmlns.hpp"

#include <stdlib.h>
#include <string.h>

void output_node_srcml(const xmlNode & node, xmlTextWriterPtr writer);
void output_node_source(const xmlNode & node, xmlOutputBufferPtr output_buffer);


// get a copy of the current node with attributes set.
xmlNodePtr getNode(xmlTextReaderPtr reader) {

  xmlNodePtr node = xmlCopyNode(xmlTextReaderCurrentNode(reader), 2);
  node->extra = xmlTextReaderIsEmptyElement(reader);
  node->type = (xmlElementType)xmlTextReaderNodeType(reader);

  return node;
}

// free the current node.  Text is not allocated and do not free
void freeNode(xmlNodePtr node) {

  if(node && (xmlReaderTypes)node->type != XML_READER_TYPE_TEXT
     && (xmlReaderTypes)node->type != XML_READER_TYPE_SIGNIFICANT_WHITESPACE)
    xmlFreeNode(node);
}

srcMLReader::srcMLReader(const char * filename)
  : is_archive(false), done(false) {

  reader = xmlNewTextReaderFilename(filename);
  if(reader == NULL) throw std::string();
    xmlTextReaderRead(reader);
  node = getNode(reader);
}

srcMLReader::srcMLReader(xmlParserInputBufferPtr input)
  : is_archive(false), done(false) {

  if(input == NULL) throw std::string();
  reader = xmlNewTextReader(input, 0);
  if(reader == NULL) throw std::string();
  xmlTextReaderRead(reader);
  node = getNode(reader);
}

srcMLReader::~srcMLReader() {

  xmlFreeTextReader(reader);
  reader = 0;

}

int srcMLReader::readUnitAttributesInternal(std::string ** language, std::string ** filename,
                                            std::string ** directory, std::string ** version) {

  xmlAttrPtr attribute = node->properties;
  while (attribute) {

    std::string name = (const char *)attribute->name;
    try {

      if(name == "language")
        (*language) = new std::string((const char *)attribute->children->content);
      else if(name == "filename")
        (*filename) = new std::string((const char *)attribute->children->content);
      else if(name == "directory")
        (*directory) = new std::string((const char *)attribute->children->content);
      else if(name == "version")
        (*version) = new std::string((const char *)attribute->children->content);

    } catch(...) {

      if(*language) delete *language, (*language) = 0;
      if(*filename) delete *filename, (*filename) = 0;
      if(*directory) delete *directory, (*directory) = 0;
      if(*version) delete *version, (*version) = 0;
      return 1;

    }

    attribute = attribute->next;
  }

  return 0;

}

int srcMLReader::readRootUnitAttributes(std::string ** language, std::string ** filename,
                                        std::string ** directory, std::string ** version,
                                        std::vector<std::string> & attributes,
                                        std::vector<std::string> & prefixes,
                                        std::vector<std::string> & namespaces,
                                        OPTION_TYPE & options,
                                        int & tabstop) {

  if(done) return 0;

  // forward to start unit
  while(true) {
    if(node && (xmlReaderTypes)node->type == XML_READER_TYPE_ELEMENT && strcmp((const char *)node->name, "unit") == 0)
      break;

    if(xmlTextReaderRead(reader) != 1) { done = true; return 0; }
    freeNode(node);
    node = getNode(reader);
  }

  xmlAttrPtr attribute = node->properties;
  while (attribute) {
    std::string name = (const char *)attribute->name;

    try {

      if(name == "language")
        (*language) = new std::string((const char *)attribute->children->content);
      else if(name == "filename")
        (*filename) = new std::string((const char *)attribute->children->content);
      else if(name == "directory")
        (*directory) = new std::string((const char *)attribute->children->content);
      else if(name == "version")
        (*version) = new std::string((const char *)attribute->children->content);
      else if(name == "tabs")
        tabstop = atoi((const char *)attribute->children->content);
      else {

        attributes.push_back(name);
        attributes.push_back((const char *)attribute->children->content);

      }

    } catch(...) {

      if(*language) delete *language, (*language) = 0;
      if(*filename) delete *filename, (*filename) = 0;
      if(*directory) delete *directory, (*directory) = 0;
      if(*version) delete *version, (*version) = 0;
      return 0;

    }

    attribute = attribute->next;
  }

  xmlNsPtr xmlns = node->nsDef;

  while(xmlns) {

    std::string prefix = xmlns->prefix ? (const char *)xmlns->prefix : "";
    std::string ns = xmlns->href ? (const char *)xmlns->href : "";

    if(ns == SRCML_CPP_NS_URI)
      options |= OPTION_CPP;
    else if(ns == SRCML_ERR_NS_URI)
      options |= OPTION_DEBUG;
    else if(ns == SRCML_EXT_LITERAL_NS_URI)
      options |= OPTION_LITERAL;
    else if(ns == SRCML_EXT_OPERATOR_NS_URI)
      options |= OPTION_OPERATOR;
    else if(ns == SRCML_EXT_MODIFIER_NS_URI)
      options |= OPTION_MODIFIER;
    else if(ns == SRCML_EXT_POSITION_NS_URI)
      options |= OPTION_POSITION;

    int i;
    try {

      for(i = 0; i < prefixes.size(); ++i)

        if(namespaces.at(i) == ns) {

          prefixes.at(i) = prefix;
          break;
        }

    } catch(...) {}

    if(i == prefixes.size()) {
      prefixes.push_back(prefix);
      namespaces.push_back(ns);
    }

    xmlns = xmlns->next;
  }

  return 1;
}

int srcMLReader::readUnitAttributes(std::string ** language, std::string ** filename,
                                    std::string ** directory, std::string ** version) {

  bool read_unit_start = false;

  if(!save_nodes.empty()) {

    try {

      for(int i = 0; i < save_nodes.size(); ++i)
        freeNode(save_nodes.at(i));

    } catch(...) {}

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

  if(readUnitAttributesInternal(language, filename, directory, version)) return 0;
  if(xmlTextReaderRead(reader) != 1) { freeNode(node); done = true; return 0; }

  save_nodes.push_back(node);
  node = getNode(reader);

  if(is_archive) return 1;

  while(true) {

    save_nodes.push_back(node);

    if(node->type == (xmlElementType)XML_READER_TYPE_ELEMENT && strcmp((const char *)node->name, "unit") == 0) {

      is_archive = true;
      try {
        for(int i = 0; i < save_nodes.size() - 1; ++i)
          freeNode(save_nodes.at(i));

      } catch(...) {}
      save_nodes.clear();
      save_nodes.push_back(node);
      if(*language) delete *language, (*language) = 0;
      if(*filename) delete *filename, (*filename) = 0;
      if(*directory) delete *directory, (*directory) = 0;
      if(*version) delete *version, (*version) = 0;
      if(readUnitAttributesInternal(language, filename, directory, version)) return 0;
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

    try {

      for(int i = 0; i < save_nodes.size(); ++i)
        output_node_srcml(*save_nodes.at(i), writer);

      for(int i = 0; i < save_nodes.size(); ++i)
        freeNode(save_nodes.at(i));

    } catch(...) {}

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
          try {

            for(int i = 0; i < save_nodes.size() - 1; ++i)
              freeNode(save_nodes.at(i));

          } catch(...) {}

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

      try {

        for(int i = 0; i < save_nodes.size(); ++i)
          output_node_srcml(*save_nodes.at(i), writer);

        for(int i = 0; i < save_nodes.size() - 1; ++i)
          freeNode(save_nodes.at(i));

      } catch(...) {}

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

    try {

      for(int i = 0; i < save_nodes.size(); ++i)
        output_node_source(*save_nodes.at(i), output_buffer);

      for(int i = 0; i < save_nodes.size(); ++i)
        freeNode(save_nodes.at(i));

    } catch(...) {}

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
          try {

            for(int i = 0; i < save_nodes.size() - 1; ++i)
              freeNode(save_nodes.at(i));

          } catch(...) {}
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
      try {

        for(int i = 0; i < save_nodes.size(); ++i)
          output_node_source(*save_nodes.at(i), output_buffer);

        for(int i = 0; i < save_nodes.size() - 1; ++i)
          freeNode(save_nodes.at(i));

      } catch(...) {}

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

  int length = buffer->use;

  while(length > 0 && buffer->content[length - 1] == '\n')
    --length;

  std::string * unit = 0;
  try {

    unit = new std::string((const char *)buffer->content, length);

  } catch(...) {}

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

  std::string * unit = 0;

  try {
    
    unit = new std::string((const char *)buffer->content);

  } catch(...) {}

  xmlOutputBufferClose(output_buffer);
  xmlBufferFree(buffer);
  return unit;

}

// output node as srcML
void output_node_srcml(const xmlNode & node, xmlTextWriterPtr writer) {

  bool isemptyelement = false;

  switch (node.type) {
  case XML_READER_TYPE_ELEMENT:

    // record if this is an empty element since it will be erased by the attribute copying
    isemptyelement = node.extra & 0x1;

    // start the element
    {

      std::string s = "";
      if(node.ns && node.ns->prefix) {

        s += ((char*) node.ns->prefix);
        s += ":";
      }
      s += (char*) node.name;

      xmlTextWriterStartElement(writer, BAD_CAST s.c_str());

    }

    if(strcmp((const char *)node.name, "unit") == 0) {

      xmlNsPtr xmlns = node.nsDef;
      while(xmlns) {

        std::string ns = xmlns->href ? (const char *)xmlns->href : "";
        if(ns != SRCML_CPP_NS_URI) {

          xmlns = xmlns->next;
          continue;

        }

        std::string prefix = "xmlns";
        if(xmlns->prefix) {

          prefix += ":";
          prefix += (const char *)xmlns->prefix;

        }

        xmlTextWriterWriteAttribute(writer, (const xmlChar *)prefix.c_str(), (const xmlChar *)ns.c_str());

        xmlns = xmlns->next;
      }
    }

    // copy all the attributes
    {
      xmlAttrPtr attribute = node.properties;
      while (attribute) {

        std::string s;
        if(attribute->ns && attribute->ns->prefix) {
          s = (const char *)attribute->ns->prefix;
          s += ":";

        }
        s += (const char *)attribute->name;

        xmlTextWriterWriteAttribute(writer, (const xmlChar *)s.c_str(), (const xmlChar *)attribute->children->content);
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


// output node as source code.  Ignore tags and other xml.
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

