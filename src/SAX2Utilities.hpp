/*
  SAX2Utilities.hpp

  Copyright (C) 2009-2010  SDML (www.sdml.info)

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
*/

#ifndef INCLUDED_SAX2UTILITIES_HPP
#define INCLUDED_SAX2UTILITIES_HPP

#include <string>
#include <libxml/parser.h>

typedef std::pair<std::string, std::string> PROPERTIES_TYPE[32];
const int MAXPROPERTIES = 32;

// expanded prefix for xmlns declaration
const char* xmlnsprefix(const char* prefix);

const char* qname(const char* prefix, const char* localname);

const char* find(PROPERTIES_TYPE& pv, const char* name);

// index of attribute in attributes
int find_attribute_index(int nb_attributes, const xmlChar** attributes, const char* attribute);

// collect attributes
void collect_attributes(int nb_attributes, const xmlChar** attributes,
			PROPERTIES_TYPE& attrv);

// collect namespaces
void collect_namespaces(int nb_namespaces, const xmlChar** namespaces,
			PROPERTIES_TYPE& nsv);
#endif

