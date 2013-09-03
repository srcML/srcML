#include <stdio.h>
#include <string.h>

#include "srcml.h"

int main(int argc, char * argv[]) {

  struct srcml_archive * iarchive = srcml_create_archive();
  struct srcml_archive * oarchive = srcml_create_archive();
  srcml_read_open_filename(iarchive, "project.xml");
  srcml_write_open_filename(oarchive, "xslt.xml");
  
  srcml_append_transform_xslt(iarchive, "copy.xsl");

  srcml_apply_transforms(iarchive, oarchive);

  srcml_close_archive(iarchive);
  srcml_close_archive(oarchive);

  srcml_free_archive(iarchive);
  srcml_free_archive(oarchive);

  return 0;
}
