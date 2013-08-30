#include "srcml.h"
#include "srcml_types.hpp"

#include "../srcMLUtility.hpp"

/* srcML attributes with namespaces (header read only)*/
const char** srcml_info(const char* srcml_filename) {

  OPTION_TYPE options = OPTION_INFO | OPTION_NAMESPACE;
  srcMLUtility utility(srcml_filename, "UTF-8", options);
  int optioncount = 5;
  int optionorder[] = { OPTION_XML_ENCODING, OPTION_LANGUAGE, OPTION_DIRECTORY, OPTION_FILENAME, OPTION_VERSION };
  std::vector<std::string> output_array;
  utility.move_to_unit(0, utility, options, optioncount, optionorder, output_array);

  const char ** output_carray = (const char **)malloc((output_array.size() + 1) * sizeof(const char *));

  for(int i = 0; i < output_array.size(); ++i)
    output_carray[i] = strdup(output_array.at(i).c_str());
  output_carray[output_array.size()] = 0;

  return output_carray;

}

/* srcML attributes with namespaces and number of units (complete file read) */
const char** srcml_longinfo(const char* srcml_filename) {

  OPTION_TYPE options = OPTION_LONG_INFO | OPTION_NAMESPACE;
  srcMLUtility utility(srcml_filename, "UTF-8", options);
  int optioncount = 5;
  int optionorder[] = { OPTION_XML_ENCODING, OPTION_LANGUAGE, OPTION_DIRECTORY, OPTION_FILENAME, OPTION_VERSION };
  std::vector<std::string> output_array;
  utility.move_to_unit(-1, utility, options, optioncount, optionorder, output_array);

  const char ** output_carray = (const char **)malloc((output_array.size() + 1) * sizeof(const char *));

  for(int i = 0; i < output_array.size(); ++i)
    output_carray[i] = strdup(output_array.at(i).c_str());
  output_carray[output_array.size()] = 0;

  return output_carray;

}

/* srcML attributes with namespaces of a particular unit in an archive */
const char** srcml_info_unit(const char* srcml_filename, int unit) {

  OPTION_TYPE options = OPTION_INFO | OPTION_NAMESPACE;
  srcMLUtility utility(srcml_filename, "UTF-8", options);
  int optioncount = 5;
  int optionorder[] = { OPTION_XML_ENCODING, OPTION_LANGUAGE, OPTION_DIRECTORY, OPTION_FILENAME, OPTION_VERSION };
  std::vector<std::string> output_array;
  utility.move_to_unit(unit, utility, options, optioncount, optionorder, output_array);

  const char ** output_carray = (const char **)malloc((output_array.size() + 1) * sizeof(const char *));

  for(int i = 0; i < output_array.size(); ++i)
    output_carray[i] = strdup(output_array.at(i).c_str());
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

  for(int i = 0; i < output_array.size(); ++i)
    output_carray[i] = strdup(output_array.at(i).c_str());
  output_carray[output_array.size()] = 0;

  return output_carray;

}

