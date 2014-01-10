/**
 * @file srcml_transform.cpp
 * @copyright
 *
 * Copyright (C) 2013-2014  SDML (www.srcML.org)
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

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcml_sax2_utilities.hpp>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/** 
 * srcml_append_transform_xpath
 * @param archive a srcml archive
 * @param xpath_string an XPath expression
 *
 * Append the XPath expression to the list
 * of transformation/queries.  As of yet no way to specify context
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_append_transform_xpath(srcml_archive* archive, const char* xpath_string) {

  if(archive == NULL || xpath_string == 0) return SRCML_STATUS_ERROR;
  if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_ERROR;

  transform tran = { SRCML_XPATH, xpath_string };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

/**
 * srcml_append_transform_xslt
 * @param archive a srcml_archive
 * @param xslt_filename an XSLT program filename path
 *
 * Append the XSLT program filename path to the list 
 * of transformation/queries.  As of yet no way to specify parameters or context
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_append_transform_xslt(srcml_archive* archive, const char* xslt_filename) {

  if(archive == NULL || xslt_filename == 0) return SRCML_STATUS_ERROR;
  if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_ERROR;

  transform tran = { SRCML_XSLT, xslt_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

/**
 * srcml_append_transform_relaxng
 * @param archive a srcml archive
 * @param relaxng_filename a RelaxNG schema filename path
 *
 * Append the RelaxNG schema filename path to the list
 * of transformation/queries.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
 int srcml_append_transform_relaxng(srcml_archive* archive, const char* relaxng_filename) {

  if(archive == NULL || relaxng_filename == 0) return SRCML_STATUS_ERROR;
  if(archive->type != SRCML_ARCHIVE_READ && archive->type != SRCML_ARCHIVE_RW) return SRCML_STATUS_ERROR;

  transform tran = { SRCML_RELAXNG, relaxng_filename };
  archive->transformations.push_back(tran);

  return SRCML_STATUS_OK;

}

/**
 * srcml_clear_transforms
 * @param archive an archive
 *
 * Remove all transformations from archive.
 *
 * @returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure. 
 */
int srcml_clear_transforms(srcml_archive * archive) {

  if(archive == NULL) return SRCML_STATUS_ERROR;

  archive->transformations.clear();

  return SRCML_STATUS_OK;
  
}

/**
 * srcml_apply_transforms
 * @param iarchive an input srcml archive
 * @param oarchive and output srcml archive
 *
 * Apply appended transformations inorder added and consecutively.
 * Intermediate results are stored in a temporary file.
 * Transformations are cleared.
 *
 * @returns Returns SRCML_STATUS_OK on success and SRCML_STATUS_ERROR on failure.
 */
int srcml_apply_transforms(srcml_archive* iarchive, srcml_archive* oarchive) {

  if(iarchive == NULL || oarchive == NULL) return SRCML_STATUS_ERROR;

  static const char * transform_filename_template = "srcml_transform_XXXXXXXX";

  const char * last_transform_filename = 0;
  for(unsigned int i = 0; i < iarchive->transformations.size(); ++i) {

    char * transform_filename = strdup(transform_filename_template);
    if(!transform_filename) {

      if(last_transform_filename) unlink(last_transform_filename);
      free((void *)last_transform_filename);
      return SRCML_STATUS_ERROR;

    }

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
    int error = 0;
    try {

      switch(iarchive->transformations.at(i).type) {

      case SRCML_XPATH: {

          const char * xpaths[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          error = srcml_xpath(pinput, "src:unit", xpaths, transform_fd, oarchive->options);
          break;
        }

      case SRCML_XSLT: {

          const char * xslts[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          const char * params[1] = { 0 };
          error = srcml_xslt(pinput, "src:unit", xslts, params, 0, transform_fd, oarchive->options);
          break;
        }

      case SRCML_RELAXNG: {

          const char * relaxngs[2] = { iarchive->transformations.at(i).transformation.c_str(), 0 };
          error = srcml_relaxng(pinput, relaxngs, transform_fd, oarchive->options);
          break;
        }

      default : break;

      }

    } catch(...) {
 
      close(transform_fd);
      if(i != 0) xmlFreeParserInputBuffer(pinput);
      if(last_transform_filename)  unlink(last_transform_filename);
      free((void *)last_transform_filename);
  
      return SRCML_STATUS_ERROR;
    }

    if(i != 0) xmlFreeParserInputBuffer(pinput);
    if(last_transform_filename) unlink(last_transform_filename);
    free((void *)last_transform_filename);
    last_transform_filename = transform_filename;
    if(error == SRCML_STATUS_ERROR) {
      if(last_transform_filename) unlink(last_transform_filename);
      free((void *)last_transform_filename);
      return error;
    }

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
  if(last_transform_filename) unlink(last_transform_filename);
  free((void *)last_transform_filename);

  iarchive->transformations.clear();

  return SRCML_STATUS_OK;

}
