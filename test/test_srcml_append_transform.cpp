/*
  test_srcml_append_transform.cpp

  Copyright (C) 2013  SDML (www.srcML.org)

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

  Test cases for srcml_append_transform_*
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

int main(int argc, char * argv[]) {

    std::string s = "<unit/>";

  /* 
     srcml_append_transform_xpath
   */

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, s.c_str(), s.size());
    srcml_append_transform_xpath(archive, "//src:unit");

    assert(archive->transformations.back().type == SRCML_XPATH);
    assert(archive->transformations.back().transformation == "//src:unit");

    srcml_close_archive(archive);
    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    assert(srcml_append_transform_xpath(archive, "//src:unit") == SRCML_STATUS_ERROR);

    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, s.c_str(), s.size());
    assert(srcml_append_transform_xpath(archive, 0) == SRCML_STATUS_ERROR);

    srcml_close_archive(archive);
    srcml_free_archive(archive);  
  }

  {
    assert(srcml_append_transform_xpath(0, "//src:unit") == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_append_transform_xslt
   */

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, s.c_str(), s.size());
    srcml_append_transform_xslt(archive, "copy.xsl");

    assert(archive->transformations.back().type == SRCML_XSLT);
    assert(archive->transformations.back().transformation == "copy.xsl");

    srcml_close_archive(archive);
    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    assert(srcml_append_transform_xslt(archive, "copy.xsl") == SRCML_STATUS_ERROR);

    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, s.c_str(), s.size());
    assert(srcml_append_transform_xslt(archive, 0) == SRCML_STATUS_ERROR);

    srcml_close_archive(archive);  
    srcml_free_archive(archive);  
  }

  {
    assert(srcml_append_transform_xslt(0, "copy.xsl") == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_append_transform_relaxng
   */

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, s.c_str(), s.size());
    srcml_append_transform_relaxng(archive, "schema.rng");

    assert(archive->transformations.back().type == SRCML_RELAXNG);
    assert(archive->transformations.back().transformation == "schema.rng");

    srcml_close_archive(archive);  
    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    assert(srcml_append_transform_relaxng(archive, "schema.rng") == SRCML_STATUS_ERROR);

    srcml_free_archive(archive);  
  }

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, s.c_str(), s.size());
    assert(srcml_append_transform_relaxng(archive, 0) == SRCML_STATUS_ERROR);

    srcml_close_archive(archive);  
    srcml_free_archive(archive);  
  }

  {
    assert(srcml_append_transform_relaxng(0, "schema.rng") == SRCML_STATUS_ERROR);
  }

  /* 
     srcml_append_transform_*
   */

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_read_open_memory(archive, s.c_str(), s.size());
    srcml_append_transform_xpath(archive, "//src:unit");
    srcml_append_transform_xslt(archive, "copy.xsl");
    srcml_append_transform_relaxng(archive, "schema.rng");

    assert(archive->transformations.at(0).type == SRCML_XPATH);
    assert(archive->transformations.at(0).transformation == "//src:unit");
    assert(archive->transformations.at(1).type == SRCML_XSLT);
    assert(archive->transformations.at(1).transformation == "copy.xsl");
    assert(archive->transformations.back().type == SRCML_RELAXNG);
    assert(archive->transformations.back().transformation == "schema.rng");

    srcml_close_archive(archive);
    srcml_free_archive(archive);  
  }

  srcml_cleanup_globals();

  return 0;

}
