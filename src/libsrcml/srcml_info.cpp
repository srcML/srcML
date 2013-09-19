/**
 * @copyright
 *
 * srcml_info.cpp
 *
 * Copyright (C) 2013  SDML (www.sdml.info)
 *
 * The srcML translator is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * The srcML translator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the srcML translator; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <srcml.h>
#include <srcml_types.hpp>

/**
 * srcml_list
 * @param srcml_filename name of a file
 *
 * List all of the filenames in the srcML Archive srcml_filename
 *
 * @returns Return NULL terminated array on success and 0 on failure.
 */
const char** srcml_list(const char* srcml_filename) {

  if(srcml_filename == NULL) return 0;

  srcml_archive * archive = srcml_create_archive();
  if(srcml_read_open_filename(archive, srcml_filename) == SRCML_STATUS_ERROR) return 0;
  std::vector<std::string> output_array;
  if(srcml_archive_get_filename(archive))
    output_array.push_back(srcml_archive_get_filename(archive));
  srcml_unit * unit;
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

