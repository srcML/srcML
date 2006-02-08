/*
  xmlwriter.cpp

  Copyright (C) 2005  SDML (www.sdml.info)

  This file is part of the srcML translator.

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

  A standalone xml writing package based on the interface that the libxml
  xmlTextWriter provides.  Designed to mimic the interface of xmlTextWriter so
  that libxml is not a required part of the package.

  This is not a replacement for xmlTextWriter.  Only the interface used is implemented.
  Also, many of the encoding features are not provided.
*/

#include "xmlwriter.h"
#include <iostream>
#include <fstream>

namespace {
void close_start_tag(xmlTextWriterPtr writer) {

  // close any current start tag
  if (writer->open_start) {
    // close the start tag
    *(writer->pout) << '>';

    // closed previous start tag
    writer->open_start = false;
  }
}
};

// starts document with xml declaration
void xmlTextWriterStartDocument(xmlTextWriterPtr writer, const char* xml_version, const char* xml_encoding, const char* xml_declaration_standalone) {
  *(writer->pout) << "<?xml version=\"" << xml_version << "\" encoding=\"" << xml_encoding << "\" standalone=\""
		  << xml_declaration_standalone << "\"?>" << '\n';

  writer->open_start = false;
}

// end document closing any open elements
void xmlTextWriterEndDocument(xmlTextWriterPtr writer) {

  // close any open tags
  close_start_tag(writer);

  // end any open elements
  if (!writer->elements.empty())
   xmlTextWriterEndElement(writer);

  if (writer->pout != &std::cout)
    delete writer->pout;
}

void xmlTextWriterStartElement(xmlTextWriterPtr writer, const char* element_name) {

  // close any current start tag
  close_start_tag(writer);

  // open the start tag
  *(writer->pout) << '<' << element_name;

  // record that this element is open
  writer->elements.push(element_name);

  // still open start tag
  writer->open_start = true;
}

void xmlTextWriterEndElement(xmlTextWriterPtr writer) {

  // an open current start tag indicates an empty element
  if (writer->open_start) {

    // close the start tag and end the element
    *(writer->pout) << "/>";

  } else {

    // end the open element
    *(writer->pout) << "</" << writer->elements.top() << ">";

  }

  // element is done
  writer->elements.pop();

  // no open start tags
  writer->open_start = false;

  // output a newline after the last element
  if (writer->elements.empty())
    *(writer->pout) << "\n";
}

void xmlTextWriterWriteAttribute(xmlTextWriterPtr writer, const char* attribute_name, const char* attribute_value) {
  *(writer->pout) << ' ' << attribute_name << "=\"" << attribute_value << "\"";
}

int xmlTextWriterWriteComment(xmlTextWriterPtr writer, const char* s) {

  // close any current start tag
  close_start_tag(writer);

  *(writer->pout) << "<!--" << s << "-->";

  return 1;
}

void xmlTextWriterWriteRawLen(xmlTextWriterPtr writer, unsigned char* data, int size) {

  // close any current start tag
  close_start_tag(writer);

  // output the date directly
  writer->pout->write((char*) data, size);
}

void xmlTextWriterFlush(xmlTextWriterPtr writer) {

  (writer->pout)->flush();
}

// free the writer allocated in xmlNewTextWriterFilename
void xmlFreeTextWriter(xmlTextWriterPtr writer) { 
  delete writer;
}

// allocate a new text writer based on the filename
// @param compression is ignored
xmlTextWriterPtr xmlNewTextWriterFilename(const char* filename, int compression) {
  xmlTextWriterPtr writer = new xmlTextWriter;

  if (filename[0] == '-' && filename[1] == '\0')
    writer->pout = &std::cout;
  else
    writer->pout = new std::ofstream(filename);

  // no open elements
  writer->open_start = false;

  return writer; 
}

xmlCharEncodingHandlerPtr xmlFindCharEncodingHandler(const char*) { return new xmlCharEncodingHandler; }

// nop since indent is not used
void xmlTextWriterSetIndent(xmlTextWriterPtr, int) {}
/*
int xmlCharEncodingHandler::input(unsigned char out[], int* poutsize, const unsigned char* in , int* pinsize) { 

  for (int i = 0; i < *pinsize; ++i)
    out[i] = in[i];

  *poutsize = *pinsize;

  return *poutsize; 
}

int xmlCharEncodingHandler::output(unsigned char out[], int* poutsize, const unsigned char* in , int* pinsize) { 

  for (int i = 0; i < *pinsize; ++i)
    out[i] = in[i];

  *poutsize = *pinsize;

  return *poutsize; 
}
*/
