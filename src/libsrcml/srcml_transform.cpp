/*
  srcml_transform.cpp

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
#include <stdio.h>

/* srcML XPath query and XSLT transform functions */
int srcml_add_transform_xpath(srcml_archive* archive, const char* xpath_string) {

  transform tran = { SRCML_XPATH, xpath_string };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

int srcml_add_transform_xslt(srcml_archive* archive, const char* xslt_filename) {

  transform tran = { SRCML_XSLT, xslt_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

int srcml_add_transform_relaxng(srcml_archive* archive, const char* relaxng_filename) {

  transform tran = { SRCML_RELAXNG, relaxng_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}


// TODO finish.  what happends to intermediate results?
int srcml_apply_transforms(srcml_archive* iarchive, srcml_archive* oarchive) {

  static char * transform_filename_template = "srcml_transform_XXXXXXXX";

  //switch to mkstemp
  //int mkstemp(char *template);
  char * transform_filename = mktemp(transform_filename_template);

  iarchive->transformations.clear();

  return SRCML_STATUS_OK;

}
