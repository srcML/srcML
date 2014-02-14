/*
  test_srcml_archive_get.cpp

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

  Test cases for srcml_archive_get_*.
*/
#include <stdio.h>
#include <string.h>
#include <cassert>

#include <srcml.h>
#include <srcml_types.hpp>
#include <srcmlns.hpp>

#include "dassert.hpp"

#include <boost/optional.hpp>

int main() {

  /* 
     srcml_archive_get_encoding
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->encoding = boost::optional<std::string>();
    dassert(srcml_archive_get_encoding(archive), 0);
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_archive_get_encoding(0), 0);
  }


  {

    srcml_archive * archive = srcml_create_archive();
    archive->encoding = "foo";
    dassert(srcml_archive_get_encoding(archive), std::string("foo"));
    srcml_free_archive(archive);
  }

  /* 
     srcml_archive_get_language
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->language = boost::optional<std::string>();
    dassert(srcml_archive_get_language(archive), 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    archive->language = "foo";
    dassert(srcml_archive_get_language(archive), std::string("foo"));
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_archive_get_language(0), 0);
  }

  /* 
     srcml_archive_get_filename
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->filename = boost::optional<std::string>();
    dassert(srcml_archive_get_filename(archive), 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    archive->filename = "foo";
    dassert(srcml_archive_get_filename(archive), std::string("foo"));
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_archive_get_filename(0), 0);
  }

  /* 
     srcml_archive_get_directory
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->directory = boost::optional<std::string>();
    dassert(srcml_archive_get_directory(archive), 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    archive->directory = "foo";
    dassert(srcml_archive_get_directory(archive), std::string("foo"));
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_archive_get_directory(0), 0);
  }

  /* 
     srcml_archive_get_version
   */

  {

    srcml_archive * archive = srcml_create_archive();
    archive->version = boost::optional<std::string>();
    dassert(srcml_archive_get_version(archive), 0);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();
    archive->version = "foo";
    dassert(srcml_archive_get_version(archive), std::string("foo"));
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_archive_get_version(0), 0);
  }

  /*
    srcml_archive_get_option
   */

  {

    srcml_archive * archive = srcml_create_archive();

    archive->options = 1;

    dassert(srcml_archive_get_options(archive), 1);
    srcml_free_archive(archive);
  }

  {

    srcml_archive * archive = srcml_create_archive();

    archive->options = 1 | 2;

    dassert(srcml_archive_get_options(archive), (1 | 2));
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_archive_get_options(0), 0);
  }

  /*
    srcml_archive_get_tabstop
  */

  {

    srcml_archive * archive = srcml_create_archive();

    archive->tabstop = 4;

    dassert(srcml_archive_get_tabstop(archive), 4);
    srcml_free_archive(archive);
  }

  {
    dassert(srcml_archive_get_tabstop(0), 0);
  }

  /* 
     srcml_get_namespace_size
   */

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_namespace_size(archive), 7);

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    srcml_archive_register_namespace(archive, "foo1", "bar1");
    srcml_archive_register_namespace(archive, "foo2", "bar2");
    dassert(srcml_archive_get_namespace_size(archive), 9);

    srcml_free_archive(archive);

  }

  {
    dassert(srcml_archive_get_namespace_size(0), -1);
  }

  /* 
     srcml_archive_get_prefix
   */

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_prefix(archive, 1), std::string("cpp"));

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_prefix(archive, -1), 0);

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_prefix(archive, 7), 0);

    srcml_free_archive(archive);

  }

  {
    dassert(srcml_archive_get_prefix(0, 0), 0);
  }

  /* 
     srcml_archive_get_prefix_uri
   */
  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_prefix_uri(archive, "http://www.sdml.info/srcML/cpp"), std::string("cpp"));

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_prefix_uri(archive, "bar3"), 0);

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_prefix_uri(archive, 0), 0);

    srcml_free_archive(archive);

  }

  {
    dassert(srcml_archive_get_prefix_uri(0, "http://www.sdml.info/srcML/cpp"), 0);
  }

  /* 
     srcml_archive_get_namespace
   */

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_namespace(archive, 1), std::string("http://www.sdml.info/srcML/cpp"));

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_namespace(archive, -1), 0);

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_namespace(archive, 7), 0);

    srcml_free_archive(archive);

  }

  {
    dassert(srcml_archive_get_namespace(0, 0), 0);
  }

  /* 
     srcml_archive_get_namespace_prefix
   */

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_namespace_prefix(archive, "cpp"), std::string("http://www.sdml.info/srcML/cpp"));

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_namespace_prefix(archive, "foo3"), 0);

    srcml_free_archive(archive);

  }

  {

    srcml_archive * archive = srcml_create_archive();

    dassert(srcml_archive_get_namespace_prefix(archive, 0), 0);

    srcml_free_archive(archive);

  }

  {
    dassert(srcml_archive_get_namespace_prefix(0, "cpp"), 0);
  }

  return 0;
}
