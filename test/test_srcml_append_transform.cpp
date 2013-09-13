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

  /* 
     srcml_append_transform_xpath
   */

  {
    srcml_archive * archive = srcml_create_archive();
    srcml_append_transform_xpath(archive, "//src:unit");

    assert(archive->transformations.back().type == SRCML_XPATH);

    srcml_free_archive(archive);  
  }


  return 0;

}
