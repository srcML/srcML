/**
 * @file srcMLSAX2Reader.hpp
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

#include <Options.hpp>

#include <libxml/xmlreader.h>
#include <libxml/xmlwriter.h>

#include <string>
#include <vector>

#ifndef INCLUDED_SRCML_SAX2_READER_HPP
#define INCLUDED_SRCML_SAX2_READER_HPP

/**
 * srcMLSAX2Reader
 *
 * Extend XML Text Reader interface to
 * progressively read a srcML Archive collecting
 * units and reading unit attributes.
 */
class srcMLSAX2Reader {

private :

  srcMLControlHandler control;
  srcMLReaderHandler handler;

public :

  // constructors
  srcMLSAX2Reader(const char * filename);
  srcMLSAX2Reader(xmlParserInputBufferPtr input);

  // destructors
  ~srcMLSAX2Reader();

  // read attribute and namespace information from root unit.  Does not advance read.
  int readRootUnitAttributes(std::string ** language, std::string ** filename,
                             std::string ** directory, std::string ** version,
                             std::vector<std::string> & attributes,
                             std::vector<std::string> & prefixes,
                             std::vector<std::string> & namespaces,
                             OPTION_TYPE & options,
                             int & tabstop);

  // helper function for processing unit attributes.  Does not advance read.
  int readUnitAttributesInternal(std::string ** language, std::string ** filename,
                                  std::string ** directory, std::string ** version);

  /* finds next unit tag if not current unit and sets attributes.  Consumes unit.
     Unit is still avaible for readsrcML or read.  But not readUnitAttributes.
   */
  int readUnitAttributes(std::string ** language, std::string ** filename,
                         std::string ** directory, std::string ** version);

  // read the next unit add write it to writer as srcML
  int readsrcML(xmlTextWriterPtr writer);

  // reads the next unit and returns it srcML
  std::string * readsrcML();

};

#endif
