/*
  SAX2Utilities.cpp

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

#include "SAX2Utilities.hpp"
#include <cstring>
#include <iostream>

// expanded prefix for xmlns declaration
const char* xmlnsprefix(const char* prefix) {

  static std::string xmlns;

  if (prefix) {
    xmlns = "xmlns:";
    xmlns += prefix;
    return xmlns.c_str();

  } else
    return "xmlns";
}

const char* qname(const char* prefix, const char* localname) {

  static std::string tag;

  if (prefix) {
    tag = prefix;
    tag += ':';
    tag += localname;
    return tag.c_str();
  } else
    return localname;
}

/*
PROPERTIES_TYPE::const_iterator find(const PROPERTIES_TYPE& pv, const char* name) {

  for (PROPERTIES_TYPE::const_iterator pos = pv.begin(); pos != pv.end(); ++pos)
    if (pos->first == name)
      return pos;

  return pv.end();
}
*/

const char* find(PROPERTIES_TYPE& pv, const char* name) {

  for (int i = 0; i < MAXPROPERTIES; ++i) {
    if (pv[i].first == "")
      return 0;

    if (pv[i].first == name)
      return pv[i].second.c_str();
  }

  return 0;
}

// index of attribute in attributes
int find_attribute_index(int nb_attributes, const xmlChar** attributes, const char* attribute) {

    for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5)
      if (strcmp((const char*) attributes[index], attribute) == 0)
	return index;

    return -1;
}

// collect attributes
void collect_attributes(int nb_attributes, const xmlChar** attributes, PROPERTIES_TYPE& attrv) {

  for (int i = 0, index = 0; i < nb_attributes; ++i, index += 5) {

      const char* name = qname((const char*) attributes[index + 1], (const char*) attributes[index]);

      // look for it
      int i;
      for (i = 0; i < MAXPROPERTIES; ++i)
	if (attrv[i].first == "" || attrv[i].first == name)
	  break;

      // if new, then stick in the name
      if (attrv[i].first == "")
	attrv[i].first = name;

      // stick in the value
      attrv[i].second.assign((const char*) attributes[index + 3], (const char*)  attributes[index + 4]);
    }
}

// collect namespaces
void collect_namespaces(int nb_namespaces, const xmlChar** namespaces, PROPERTIES_TYPE& nsv) {

  for (int i = 0, index = 0; i < nb_namespaces; ++i, index += 2) {

    const char* uri = (const char*) namespaces[index + 1];

    // look for it
    int i;
    for (i = 0; i < MAXPROPERTIES; ++i)
      if (nsv[i].first == "" || nsv[i].first == uri)
	  break;

    // if new, then stick in the name
    if (nsv[i].first == "")
      nsv[i].first = uri;

    nsv[i].second = xmlnsprefix((const char*) namespaces[index]);
  }
}
