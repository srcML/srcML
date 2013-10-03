/**
 * @file srcMLSAX2Reader.cpp
 * @copyright
 *
 * Copyright (C) 2013  SDML (www.srcML.org)
 *
 * The srcML Toolkit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML Toolkit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML Toolkit; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srcMLSAX2Reader.hpp>

#include <../srcmlns.hpp>
#include <srcml.h>

#include <stdlib.h>
#include <string.h>

void * start_routine(void *) {

}

/**
 * srcMLSAX2Reader
 * @param filename name of a file
 *
 * Construct a srcMLSAX2Reader using a filename
 */
srcMLSAX2Reader::srcMLSAX2Reader(const char * filename) 
  : control(filename) {

  pthread_create(&thread, 0, start_routine, &handler);
  handler.wait();

}

/**
 * srcMLSAX2Reader
 * @param input parser input buffer
 *
 * Construct a srcMLSAX2Reader using a parser input buffer
 */
srcMLSAX2Reader::srcMLSAX2Reader(xmlParserInputBufferPtr input) {

}

/**
 * ~srcMLSAX2Reader
 *
 * Destructor a srcMLSAX2Reader
 */
srcMLSAX2Reader::~srcMLSAX2Reader() {

}

/**
 * readRootUnitAttributes
 * @param language a location to store the language attribute
 * @param filename a location to store the filename attribute
 * @param directory a location to store the directory attribute
 * @param version a location to store the version attribute
 * @param attributes array to store other attributes gathered
 * @param prefixes an array to store gathered XML namespace prefixes
 * @param namespaces an array to store gathered XML naamespaces
 * @param options a variable to set used options
 * @param tabstop a variable to set the tabstop
 *
 * Read attributes and namespace information fromt the root unit,
 * setting the necessary options.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcMLSAX2Reader::readRootUnitAttributes(std::string ** language, std::string ** filename,
                                        std::string ** directory, std::string ** version,
                                        std::vector<std::string> & attributes,
                                        std::vector<std::string> & prefixes,
                                        std::vector<std::string> & namespaces,
                                        OPTION_TYPE & options,
                                        int & tabstop) {

  if(language == 0 || filename == 0 || directory == 0 || version == 0) return 0;

  *language = new root_language;

  return 1;
}

/**
 * readUnitAttributes
 * @param language a location to store the language attribute
 * @param filename a location to store the filename attribute
 * @param directory a location to store the directory attribute
 * @param version a location to store the version attribute
 *
 * Read attributes from next unit.
 *
 * @returns 1 on success and 0 on failure.
 */
int srcMLSAX2Reader::readUnitAttributes(std::string ** language, std::string ** filename,
                                    std::string ** directory, std::string ** version) {

  if(language == 0 || filename == 0 || directory == 0 || version == 0) return 0;

  return 1;

}

/**
 * readsrcML
 * @param writer an XML text writer
 *
 * Read the next unit of a srcML Archive.
 * and write it to the writer.
 *
 * @returns Return 0 when finished and 1 otherwize.
 */
int srcMLSAX2Reader::readsrcML(xmlTextWriterPtr writer) {

  if(done) return 0;

  return 1;

}

/**
 * readsrcML
 * 
 * Read the next unit from a srcML Archive
 * and return it as a std::string. Uses
 * readsrcML(xmlTextWriterPtr writer).
 *
 * @returns string on success and finished return a 0.
 */
std::string * srcMLSAX2Reader::readsrcML() {

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
