#include <stdio.h>
#include <string.h>

#include "srcml.h"

int main(int argc, char * argv[]) {

  struct srcml_archive * iarchive = srcml_create_archive();
  struct srcml_archive * oarchive = srcml_create_archive();
  srcml_write_open_filename(oarchive, "relaxng.xml");

  srcml_archive_set_filename(iarchive, "project.xml");
  
  srcml_append_transform_relaxng(iarchive, "schema.rng");

  srcml_apply_transforms(iarchive, oarchive);

  srcml_close_archive(oarchive);
  srcml_free_archive(oarchive);

  return 0;
}
