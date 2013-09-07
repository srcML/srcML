/*
  srcml_info.cpp

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

#include "srcml.h"
#include "srcml_types.hpp"

#include "../srcMLUtility.hpp"

#define SRCML_OPTION_INFO 1 << 19
#define SRCML_OPTION_NAMESPACE 1 << 26
#define SRCML_OPTION_LONG_INFO 1 << 20
#define SRCML_OPTION_XML_ENCODING 1 << 13
#define SRCML_OPTION_LANGUAGE 1 << 12
#define SRCML_OPTION_DIRECTORY 1 << 7
#define SRCML_OPTION_FILENAME 1 << 6
#define SRCML_OPTION_VERSION 1 << 8

int optioncount = 5;
int optionorder[] = { SRCML_OPTION_XML_ENCODING, SRCML_OPTION_LANGUAGE, SRCML_OPTION_DIRECTORY, SRCML_OPTION_FILENAME, SRCML_OPTION_VERSION };


/* srcML attributes with namespaces (header read only)*/
const char** srcml_info(const char* srcml_filename) {

  OPTION_TYPE options = SRCML_OPTION_INFO | SRCML_OPTION_NAMESPACE;
  srcMLUtility utility(srcml_filename, "UTF-8", options);
  std::vector<std::string> output_array;
  utility.move_to_unit(0, utility, options, optioncount, optionorder, output_array);

  const char ** output_carray = (const char **)malloc((output_array.size() + 1) * sizeof(const char *));

  try {

  for(int i = 0; i < output_array.size(); ++i)
    output_carray[i] = strdup(output_array.at(i).c_str());

  } catch(...) {}
  output_carray[output_array.size()] = 0;

  return output_carray;

}

/* srcML attributes with namespaces and number of units (complete file read) */
const char** srcml_longinfo(const char* srcml_filename) {

  OPTION_TYPE options = SRCML_OPTION_LONG_INFO | SRCML_OPTION_NAMESPACE;
  srcMLUtility utility(srcml_filename, "UTF-8", options);
  std::vector<std::string> output_array;
  utility.move_to_unit(-1, utility, options, optioncount, optionorder, output_array);

  const char ** output_carray = (const char **)malloc((output_array.size() + 1) * sizeof(const char *));

  try {

    for(int i = 0; i < output_array.size(); ++i)
      output_carray[i] = strdup(output_array.at(i).c_str());

  } catch(...) {}
  output_carray[output_array.size()] = 0;

  return output_carray;

}

/* srcML attributes with namespaces of a particular unit in an archive */
const char** srcml_info_unit(const char* srcml_filename, int unit) {

  OPTION_TYPE options = SRCML_OPTION_INFO | SRCML_OPTION_NAMESPACE;
  srcMLUtility utility(srcml_filename, "UTF-8", options);
  std::vector<std::string> output_array;
  utility.move_to_unit(unit, utility, options, optioncount, optionorder, output_array);

  const char ** output_carray = (const char **)malloc((output_array.size() + 1) * sizeof(const char *));

  try {

    for(int i = 0; i < output_array.size(); ++i)
      output_carray[i] = strdup(output_array.at(i).c_str());

  } catch(...) {}
  output_carray[output_array.size()] = 0;

  return output_carray;

}

/* list of filenames */
const char** srcml_list(const char* srcml_filename) {

  srcml_archive * archive = srcml_create_archive();
  srcml_read_open_filename(archive, srcml_filename);
  srcml_unit * unit;
  std::vector<std::string> output_array;
  while((unit = srcml_read_unit(archive)))
    output_array.push_back(srcml_unit_get_filename(unit));

  const char ** output_carray = (const char **)malloc((output_array.size() + 1) * sizeof(const char *));

  try {
  for(int i = 0; i < output_array.size(); ++i)
    output_carray[i] = strdup(output_array.at(i).c_str());

  } catch(...) {}
  output_carray[output_array.size()] = 0;

  return output_carray;

}

