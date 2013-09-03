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

#include "../srcMLUtility.hpp"

#include <stdio.h>

/* srcML XPath query and XSLT transform functions */
int srcml_append_transform_xpath(srcml_archive* archive, const char* xpath_string) {

  transform tran = { SRCML_XPATH, xpath_string };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

int srcml_append_transform_xslt(srcml_archive* archive, const char* xslt_filename) {

  transform tran = { SRCML_XSLT, xslt_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

int srcml_append_transform_relaxng(srcml_archive* archive, const char* relaxng_filename) {

  transform tran = { SRCML_RELAXNG, relaxng_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

void libxml_error(void *ctx, const char *msg, ...) {}

// TODO What happends to intermediate results?
int srcml_apply_transforms(srcml_archive* iarchive, srcml_archive* oarchive) {

  const char * transform_filename_template = "srcml_transform_XXXXXXXX";

  int input = 0;
  const char * last_transform_filename = 0;
  for(int i = 0; i < iarchive->transformations.size(); ++i) {

    char * transform_filename = strdup(transform_filename_template);
    int transform_fd = mkstemp(transform_filename);
    OPTION_TYPE save_options = oarchive->options;

    xmlParserInputBufferPtr pinput = 0;
    if(i == 0) pinput = iarchive->input;
    else pinput = xmlParserInputBufferCreateFilename(last_transform_filename, xmlParseCharEncoding(0));
    srcMLUtility utility(pinput, oarchive->encoding ? oarchive->encoding->c_str() : "UTF-8", oarchive->options);

    try {

      switch(iarchive->transformations.at(i).type) {

      case SRCML_XPATH:

        {

          oarchive->options |= OPTION_XPATH;
          const char * xpaths[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          utility.xpath(0, "src:unit", xpaths, transform_fd);
          break;
        }

      case SRCML_XSLT:

        {

          oarchive->options |= OPTION_XSLT;
          const char * xslts[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          const char * params[1] = { 0 };
          utility.xslt("src:unit", 0, xslts, params, 0, transform_fd);
          break;
        }

      case SRCML_RELAXNG:

        {

          // TODO fix so actually works.  Suppresses error messages.
          xmlGenericErrorFunc handler = (xmlGenericErrorFunc) libxml_error;
          initGenericErrorDefaultFunc(&handler);

          oarchive->options |= OPTION_RELAXNG;
          const char * relaxngs[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          utility.relaxng(0, relaxngs, transform_fd);
          break;
        }

      default :
        break;

      }

    } catch(...) {}


    if(i != 0) xmlFreeParserInputBuffer(pinput);
    unlink(last_transform_filename);
    free((void *)last_transform_filename);
    last_transform_filename = transform_filename;
    oarchive->options = save_options;

  }



  srcml_archive * tmp_archive = srcml_create_archive();

  srcml_read_open_filename(tmp_archive, last_transform_filename);

  srcml_unit * unit;
  while((unit = srcml_read_unit(tmp_archive))) {

    srcml_write_unit(oarchive, unit);
    srcml_free_unit(unit);

  }

  srcml_close_archive(tmp_archive);
  srcml_free_archive(tmp_archive);
  unlink(last_transform_filename);
  free((void *)last_transform_filename);

  iarchive->transformations.clear();

  return SRCML_STATUS_OK;

}
