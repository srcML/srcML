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

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_sax2_utilities.hpp>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* srcML XPath query and XSLT transform functions */
// As of yet no way to specify context
int srcml_append_transform_xpath(srcml_archive* archive, const char* xpath_string) {

  if(archive == NULL) return SRCML_STATUS_ERROR;

  transform tran = { SRCML_XPATH, xpath_string };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

// As of yet no way to specify parameters or context
int srcml_append_transform_xslt(srcml_archive* archive, const char* xslt_filename) {

  if(archive == NULL) return SRCML_STATUS_ERROR;

  transform tran = { SRCML_XSLT, xslt_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

int srcml_append_transform_relaxng(srcml_archive* archive, const char* relaxng_filename) {

  if(archive == NULL) return SRCML_STATUS_ERROR;

  transform tran = { SRCML_RELAXNG, relaxng_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

/*

  Apply appended transformations inorder added and consecutively.
  Intermediate results are stored in a temporary file.

 */
int srcml_apply_transforms(srcml_archive* iarchive, srcml_archive* oarchive) {

  if(iarchive == NULL || oarchive == NULL) return SRCML_STATUS_ERROR;

  static const char * transform_filename_template = "srcml_transform_XXXXXXXX";

  const char * last_transform_filename = 0;
  for(unsigned int i = 0; i < iarchive->transformations.size(); ++i) {

    char * transform_filename = strdup(transform_filename_template);
#if defined(__GNUG__) && !defined(__MINGW32__)
    int transform_fd = mkstemp(transform_filename);
#else
    mktemp(transform_filename);
    int transform_fd = open(transform_filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
#endif

    xmlParserInputBufferPtr pinput = 0;
    if(i == 0) pinput = iarchive->input;
    else pinput = xmlParserInputBufferCreateFilename(last_transform_filename, xmlParseCharEncoding(0));

    if(pinput == NULL) {

      close(transform_fd);
      free((void *)transform_filename);
      return SRCML_STATUS_ERROR;

    }      

    try {

      switch(iarchive->transformations.at(i).type) {

      case SRCML_XPATH: {

          const char * xpaths[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          xpath(pinput, "src:unit", xpaths, transform_fd, oarchive-> options);
          break;
        }

      case SRCML_XSLT: {

          const char * xslts[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          const char * params[1] = { 0 };
          xslt(pinput, "src:unit", xslts, params, 0, transform_fd, oarchive->options);
          break;
        }

      case SRCML_RELAXNG: {

          const char * relaxngs[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          relaxng(pinput, relaxngs, transform_fd, oarchive->options);
          break;
        }

      default : break;

      }

    } catch(...) {
 
      close(transform_fd);
      if(i != 0) xmlFreeParserInputBuffer(pinput);
      unlink(last_transform_filename);
      free((void *)last_transform_filename);
  
      return SRCML_STATUS_ERROR;
    }

    if(i != 0) xmlFreeParserInputBuffer(pinput);
    unlink(last_transform_filename);
    free((void *)last_transform_filename);
    last_transform_filename = transform_filename;

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
