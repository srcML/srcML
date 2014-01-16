/*
  test_srcml_clear_transforms.cpp

  Copyright (C) 2013-2014  SDML (www.srcML.org)

  The srcML Toolkit is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  The srcML Toolkit is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with the srcML Toolkit; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*

  Test cases for srcml_clear_transforms
*/
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <fstream>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include "dassert.hpp"

int main() {

  /* 
     srcml_clear_transforms
   */

  {
    srcml_archive * archive = srcml_create_archive();
    archive->type = SRCML_ARCHIVE_RW;
    dassert(archive->transformations.size(), 0);
    dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
    dassert(archive->transformations.size(), 0);
    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    archive->type = SRCML_ARCHIVE_RW;
    srcml_append_transform_xpath(archive, "//src:unit");
    dassert(!archive->transformations.size(), 0);
    dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
    dassert(archive->transformations.size(), 0);
    srcml_free_archive(archive);  
  }


  {
    srcml_archive * archive = srcml_create_archive();
    archive->type = SRCML_ARCHIVE_RW;
    srcml_append_transform_xslt(archive, "copy.xsl");
    dassert(!archive->transformations.size(), 0);
    dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
    dassert(archive->transformations.size(), 0);
    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    archive->type = SRCML_ARCHIVE_RW;
    srcml_append_transform_relaxng(archive, "schema.rng");
    dassert(!archive->transformations.size(), 0);
    dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
    dassert(archive->transformations.size(), 0);
    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    archive->type = SRCML_ARCHIVE_RW;
    srcml_append_transform_xpath(archive, "//src:unit");
    srcml_append_transform_xslt(archive, "copy.xsl");
    srcml_append_transform_relaxng(archive, "schema.rng");
    dassert(!archive->transformations.size(), 0);
    dassert(srcml_clear_transforms(archive), SRCML_STATUS_OK);
    dassert(archive->transformations.size(), 0);
    srcml_free_archive(archive);  
  }

  {
    dassert(srcml_clear_transforms(0), SRCML_STATUS_ERROR);
  }

  return 0;

}
